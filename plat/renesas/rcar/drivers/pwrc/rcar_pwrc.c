/*
 * Copyright (c) 2013-2014, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2015-2016, Renesas Electronics Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of ARM nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <string.h>
#include <bakery_lock.h>
#include <mmio.h>
#include <debug.h>
#include <arch.h>
#include <arch_helpers.h>
#include "rcar_def.h"
#include "rcar_private.h"
#include "rcar_pwrc.h"
#include "iic_dvfs.h"

/*
 * TODO: Someday there will be a generic power controller api. At the moment
 * each platform has its own pwrc so just exporting functions is fine.
 */
RCAR_INSTANTIATE_LOCK

#define	WUP_IRQ_SHIFT	(0U)
#define	WUP_FIQ_SHIFT	(8U)
#define	WUP_CSD_SHIFT	(16U)

#define	BIT_SOFTRESET	((uint32_t)1U<<15)
#define	BIT_CA53_SCU	((uint32_t)1U<<21)
#define	BIT_CA57_SCU	((uint32_t)1U<<12)
#define	REQ_RESUME	((uint32_t)1U<<1)
#define	REQ_OFF		((uint32_t)1U<<0)
#define	STATUS_PWRUP	((uint32_t)1U<<4)
#define	STATUS_PWRDOWN	((uint32_t)1U<<0)

#define	STATE_CA57_CPU	(27U)
#define	STATE_CA53_CPU	(22U)

#define	STATUS_L2RST	((uint32_t)0U<<4)
#define	MODE_L2_DOWN	(0x00000002U)
#define	CPU_PWR_OFF	(0x00000003U)

#define	RCAR_PSTR_MASK	(0x00000003U)
#define	ST_ALL_STANDBY	(0x00003333U)

/* for suspend to ram	*/
/* DBSC Defines */
#define	DBSC4_REG_BASE			(0xE6790000U)
#define	DBSC4_REG_DBACEN		(DBSC4_REG_BASE + 0x0200U)
#define	DBSC4_REG_DBCMD			(DBSC4_REG_BASE + 0x0208U)
#define	DBSC4_REG_DBRFEN		(DBSC4_REG_BASE + 0x0204U)
#define	DBSC4_REG_DBWAIT		(DBSC4_REG_BASE + 0x0210U)

#define	DBSC4_BIT_DBACEN_ACCEN		((uint32_t)(1U << 0))
#define	DBSC4_BIT_DBRFEN_ARFEN		((uint32_t)(1U << 0))
#define	DBSC4_SET_DBCMD_OPC_PRE		(0x04000000U)
#define	DBSC4_SET_DBCMD_OPC_SR		(0x0A000000U)
#define	DBSC4_SET_DBCMD_OPC_PD		(0x08000000U)
#define	DBSC4_SET_DBCMD_CH_ALL		(0x00800000U)
#define	DBSC4_SET_DBCMD_RANK_ALL	(0x00040000U)
#define	DBSC4_SET_DBCMD_ARG_ALL		(0x00000010U)
#define	DBSC4_SET_DBCMD_ARG_ENTER	(0x00000000U)

#if PMIC_ROHM_BD9571
/* PMIC for BD9571MWV-M*/
#define	PMIC_SLAVE_ADDR			(0x30U)
#define	PMIC_BKUP_MODE_CNT		(0x20U)
#define	BIT_BKUP_CTRL_OUT		((uint8_t)(1U << 4))

#define	PMIC_RETRY_MAX			(100U)
#endif /* PMIC_ROHM_BD9571 */

/* sctlr_el3 M bit (for QAC) */
#define	SCTLR_EL3_M_BIT			((uint32_t)1U << 0)

/* prototype */
#if RCAR_SYSTEM_SUSPEND
static void rcar_bl31_set_self_refresh(void);
#endif /* RCAR_SYSTEM_SUSPEND */
static void SCU_power_up(uint64_t mpidr);

uint32_t rcar_pwrc_status(uint64_t mpidr)
{
	uint32_t rc;
	uint64_t cpu_no;
	uint32_t prr_data;

	rcar_lock_get();
	prr_data = mmio_read_32((uintptr_t)RCAR_PRR);
	cpu_no = mpidr & (uint64_t)MPIDR_CPU_MASK;
	if ((mpidr & ((uint64_t)MPIDR_CLUSTER_MASK)) != 0U) {
		/* A53 side				*/
		if ((prr_data & ((uint32_t)1U << (STATE_CA53_CPU + cpu_no)))
				== 0U) {
			rc = 0U;
		} else {
			rc = RCAR_INVALID;
		}
	} else {
		/* A57 side				*/
		if ((prr_data & ((uint32_t)1U << (STATE_CA57_CPU + cpu_no)))
				== 0U) {
			rc = 0U;
		} else {
			rc = RCAR_INVALID;
		}
	}
	rcar_lock_release();

	return rc;
}

void rcar_pwrc_cpuon(uint64_t mpidr)
{

	uintptr_t res_reg;
	uint32_t res_data;
	uintptr_t on_reg;
	uint64_t cpu_no;
	uint32_t upper_value;
	uint32_t wup_data;

	rcar_lock_get();

	cpu_no = mpidr & (uint64_t)MPIDR_CPU_MASK;
	if ((mpidr & ((uint64_t)MPIDR_CLUSTER_MASK)) != 0U) {
		/* A53 side				*/
		res_reg = (uintptr_t)RCAR_CA53RESCNT;
		on_reg = (uintptr_t)RCAR_CA53WUPCR;
		upper_value = 0x5A5A0000U;
	} else {
		/* A57 side				*/
		res_reg = (uintptr_t)RCAR_CA57RESCNT;
		on_reg = (uintptr_t)RCAR_CA57WUPCR;
		upper_value = 0xA5A50000U;
	}
	res_data = mmio_read_32(res_reg) | upper_value;
	SCU_power_up(mpidr);
	wup_data = (uint32_t)((uint32_t)1U << cpu_no);
	mmio_write_32(RCAR_CPGWPR, ~wup_data);
	mmio_write_32(on_reg, wup_data);
	/* Dessert to CPU reset	*/
	mmio_write_32(res_reg, (res_data & (~((uint32_t)1U << (3U - cpu_no)))));
	rcar_lock_release();
}

static void SCU_power_up(uint64_t mpidr)
{
	uint32_t reg_SYSC_bit;
	uintptr_t reg_PWRONCR;
	volatile uintptr_t reg_PWRER;
	uintptr_t reg_PWRSR;
	uintptr_t reg_CPUCMCR;
	uintptr_t reg_SYSCIER = (uintptr_t)RCAR_SYSCIER;
	uintptr_t reg_SYSCIMR = (uintptr_t)RCAR_SYSCIMR;
	volatile uintptr_t reg_SYSCSR = (volatile uintptr_t)RCAR_SYSCSR;
	volatile uintptr_t reg_SYSCISR = (volatile uintptr_t)RCAR_SYSCISR;
	volatile uintptr_t reg_SYSCISCR = (volatile uintptr_t)RCAR_SYSCISCR;

	if ((mpidr & ((uint64_t)MPIDR_CLUSTER_MASK)) == 0U) {
		/* CA57-SCU	*/
		reg_SYSC_bit = (uint32_t)BIT_CA57_SCU;
		reg_PWRONCR = (uintptr_t)RCAR_PWRONCR5;
		reg_PWRER = (volatile uintptr_t)RCAR_PWRER5;
		reg_PWRSR = (uintptr_t)RCAR_PWRSR5;
		reg_CPUCMCR = (uintptr_t)RCAR_CA57CPUCMCR;
	} else {
		/* CA53-SCU	*/
		reg_SYSC_bit = (uint32_t)BIT_CA53_SCU;
		reg_PWRONCR = (uintptr_t)RCAR_PWRONCR3;
		reg_PWRER = (volatile uintptr_t)RCAR_PWRER3;
		reg_PWRSR = (uintptr_t)RCAR_PWRSR3;
		reg_CPUCMCR = (uintptr_t)RCAR_CA53CPUCMCR;
	}
	if ((mmio_read_32(reg_PWRSR) & (uint32_t)STATUS_PWRDOWN) != 0x0000U) {
		if (mmio_read_32(reg_CPUCMCR) != 0U) {
			mmio_write_32(reg_CPUCMCR, (uint32_t)0x00000000U);
		}
		/* set SYSCIER and SYSCIMR		*/
		mmio_write_32(reg_SYSCIER,
				(mmio_read_32(reg_SYSCIER) | reg_SYSC_bit));
		mmio_write_32(reg_SYSCIMR,
				(mmio_read_32(reg_SYSCIMR) | reg_SYSC_bit));
		do {
			/* SYSCSR[1]=1?				*/
			while ((mmio_read_32(reg_SYSCSR) & (uint32_t)REQ_RESUME)
					== 0U) {
				;
			}
			/* If SYSCSR[1]=1 then set bit in PWRONCRn to 1	*/
			mmio_write_32(reg_PWRONCR, 0x0001U);
		} while ((mmio_read_32(reg_PWRER) & 0x0001U) != 0U);

		/* bit in SYSCISR=1 ?				*/
		while ((mmio_read_32(reg_SYSCISR) & reg_SYSC_bit) == 0U) {
			;
		}
		/* clear bit in SYSCISR				*/
		mmio_write_32(reg_SYSCISCR, reg_SYSC_bit);

		/* Check the SCU power-up			*/
		while ((mmio_read_32(reg_PWRSR) & (uint32_t)STATUS_PWRUP) == 0x0000U) {
			;
		}
	}
}

void rcar_pwrc_cpuoff(uint64_t mpidr)
{
	uintptr_t off_reg;
	uint64_t cpu_no;

	rcar_lock_get();
	cpu_no = mpidr & (uint64_t)MPIDR_CPU_MASK;
	if ((mpidr & ((uint64_t)MPIDR_CLUSTER_MASK)) != 0U) {
		/* A53 side				*/
		off_reg = (uintptr_t)RCAR_CA53CPU0CR;
	} else {
		/* A57 side				*/
		off_reg = (uintptr_t)RCAR_CA57CPU0CR;
	}
	if (read_mpidr_el1() == mpidr) {
		mmio_write_32(RCAR_CPGWPR, ~((uint32_t)CPU_PWR_OFF));
		mmio_write_32(off_reg + (cpu_no * 0x0010U),
				(uint32_t)CPU_PWR_OFF);
	} else {
		panic();
	}
	rcar_lock_release();
}

void rcar_pwrc_enable_interrupt_wakeup(uint64_t mpidr)
{
	uintptr_t reg;
	uint64_t cpu_no;
	uint32_t shift_irq;
	uint32_t shift_fiq;

	rcar_lock_get();
	cpu_no = mpidr & (uint64_t)MPIDR_CPU_MASK;
	if ((mpidr & ((uint64_t)MPIDR_CLUSTER_MASK)) != 0U) {
		/* A53 side				*/
		reg = (uintptr_t)RCAR_WUPMSKCA53;
	} else {
		/* A57 side				*/
		reg = (uintptr_t)RCAR_WUPMSKCA57;
	}
	shift_irq = WUP_IRQ_SHIFT + (uint32_t)cpu_no;
	shift_fiq = WUP_FIQ_SHIFT + (uint32_t)cpu_no;
	mmio_write_32(reg, (uint32_t)((~((uint32_t)1U << shift_irq))
			& (~((uint32_t)1U << shift_fiq))));
	rcar_lock_release();
}

void rcar_pwrc_disable_interrupt_wakeup(uint64_t mpidr)
{
	uintptr_t reg;
	uint64_t cpu_no;
	uint32_t shift_irq;
	uint32_t shift_fiq;

	rcar_lock_get();
	cpu_no = mpidr & (uint64_t)MPIDR_CPU_MASK;
	if ((mpidr & ((uint64_t)MPIDR_CLUSTER_MASK)) != 0U) {
		/* A53 side				*/
		reg = (uintptr_t)RCAR_WUPMSKCA53;
	} else {
		/* A57 side				*/
		reg = (uintptr_t)RCAR_WUPMSKCA57;
	}
	shift_irq = WUP_IRQ_SHIFT + (uint32_t)cpu_no;
	shift_fiq = WUP_FIQ_SHIFT + (uint32_t)cpu_no;
	mmio_write_32(reg, (uint32_t)(((uint32_t)1U << shift_irq)
			| ((uint32_t)1U << shift_fiq)));
	rcar_lock_release();
}

void rcar_pwrc_clusteroff(uint64_t mpidr)
{
}

#if !PMIC_ROHM_BD9571
void rcar_pwrc_system_reset(void)
{
	mmio_write_32(RCAR_SRESCR, (0x5AA50000U | BIT_SOFTRESET));
}
#endif /* PMIC_ROHM_BD9571 */

#define	RST_CA53CPU0BARH		(0xE6160080U)
#define	RST_CA53CPU0BARL		(0xE6160084U)

/* Nothing else to do here apart from initializing the lock */
void rcar_pwrc_setup(void)
{
	uint32_t product = mmio_read_32((uintptr_t)RCAR_PRR)
			& RCAR_PRODUCT_MASK;
	uintptr_t rst_CA5xCPUxBARH = RST_CA53CPU0BARH;
	uintptr_t rst_CA5xCPUxBARL = RST_CA53CPU0BARL;
	uint32_t loop;
	uint32_t loop_max = 6U;

	if (RCAR_PRODUCT_H3 == product) {
		loop_max = 8U;
	}

	for (loop = 0U; loop < loop_max; loop ++) {
		mmio_write_32(rst_CA5xCPUxBARH, 0U);
		mmio_write_32(rst_CA5xCPUxBARL,
				(uint32_t)((uint64_t)&bl31_secondly_reset
					& 0xFFFFFFFFU));
		rst_CA5xCPUxBARH += 0x10U;
		rst_CA5xCPUxBARL += 0x10U;
	}

	rcar_lock_init();
}

#if RCAR_SYSTEM_SUSPEND
void __attribute__ ((section (".system_ram"))) __attribute__ ((noinline)) rcar_bl31_go_suspend_to_ram(void)
{
#if PMIC_ROHM_BD9571
	uint8_t		mode;
	int32_t		ret = -1;
	uint32_t	loop;
#endif /* PMIC_ROHM_BD9571 */

	rcar_bl31_set_self_refresh();	/* Self-Refresh	*/

#if PMIC_ROHM_BD9571
	/* Set trigger of power down to PMIV		*/
	for(loop = 0U; (loop < PMIC_RETRY_MAX) && (0 != ret); loop++){
		ret = rcar_iic_dvfs_recieve(PMIC_SLAVE_ADDR,
				PMIC_BKUP_MODE_CNT, &mode);
		if (0 == ret){
			mode |= BIT_BKUP_CTRL_OUT;
			ret = rcar_iic_dvfs_send(PMIC_SLAVE_ADDR,
					PMIC_BKUP_MODE_CNT, mode);
		}
	}
#endif /* PMIC_ROHM_BD9571 */

	wfi();

	/* no return */
	while(1){}
}

static void __attribute__ ((section (".system_ram")))  rcar_bl31_set_self_refresh(void)
{
	uint32_t reg;
	uint32_t i;

	/* Set the Self-Refresh mode	*/
	mmio_write_32(DBSC4_REG_DBACEN, 0U);		/* Set the ACCEN bit to 0 in the DBACEN	*/
		/* Wait until the processing in response to the SDRAM access request in the DBSC4 is completed.	*/
	for ( i=0U; i<10000U ;i++ ){
	}

	reg = DBSC4_SET_DBCMD_OPC_PRE |
		DBSC4_SET_DBCMD_CH_ALL |
		DBSC4_SET_DBCMD_RANK_ALL |
		DBSC4_SET_DBCMD_ARG_ALL;
	mmio_write_32(DBSC4_REG_DBCMD, reg);		/* PREA command	*/
		/* Poll the operation completion waiting register (DBWAIT) to check when the issuing of manual commands is complete. */
	while (mmio_read_32(DBSC4_REG_DBWAIT) != 0U){
	}

	reg = DBSC4_SET_DBCMD_OPC_SR |
		DBSC4_SET_DBCMD_CH_ALL |
		DBSC4_SET_DBCMD_RANK_ALL |
		DBSC4_SET_DBCMD_ARG_ENTER;
	mmio_write_32(DBSC4_REG_DBCMD, reg);		/* Self-Refresh entry command	*/
		/* Poll the operation completion waiting register (DBWAIT) to check when the issuing of manual commands is complete. */
	while (mmio_read_32(DBSC4_REG_DBWAIT) != 0U){
	}

	reg = DBSC4_SET_DBCMD_OPC_PD |
		DBSC4_SET_DBCMD_CH_ALL |
		DBSC4_SET_DBCMD_RANK_ALL |
		DBSC4_SET_DBCMD_ARG_ENTER;
	mmio_write_32(DBSC4_REG_DBCMD, reg);		/* Power Down entry command	*/
		/* Poll the operation completion waiting register (DBWAIT) to check when the issuing of manual commands is complete. */
	while (mmio_read_32(DBSC4_REG_DBWAIT) != 0U){
	}

	mmio_write_32(DBSC4_REG_DBRFEN, 0U);		/* Set the ARFEN bit to 0 in the DBRFEN	*/

		/* Wait for the tCKELPD period. */
	for ( i=0U; i<10000U; i++ ){
	}
		/* DDR PHY must be entered "deep sleep" mode (details are T.B.D.). */
		/* MxBKUP is set High Level. */
		/* The power except the DDR IO are removed. */
}

void rcar_bl31_set_suspend_to_ram(void)
{
	uint32_t sctlr;

	rcar_bl31_code_copy_to_system_ram();

	rcar_bl31_save_generic_timer(rcar_stack_generic_timer);

	/* disable MMU */
	sctlr = (uint32_t)read_sctlr_el3();
	sctlr &= (uint32_t)~SCTLR_EL3_M_BIT;
	write_sctlr_el3((uint64_t)sctlr);

	(void)rcar_bl31_asm_switch_stack_pointer((uintptr_t)&rcar_bl31_go_suspend_to_ram,
			(uintptr_t)(DEVICE_SRAM_STACK_BASE\
			+ DEVICE_SRAM_STACK_SIZE_U), NULL);
}

void rcar_bl31_init_suspend_to_ram(void)
{
#if PMIC_ROHM_BD9571
	uint8_t		mode;
#endif /* PMIC_ROHM_BD9571 */

	rcar_bl31_code_copy_to_system_ram();

#if PMIC_ROHM_BD9571
	if (rcar_iic_dvfs_recieve(PMIC_SLAVE_ADDR, PMIC_BKUP_MODE_CNT, &mode) != 0){
		panic();
	}
	mode &= (uint8_t)(~BIT_BKUP_CTRL_OUT);
	if (rcar_iic_dvfs_send(PMIC_SLAVE_ADDR, PMIC_BKUP_MODE_CNT, mode) != 0){
		panic();
	}
#endif /* PMIC_ROHM_BD9571 */
}
#endif /* RCAR_SYSTEM_SUSPEND */


void rcar_bl31_code_copy_to_system_ram(void)
{
	(void)memcpy((void *)DEVICE_SRAM_SHADOW_BASE, &__SRAM_COPY_START__,
			(size_t)((uint64_t)__system_ram_end__ - (uint64_t)__system_ram_start__));

	flush_dcache_range((uint64_t)DEVICE_SRAM_SHADOW_BASE,
			((uint64_t)__system_ram_end__ - (uint64_t)__system_ram_start__));

	/* Invalidate instruction cache */
	iciallu();
	dsb();
	isb();
}

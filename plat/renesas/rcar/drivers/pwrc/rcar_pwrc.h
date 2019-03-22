/*
 * Copyright (c) 2013-2014, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2015-2019, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef RCAR_PWRC_H__
#define RCAR_PWRC_H__

/* RCAR Power controller register offset etc */
#define PPOFFR_OFF		0x0
#define PPONR_OFF		0x4
#define PCOFFR_OFF		0x8
#define PWKUPR_OFF		0xc
#define PSYSR_OFF		0x10

#define PWKUPR_WEN		(1ULL << 31)

#define PSYSR_AFF_L2		(1 << 31)
#define PSYSR_AFF_L1		(1 << 30)
#define PSYSR_AFF_L0		(1 << 29)
#define PSYSR_WEN		(1 << 28)
#define PSYSR_PC		(1 << 27)
#define PSYSR_PP		(1 << 26)

#define PSYSR_WK_SHIFT		(24)
#define PSYSR_WK_MASK		(0x3)
#define PSYSR_WK(x)		(((x) >> PSYSR_WK_SHIFT) & PSYSR_WK_MASK)

#define WKUP_COLD		0x0
#define WKUP_RESET		0x1
#define WKUP_PPONR		0x2
#define WKUP_GICREQ		0x3

#define	RCAR_INVALID		(0xffffffffU)
#define PSYSR_INVALID		0xffffffff

#define	RCAR_CLUSTER_A53A57	(0U)
#define	RCAR_CLUSTER_CA53	(1U)
#define	RCAR_CLUSTER_CA57	(2U)


#define	RCAR_DYNAMIC_REGION_NOT_EXIST	(0U)
#define	RCAR_DYNAMIC_REGION_EXIST	(1U)


#ifndef __ASSEMBLY__

/*******************************************************************************
 * Function & variable prototypes
 ******************************************************************************/
void rcar_pwrc_setup(void);
void rcar_pwrc_clusteroff(uint64_t mpidr);
void rcar_pwrc_cpuoff(uint64_t mpidr);
void rcar_pwrc_cpuon(uint64_t mpidr);
void rcar_pwrc_enable_interrupt_wakeup(uint64_t mpidr);
void rcar_pwrc_disable_interrupt_wakeup(uint64_t mpidr);
uint32_t rcar_pwrc_status(uint64_t mpidr);
uint32_t rcar_pwrc_get_cpu_wkr(uint64_t mpidr);
#if !PMIC_ROHM_BD9571
void rcar_pwrc_system_reset(void);
#endif /* PMIC_ROHM_BD9571 */
void bl31_secondly_reset(void);

#if RCAR_SYSTEM_SUSPEND
/* for Suspend to RAM */
void rcar_bl31_go_suspend_to_ram(void);
void rcar_bl31_set_suspend_to_ram(void);
void rcar_bl31_init_suspend_to_ram(void);
void rcar_bl31_suspend_to_ram(void);
#endif /* RCAR_SYSTEM_SUSPEND */
void rcar_bl31_code_copy_to_system_ram(uint32_t type);
uint32_t rcar_bl31_get_cluster(void);
uint32_t rcar_bl31_get_mpidr_cluster(uint64_t mpidr);
uint32_t rcar_bl31_get_cpu_num(uint32_t cluster_type);
void rcar_bl31_restore_timer_state(void);

extern uint32_t rcar_bl31_asm_switch_stack_pointer(uintptr_t jump, uintptr_t stack,
                void *arg);

extern uint8_t __system_ram_start__[1];
extern uint8_t __system_ram_end__[1];
extern uint8_t __SRAM_COPY_START__[1];

#endif /*__ASSEMBLY__*/

#endif /* RCAR_PWRC_H__ */

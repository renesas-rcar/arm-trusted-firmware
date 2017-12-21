/*
 * Copyright (c) 2013-2014, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2015-2017, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <debug.h>
#include <platform_def.h>
/* TODO: Reusing psci error codes & state information. Get our own! */
#include <psci.h>
#include "drivers/pwrc/rcar_pwrc.h"

/* We treat '255' as an invalid affinity instance */
#define AFFINST_INVAL	0xff

typedef struct affinity_info {
	unsigned char sibling;
	unsigned char child;
	unsigned char state;
	unsigned int data;
} affinity_info_t;

/*******************************************************************************
 * The following two data structures store the topology tree for the rcar. There
 * is a separate array for each affinity level i.e. cpus and clusters. The child
 * and sibling references allow traversal inside and in between the two arrays.
 ******************************************************************************/
static affinity_info_t rcar_aff1_topology_map[PLATFORM_CLUSTER_COUNT];
static affinity_info_t rcar_aff0_topology_map[PLATFORM_CORE_COUNT];


/*******************************************************************************
 * This function implements a part of the critical interface between the psci
 * generic layer and the platform to allow the former to detect the platform
 * topology. psci queries the platform to determine how many affinity instances
 * are present at a particular level for a given mpidr e.g. consider a dual
 * cluster platform where each cluster has 4 cpus. A call to this function with
 * (0, 0x100) will return the number of cpus implemented under cluster 1 i.e. 4.
 * Similarly a call with (1, 0x100) will return 2 i.e. the number of clusters.
 * This is 'cause we are effectively asking how many affinity level 1 instances
 * are implemented under affinity level 2 instance 0.
 ******************************************************************************/
unsigned int plat_get_aff_count(unsigned int aff_lvl,
				unsigned long mpidr)
{
	unsigned int aff_count = 1, ctr;
	unsigned char parent_aff_id;

	switch (aff_lvl) {
	case 3:
	case 2:
		/*
		 * Report that we implement a single instance of
		 * affinity levels 2 & 3 which are AFF_ABSENT
		 */
		break;
	case 1:
		/* Fetch the starting index in the aff1 array */
		for (ctr = 0;
		     rcar_aff1_topology_map[ctr].sibling != AFFINST_INVAL;
		     ctr = rcar_aff1_topology_map[ctr].sibling) {
			aff_count++;
		}

		break;
	case 0:
		/* Panic if the cluster id is anything apart from 0 or 1 */
		parent_aff_id = (mpidr >> MPIDR_AFF1_SHIFT) & MPIDR_AFFLVL_MASK;
		if (PLATFORM_CLUSTER_COUNT <= parent_aff_id) {
			panic();
		}

		/* Fetch the starting index in the aff0 array */
		for (ctr = rcar_aff1_topology_map[parent_aff_id].child;
		     rcar_aff0_topology_map[ctr].sibling != AFFINST_INVAL;
		     ctr = rcar_aff0_topology_map[ctr].sibling) {
			aff_count++;
		}

		break;
	default:
		panic();
		break;
	}

	return aff_count;
}

/*******************************************************************************
 * This function implements a part of the critical interface between the psci
 * generic layer and the platform to allow the former to detect the state of a
 * affinity instance in the platform topology. psci queries the platform to
 * determine whether an affinity instance is present or absent. This caters for
 * topologies where an intermediate affinity level instance is missing e.g.
 * consider a platform which implements a single cluster with 4 cpus and there
 * is another cpu sitting directly on the interconnect along with the cluster.
 * The mpidrs of the cluster would range from 0x0-0x3. The mpidr of the single
 * cpu would be 0x100 to highlight that it does not belong to cluster 0. Cluster
 * 1 is however missing but needs to be accounted to reach this single cpu in
 * the topology tree. Hence it will be marked as PSCI_AFF_ABSENT. This is not
 * applicable to the RCAR but depicted as an example.
 ******************************************************************************/
unsigned int plat_get_aff_state(unsigned int aff_lvl,
				unsigned long mpidr)
{
	unsigned int aff_state = PSCI_AFF_ABSENT, idx;
	idx = (mpidr >> MPIDR_AFF1_SHIFT) & MPIDR_AFFLVL_MASK;

	switch (aff_lvl) {
	case 3:
	case 2:
		/* Report affinity levels 2 & 3 as absent */
		break;
	case 1:
		aff_state = rcar_aff1_topology_map[idx].state;
		break;
	case 0:
		/*
		 * First get start index of the aff0 in its array & then add
		 * to it the affinity id that we want the state of
		 */
		idx = rcar_aff1_topology_map[idx].child;
		idx += (mpidr >> MPIDR_AFF0_SHIFT) & MPIDR_AFFLVL_MASK;
		aff_state = rcar_aff0_topology_map[idx].state;
		break;
	default:
		panic();
		break;
	}

	return aff_state;
}

/*******************************************************************************
 * This function populates the RCAR specific topology information depending upon
 * the RCAR flavour its running on. We construct all the mpidrs we can handle
 * and rely on the PRR to flag absent cpus when their status is queried.
 ******************************************************************************/
void rcar_setup_topology(void)
{
	unsigned char aff0, aff1, aff_state, aff0_offset = 0;
	unsigned long mpidr;

	for (aff1 = 0; aff1 < PLATFORM_CLUSTER_COUNT; aff1++) {

		rcar_aff1_topology_map[aff1].child = aff0_offset;
		rcar_aff1_topology_map[aff1].sibling = aff1 + 1;

		for (aff0 = 0; aff0 < PLATFORM_MAX_CPUS_PER_CLUSTER; aff0++) {

			mpidr = aff1 << MPIDR_AFF1_SHIFT;
			mpidr |= aff0 << MPIDR_AFF0_SHIFT;

			if (rcar_pwrc_status(mpidr) != RCAR_INVALID) {
				/*
				 * Presence of even a single aff0 indicates
				 * presence of parent aff1 on the RCAR.
				 */
				aff_state = PSCI_AFF_PRESENT;
				rcar_aff1_topology_map[aff1].state =
					PSCI_AFF_PRESENT;
			} else {
				aff_state = PSCI_AFF_ABSENT;
			}

			rcar_aff0_topology_map[aff0_offset].child = AFFINST_INVAL;
			rcar_aff0_topology_map[aff0_offset].state = aff_state;
			rcar_aff0_topology_map[aff0_offset].sibling =
				aff0_offset + 1;

			/* Increment the absolute number of aff0s traversed */
			aff0_offset++;
		}

		/* Tie-off the last aff0 sibling to -1 to avoid overflow */
		rcar_aff0_topology_map[aff0_offset - 1].sibling = AFFINST_INVAL;
	}

	/* Tie-off the last aff1 sibling to AFFINST_INVAL to avoid overflow */
	rcar_aff1_topology_map[aff1 - 1].sibling = AFFINST_INVAL;

}

/*
 * Copyright (c) 2014, Mentor Graphics Corporation
 * All rights reserved.
 * Copyright (c) 2015 Xilinx, Inc. All rights reserved.
  *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
  *
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
 * 3. Neither the name of Mentor Graphics Corporation nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
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

/* This file populates resource table for BM remote
 * for use by the Linux Master */

#ifndef RSC_TABLE_H_
#define RSC_TABLE_H_

#include "openamp/open_amp.h"
#include "openamp_conf.h"

/* Place resource table in special ELF section */

/* Resource table for the given remote */
struct shared_resource_table {
	unsigned int version;
	unsigned int num;
	unsigned int reserved[2];
	unsigned int offset[NUM_RESOURCE_ENTRIES];
	/* text carveout entry */

	/* rpmsg vdev entry */
	struct fw_rsc_vdev vdev;
	struct fw_rsc_vdev_vring vring0;
	struct fw_rsc_vdev_vring vring1;
	struct fw_rsc_trace cm_trace;
};

void resource_table_init(int RPMsgRole, void **table_ptr, int *length);

#endif /* RSC_TABLE_H_ */


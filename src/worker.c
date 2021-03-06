/*
 * Copyright (c) 2017 Breno Leitao, Gustavo Romero, IBM Corp.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#include "torture.h"
#include<htmintrin.h>

void *worker(void *arg)
{
	// I know you will frown upon void * to int cast ;-)
	uint64_t workload = (int) arg;
	int r;

	uint64_t nr;
	uint64_t res;
	// VSX registers. To be used in the future.
	register vector __int128 vsx0 asm ("vs0") __attribute__((unused));
	register vector __int128 vsx1 asm ("vs1") __attribute__((unused));

	// VMX registers.
	register vector __int128 vmx0  asm ("vs32") __attribute__((unused));
	register vector __int128 vmx1  asm ("vs33") __attribute__((unused));
	register vector __int128 vmx2  asm ("vs34") __attribute__((unused));
	register vector __int128 vmx3  asm ("vs35") __attribute__((unused));
	register vector __int128 vmx4  asm ("vs36") __attribute__((unused));
	register vector __int128 vmx5  asm ("vs37") __attribute__((unused));
	register vector __int128 vmx6  asm ("vs38") __attribute__((unused));
	register vector __int128 vmx7  asm ("vs39") __attribute__((unused));
	register vector __int128 vmx8  asm ("vs40") __attribute__((unused));
	register vector __int128 vmx9  asm ("vs41") __attribute__((unused));
	register vector __int128 vmx10 asm ("vs42") __attribute__((unused));
	register vector __int128 vmx11 asm ("vs43") __attribute__((unused));
	register vector __int128 vmx12 asm ("vs44") __attribute__((unused));
	register vector __int128 vmx13 asm ("vs45") __attribute__((unused));
	register vector __int128 vmx14 asm ("vs46") __attribute__((unused));
	register vector __int128 vmx15 asm ("vs47") __attribute__((unused));
	register vector __int128 vmx16 asm ("vs48") __attribute__((unused));
	register vector __int128 vmx17 asm ("vs49") __attribute__((unused));
	register vector __int128 vmx18 asm ("vs50") __attribute__((unused));
	register vector __int128 vmx19 asm ("vs51") __attribute__((unused));
	register vector __int128 vmx20 asm ("vs52") __attribute__((unused));
	register vector __int128 vmx21 asm ("vs53") __attribute__((unused));
	register vector __int128 vmx22 asm ("vs54") __attribute__((unused));
	register vector __int128 vmx23 asm ("vs55") __attribute__((unused));
	register vector __int128 vmx24 asm ("vs56") __attribute__((unused));
	register vector __int128 vmx25 asm ("vs57") __attribute__((unused));
	register vector __int128 vmx26 asm ("vs58") __attribute__((unused));
	register vector __int128 vmx27 asm ("vs59") __attribute__((unused));
	register vector __int128 vmx28 asm ("vs60") __attribute__((unused));
	register vector __int128 vmx29 asm ("vs61") __attribute__((unused));
	register vector __int128 vmx30 asm ("vs62") __attribute__((unused));
	register vector __int128 vmx31 asm ("vs63") __attribute__((unused));

	vector __int128 vmx_correct_value[32];
	vector __int128 vmx_scratch_area[2];

	uint64_t offset = 0;
	uint64_t z;

	if (debug)
		printf("Thread executing workload %s: ", (char *) wname[workload]);

	// Expected correct values restored after a HTM failure.
	vmx_correct_value[0]  = (vector __int128) {0xBEEF};
	vmx_correct_value[1]  = (vector __int128) {0xBEEF};
	vmx_correct_value[2]  = (vector __int128) {0xBEEF};
	vmx_correct_value[3]  = (vector __int128) {0xBEEF};
	vmx_correct_value[4]  = (vector __int128) {0xBEEF};
	vmx_correct_value[5]  = (vector __int128) {0xBEEF};
	vmx_correct_value[6]  = (vector __int128) {0xBEEF};
	vmx_correct_value[7]  = (vector __int128) {0xBEEF};
	vmx_correct_value[8]  = (vector __int128) {0xBEEF};
	vmx_correct_value[9]  = (vector __int128) {0xBEEF};
	vmx_correct_value[10] = (vector __int128) {0xBEEF};
	vmx_correct_value[11] = (vector __int128) {0xBEEF};
	vmx_correct_value[12] = (vector __int128) {0xBEEF};
	vmx_correct_value[13] = (vector __int128) {0xBEEF};
	vmx_correct_value[14] = (vector __int128) {0xBEEF};
	vmx_correct_value[15] = (vector __int128) {0xBEEF};
	vmx_correct_value[16] = (vector __int128) {0xBEEF};
	vmx_correct_value[17] = (vector __int128) {0xBEEF};
	vmx_correct_value[18] = (vector __int128) {0xBEEF};
	vmx_correct_value[19] = (vector __int128) {0xBEEF};
	vmx_correct_value[20] = (vector __int128) {0xBEEF};
	vmx_correct_value[21] = (vector __int128) {0xBEEF};
	vmx_correct_value[22] = (vector __int128) {0xBEEF};
	vmx_correct_value[23] = (vector __int128) {0xBEEF};
	vmx_correct_value[24] = (vector __int128) {0xBEEF};
	vmx_correct_value[25] = (vector __int128) {0xBEEF};
	vmx_correct_value[26] = (vector __int128) {0xBEEF};
	vmx_correct_value[27] = (vector __int128) {0xBEEF};
	vmx_correct_value[28] = (vector __int128) {0xBEEF};
	vmx_correct_value[29] = (vector __int128) {0xBEEF};
	vmx_correct_value[30] = (vector __int128) {0xBEEF};
	vmx_correct_value[31] = (vector __int128) {0xBEEF};

	// Expected correct value restored to VRSAVE after a HTM failure.
	// A place to keep the correct value in memory
	uint64_t vrsave_correct_value[1];

	// The correct value itself, a 32-bit value
	vrsave_correct_value[0] = 0xBABEBEEF;

	// Set values for each VMX register just before entering in a
	// transactional block.  Values below must match the expected correct
	// values after a HTM failure, as specified in the code section above.

	// TODO: add random generated value as the expected correct values to
	// torture kernel VMX restore code.

	vmx0  = (vector __int128) {0xBEEF};
	vmx1  = (vector __int128) {0xBEEF};
	vmx2  = (vector __int128) {0xBEEF};
	vmx3  = (vector __int128) {0xBEEF};
	vmx4  = (vector __int128) {0xBEEF};
	vmx5  = (vector __int128) {0xBEEF};
	vmx6  = (vector __int128) {0xBEEF};
	vmx7  = (vector __int128) {0xBEEF};
	vmx8  = (vector __int128) {0xBEEF};
	vmx9  = (vector __int128) {0xBEEF};
	vmx10 = (vector __int128) {0xBEEF};
	vmx11 = (vector __int128) {0xBEEF};
	vmx12 = (vector __int128) {0xBEEF};
	vmx13 = (vector __int128) {0xBEEF};
	vmx14 = (vector __int128) {0xBEEF};
	vmx15 = (vector __int128) {0xBEEF};
	vmx16 = (vector __int128) {0xBEEF};
	vmx17 = (vector __int128) {0xBEEF};
	vmx18 = (vector __int128) {0xBEEF};
	vmx19 = (vector __int128) {0xBEEF};
	vmx20 = (vector __int128) {0xBEEF};
	vmx21 = (vector __int128) {0xBEEF};
	vmx22 = (vector __int128) {0xBEEF};
	vmx23 = (vector __int128) {0xBEEF};
	vmx24 = (vector __int128) {0xBEEF};
	vmx25 = (vector __int128) {0xBEEF};
	vmx26 = (vector __int128) {0xBEEF};
	vmx27 = (vector __int128) {0xBEEF};
	vmx28 = (vector __int128) {0xBEEF};
	vmx29 = (vector __int128) {0xBEEF};
	vmx30 = (vector __int128) {0xBEEF};
	vmx31 = (vector __int128) {0xBEEF};

	if (susp == SUSP_RANDOM)
		r = rand() % 2;
	else if (susp == SUSP_ALWAYS) {
		r = 1;
	}
	else {
		r = 0;
	}


	// Set value for VRSAVE register just before entering in HTM block.
	_ ("lwz 5, 0(%[vrsave_correct_value]) \n\t"
	   "mtvrsave 5                        \n\t"
	   :
	   : [vrsave_correct_value] "r"(vrsave_correct_value)
	   : "r5"
	  );

	/***************
	 ** HTM BEGIN **
	****************/
	_ ("tbegin.  \n\t");
	_ goto ("beq %l[_failure] \n\t" : : : : _failure);

	/* Suspend half of the transactions */
	if (r == 1)
		_ ("tsuspend.	\n\t");
	/**************
	 ** HTM BODY **
	 **************/

	// Mess with the VRSAVE using a random value taken from Time Base
	// register.
	_ ("mftb     5    \n\t"
	   "mtvrsave 5    \n\t"
	   :
	   :
	   : "r5"
	  );

	// We set VMX register with a value different from what they
	// were set *before* entering the HTM. If the transaction
	// fails, then VMX registers after _failure is taken must
	// be restored back to the values *before* entering the HTM.
	vmx0  = (vector __int128) {0xBABE};
	vmx1  = (vector __int128) {0xBABE};
	vmx2  = (vector __int128) {0xBABE};
	vmx3  = (vector __int128) {0xBABE};
	vmx4  = (vector __int128) {0xBABE};
	vmx5  = (vector __int128) {0xBABE};
	vmx6  = (vector __int128) {0xBABE};
	vmx7  = (vector __int128) {0xBABE};
	vmx8  = (vector __int128) {0xBABE};
	vmx9  = (vector __int128) {0xBABE};
	vmx10 = (vector __int128) {0xBABE};
	vmx11 = (vector __int128) {0xBABE};
	vmx12 = (vector __int128) {0xBABE};
	vmx13 = (vector __int128) {0xBABE};
	vmx14 = (vector __int128) {0xBABE};
	vmx15 = (vector __int128) {0xBABE};
	vmx16 = (vector __int128) {0xBABE};
	vmx17 = (vector __int128) {0xBABE};
	vmx18 = (vector __int128) {0xBABE};
	vmx19 = (vector __int128) {0xBABE};
	vmx20 = (vector __int128) {0xBABE};
	vmx21 = (vector __int128) {0xBABE};
	vmx22 = (vector __int128) {0xBABE};
	vmx23 = (vector __int128) {0xBABE};
	vmx24 = (vector __int128) {0xBABE};
	vmx25 = (vector __int128) {0xBABE};
	vmx26 = (vector __int128) {0xBABE};
	vmx27 = (vector __int128) {0xBABE};
	vmx28 = (vector __int128) {0xBABE};
	vmx29 = (vector __int128) {0xBABE};
	vmx30 = (vector __int128) {0xBABE};
	vmx31 = (vector __int128) {0xBABE};

	// Call workload.
	if (workloads[workload] == NULL) {
		printf("ERROR: Workload %ld is NULL. Aborting\n", workload);
		exit(-200);
	} else
		(*workloads[workload])();

	/*************
	 ** HTM END **
	 *************/
	_ ("tend.    \n\t");
	_ goto ("b %l[_success] \n\t" : : : : _success);


_failure:
	_ goto (

	// Check if VRSAVE is sane.
	"lwz      5, 0(%[vrsave_correct_value]) \n\t"
	"mfvrsave 6                             \n\t"
	"cmpd     5, 6                          \n\t"
	"li       6, 32                         \n\t"
	"bne  %l[_value_mismatch]               \n\t"

	// Check if vmx0 is sane.
	"stvx 1, 0, %[vmx_scratch_area]   \n\t"
	"lvx  1, 0, %[vmx_correct_value]  \n\t"
	"vcmpequb. 0, 0, 1                \n\t"
	"lvx  1, 0,  %[vmx_scratch_area]  \n\t"
	"mfvrd 5, 0                       \n\t"
	"li     6, 0                      \n\t"
	"bc   4, 24, %l[_value_mismatch]  \n\t"

	// Check if vmx1 is sane.
	"addi %[offset], %[offset], 16    \n\t"
	"lvx  0, %[offset], %[vmx_correct_value]  \n\t"
	"vcmpequb. 0, 0, 1                \n\t"
	"mfvrd 5, 1                       \n\t"
	"li     6, 1                      \n\t"
	"bc 4, 24, %l[_value_mismatch]    \n\t"

	// Check if vmx2 is sane.
	"addi %[offset], %[offset], 16    \n\t"
	"lvx  0, %[offset], %[vmx_correct_value]  \n\t"
	"vcmpequb. 0, 0, 2                \n\t"
	"mfvrd 5, 2 \n\t"
	"li     6, 2 \n\t"
	"bc 4, 24, %l[_value_mismatch]    \n\t"

	// Check if vmx3 is sane.
	"addi %[offset], %[offset], 16    \n\t"
	"lvx  0, %[offset], %[vmx_correct_value] \n\t"
	"vcmpequb. 0, 0, 3                \n\t"
	"mfvrd 5, 3 \n\t"
	"li     6, 3 \n\t"
	"bc 4, 24, %l[_value_mismatch]    \n\t"

	// Check if vmx4 is sane.
	"addi %[offset], %[offset], 16    \n\t"
	"lvx  0, %[offset], %[vmx_correct_value] \n\t"
	"vcmpequb. 0, 0, 4                \n\t"
	"mfvrd 5, 4 \n\t"
	"li     6, 4 \n\t"
	"bc 4, 24, %l[_value_mismatch]    \n\t"

	// Check if vmx5 is sane.
	"addi %[offset], %[offset], 16    \n\t"
	"lvx  0, %[offset], %[vmx_correct_value] \n\t"
	"vcmpequb. 0, 0, 5                \n\t"
	"mfvrd 5, 5 \n\t"
	"li     6, 5 \n\t"
	"bc 4, 24, %l[_value_mismatch]    \n\t"

	// Check if vmx6 is sane.
	"addi %[offset], %[offset], 16    \n\t"
	"lvx  0, %[offset], %[vmx_correct_value] \n\t"
	"vcmpequb. 0, 0, 6                \n\t"
	"mfvrd 5, 6 \n\t"
	"li     6, 6 \n\t"
	"bc 4, 24, %l[_value_mismatch]    \n\t"

	// Check if vmx7 is sane.
	"addi %[offset], %[offset], 16    \n\t"
	"lvx  0, %[offset], %[vmx_correct_value] \n\t"
	"vcmpequb. 0, 0, 7                \n\t"
	"mfvrd 5, 7 \n\t"
	"li     6, 7 \n\t"
	"bc 4, 24, %l[_value_mismatch]    \n\t"

	// Check if vmx8 is sane.
	"addi %[offset], %[offset], 16    \n\t"
	"lvx  0, %[offset], %[vmx_correct_value] \n\t"
	"vcmpequb. 0, 0, 8                \n\t"
	"mfvrd 5, 8 \n\t"
	"li     6, 8 \n\t"
	"bc 4, 24, %l[_value_mismatch]    \n\t"

	// Check if vmx9 is sane.
	"addi %[offset], %[offset], 16    \n\t"
	"lvx  0, %[offset], %[vmx_correct_value] \n\t"
	"vcmpequb. 0, 0, 9                \n\t"
	"mfvrd 5, 9 \n\t"
	"li     6, 9 \n\t"
	"bc 4, 24, %l[_value_mismatch]    \n\t"

	// Check if vmx10 is sane.
	"addi %[offset], %[offset], 16    \n\t"
	"lvx  0, %[offset], %[vmx_correct_value] \n\t"
	"vcmpequb. 0, 0, 10               \n\t"
	"mfvrd 5, 10 \n\t"
	"li     6, 10 \n\t"
	"bc 4, 24, %l[_value_mismatch]    \n\t"

	// Check if vmx11 is sane.
	"addi %[offset], %[offset], 16    \n\t"
	"lvx  0, %[offset], %[vmx_correct_value] \n\t"
	"vcmpequb. 0, 0, 11               \n\t"
	"mfvrd 5, 11 \n\t"
	"li     6, 11 \n\t"
	"bc 4, 24, %l[_value_mismatch]    \n\t"

	// Check if vmx12 is sane.
	"addi %[offset], %[offset], 16    \n\t"
	"lvx  0, %[offset], %[vmx_correct_value] \n\t"
	"vcmpequb. 0, 0, 12                \n\t"
	"mfvrd 5, 12 \n\t"
	"li     6, 12 \n\t"
	"bc 4, 24, %l[_value_mismatch]    \n\t"

	// Check if vmx13 is sane.
	"addi %[offset], %[offset], 16    \n\t"
	"lvx  0, %[offset], %[vmx_correct_value] \n\t"
	"vcmpequb. 0, 0, 13               \n\t"
	"mfvrd 5, 13 \n\t"
	"li     6, 13 \n\t"
	"bc 4, 24, %l[_value_mismatch]    \n\t"

	// Check if vmx14 is sane.
	"addi %[offset], %[offset], 16    \n\t"
	"lvx  0, %[offset], %[vmx_correct_value] \n\t"
	"vcmpequb. 0, 0, 14               \n\t"
	"mfvrd 5, 14 \n\t"
	"li     6, 14 \n\t"
	"bc 4, 24, %l[_value_mismatch]    \n\t"

	// Check if vmx15 is sane.
	"addi %[offset], %[offset], 16    \n\t"
	"lvx  0, %[offset], %[vmx_correct_value] \n\t"
	"vcmpequb. 0, 0, 15               \n\t"
	"mfvrd 5, 15 \n\t"
	"li     6, 15 \n\t"
	"bc 4, 24, %l[_value_mismatch]    \n\t"

	// Check if vmx16 is sane.
	"addi %[offset], %[offset], 16    \n\t"
	"lvx  0, %[offset], %[vmx_correct_value] \n\t"
	"vcmpequb. 0, 0, 16               \n\t"
	"mfvrd 5, 16 \n\t"
	"li     6, 16 \n\t"
	"bc 4, 24, %l[_value_mismatch]    \n\t"

	// Check if vmx17 is sane.
	"addi %[offset], %[offset], 16    \n\t"
	"lvx  0, %[offset], %[vmx_correct_value] \n\t"
	"vcmpequb. 0, 0, 17               \n\t"
	"mfvrd 5, 17 \n\t"
	"li     6, 17 \n\t"
	"bc 4, 24, %l[_value_mismatch]    \n\t"

	// Check if vmx18 is sane.
	"addi %[offset], %[offset], 16    \n\t"
	"lvx  0, %[offset], %[vmx_correct_value] \n\t"
	"vcmpequb. 0, 0, 18               \n\t"
	"mfvrd 5, 18 \n\t"
	"li     6, 18 \n\t"
	"bc 4, 24, %l[_value_mismatch]    \n\t"

	// Check if vmx19 is sane.
	"addi %[offset], %[offset], 16    \n\t"
	"lvx  0, %[offset], %[vmx_correct_value] \n\t"
	"vcmpequb. 0, 0, 19               \n\t"
	"mfvrd 5, 19 \n\t"
	"li     6, 19 \n\t"
	"bc 4, 24, %l[_value_mismatch]    \n\t"

	// Check if vmx20 is sane.
	"addi %[offset], %[offset], 16    \n\t"
	"lvx  0, %[offset], %[vmx_correct_value] \n\t"
	"vcmpequb. 0, 0, 20               \n\t"
	"mfvrd 5, 20 \n\t"
	"li     6, 20 \n\t"
	"bc 4, 24, %l[_value_mismatch]    \n\t"

	// Check if vmx21 is sane.
	"addi %[offset], %[offset], 16    \n\t"
	"lvx  0, %[offset], %[vmx_correct_value] \n\t"
	"vcmpequb. 0, 0, 21               \n\t"
	"mfvrd 5, 21 \n\t"
	"li     6, 21 \n\t"
	"bc 4, 24, %l[_value_mismatch]    \n\t"

	// Check if vmx22 is sane.
	"addi %[offset], %[offset], 16    \n\t"
	"lvx  0, %[offset], %[vmx_correct_value] \n\t"
	"vcmpequb. 0, 0, 22               \n\t"
	"mfvrd 5, 22 \n\t"
	"li     6, 22 \n\t"
	"bc 4, 24, %l[_value_mismatch]    \n\t"

	// Check if vmx23 is sane.
	"addi %[offset], %[offset], 16    \n\t"
	"lvx  0, %[offset], %[vmx_correct_value] \n\t"
	"vcmpequb. 0, 0, 23               \n\t"
	"mfvrd 5, 23 \n\t"
	"li     6, 23 \n\t"
	"bc 4, 24, %l[_value_mismatch]    \n\t"

	// Check if vmx24 is sane.
	"addi %[offset], %[offset], 16    \n\t"
	"lvx  0, %[offset], %[vmx_correct_value] \n\t"
	"vcmpequb. 0, 0, 24               \n\t"
	"mfvrd 5, 24 \n\t"
	"li     6, 24 \n\t"
	"bc 4, 24, %l[_value_mismatch]    \n\t"

	// Check if vmx25 is sane.
	"addi %[offset], %[offset], 16    \n\t"
	"lvx  0, %[offset], %[vmx_correct_value] \n\t"
	"vcmpequb. 0, 0, 25               \n\t"
	"mfvrd 5, 25 \n\t"
	"li     6, 25 \n\t"
	"bc 4, 24, %l[_value_mismatch]    \n\t"

	// Check if vmx26 is sane.
	"addi %[offset], %[offset], 16    \n\t"
	"lvx  0, %[offset], %[vmx_correct_value] \n\t"
	"vcmpequb. 0, 0, 26               \n\t"
	"mfvrd 5, 26 \n\t"
	"li     6, 26 \n\t"
	"bc 4, 24, %l[_value_mismatch]    \n\t"

	// Check if vmx27 is sane.
	"addi %[offset], %[offset], 16    \n\t"
	"lvx  0, %[offset], %[vmx_correct_value] \n\t"
	"vcmpequb. 0, 0, 27                \n\t"
	"mfvrd 5, 27 \n\t"
	"li     6, 27 \n\t"
	"bc 4, 24, %l[_value_mismatch]    \n\t"

	// Check if vmx28 is sane.
	"addi %[offset], %[offset], 16    \n\t"
	"lvx  0, %[offset], %[vmx_correct_value] \n\t"
	"vcmpequb. 0, 0, 28               \n\t"
	"mfvrd 5, 28 \n\t"
	"li     6, 28 \n\t"
	"bc 4, 24, %l[_value_mismatch]    \n\t"

	// Check if vmx29 is sane.
	"addi %[offset], %[offset], 16    \n\t"
	"lvx  0, %[offset], %[vmx_correct_value] \n\t"
	"vcmpequb. 0, 0, 29               \n\t"
	"mfvrd 5, 29 \n\t"
	"li     6, 29 \n\t"
	"bc 4, 24, %l[_value_mismatch]    \n\t"

	// Check if vmx30 is sane.
	"addi %[offset], %[offset], 16    \n\t"
	"lvx  0, %[offset], %[vmx_correct_value] \n\t"
	"vcmpequb. 0, 0, 30               \n\t"
	"mfvrd 5, 30 \n\t"
	"li     6, 30 \n\t"
	"bc 4, 24, %l[_value_mismatch]    \n\t"

	// Check if vmx31 is sane.
	"addi %[offset], %[offset], 16    \n\t"
	"lvx  0, %[offset], %[vmx_correct_value] \n\t"
	"vcmpequb. 0, 0, 31               \n\t"
	"mfvrd 5, 31 \n\t"
	"li     6, 31 \n\t"
	"bc 4, 24, %l[_value_mismatch]    \n\t"

	// Reach here, then all registers are sane.
	"b  %l[_value_match]              \n\t"

	   : // no output
	   : [offset] "r"(offset),
	     [vmx_correct_value] "r"(vmx_correct_value),
	     [vmx_scratch_area]  "r"(vmx_scratch_area),
	     [vrsave_correct_value] "r"(vrsave_correct_value)
	   : "memory", "r5", "r6"
	   : _value_match, _value_mismatch
	);

_value_mismatch:
	z = __builtin_get_texasr();
	printf("\n\n==============\n\n");
	printf("\nFailure with error: %lx\n\n",  _TEXASR_FAILURE_CODE(z));
	printf(": Summary error: %lx\n",  _TEXASR_FAILURE_SUMMARY(z));
	printf(": TFIAR error: %lx\n", _TEXASR_TFIAR_EXACT(z));
	printf(": TEXASR: %.16lx\n", z);
	printf(": Workload: %ld\n\n", workload);

       _ (".long 0"); // exit with a core dump
	// TODO: move 'nr' and 'res' to input list in inline asm above and
	// remove from here.
	_ ("mr %[nr], 6 \n\t": [nr] "=r"(nr) : :);
	_ ("mr %[res], 5 \n\t": [res] "=r"(res) : :);

       if (nr == 32) { // VRSAVE got corrupted
		printf("Wordload %ld: register VRSAVE got corrupted.\n", workload);
		printf("HTM failed and VRSAVE register got corrupted!\n");
		exit(13);
       } else {       // VSX/VMX got corrupted
		printf("Workload %ld: register vs%"PRIu64 " contains value 0x%"PRIx64"\n", workload, nr+32, res);
		printf("HTM failed and VMX registers got corrupted!\n");
		exit(14);
       }

_value_match:
	printf("!");
	tm_fails++;
	if (debug)
		printf(" HTM failed but VMX and VRSAVE registers are OK\n");
	goto _finish;

_success:
	printf(".");
	tm_commit++;
	if (debug)
		printf(" HTM succeeded\n");
_finish:
	_ ("nop"); // Can't label in the void...


	return 0;
}

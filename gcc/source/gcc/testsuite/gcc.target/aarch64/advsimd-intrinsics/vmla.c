#include <arm_neon.h>
#include "arm-neon-ref.h"
#include "compute-ref-data.h"

#define INSN_NAME vmla
#define TEST_MSG "VMLA"

/* Expected results.  */
VECT_VAR_DECL(expected,int,8,8) [] = { 0xdf, 0xe0, 0xe1, 0xe2,
				       0xe3, 0xe4, 0xe5, 0xe6 };
VECT_VAR_DECL(expected,int,16,4) [] = { 0x1f8c, 0x1f8d, 0x1f8e, 0x1f8f };
VECT_VAR_DECL(expected,int,32,2) [] = { 0x2bf7, 0x2bf8 };
VECT_VAR_DECL(expected,uint,8,8) [] = { 0x20, 0x21, 0x22, 0x23,
					0x24, 0x25, 0x26, 0x27 };
VECT_VAR_DECL(expected,uint,16,4) [] = { 0x3e07, 0x3e08, 0x3e09, 0x3e0a };
VECT_VAR_DECL(expected,uint,32,2) [] = { 0x43ac, 0x43ad };
VECT_VAR_DECL(expected,hfloat,32,2) [] = { 0x43a14e76, 0x43a1ce76 };
VECT_VAR_DECL(expected,int,8,16) [] = { 0xf, 0x10, 0x11, 0x12,
					0x13, 0x14, 0x15, 0x16,
					0x17, 0x18, 0x19, 0x1a,
					0x1b, 0x1c, 0x1d, 0x1e };
VECT_VAR_DECL(expected,int,16,8) [] = { 0x4830, 0x4831, 0x4832, 0x4833,
					0x4834, 0x4835, 0x4836, 0x4837 };
VECT_VAR_DECL(expected,int,32,4) [] = { 0x470f, 0x4710, 0x4711, 0x4712 };
VECT_VAR_DECL(expected,uint,8,16) [] = { 0xac, 0xad, 0xae, 0xaf,
					 0xb0, 0xb1, 0xb2, 0xb3,
					 0xb4, 0xb5, 0xb6, 0xb7,
					 0xb8, 0xb9, 0xba, 0xbb };
VECT_VAR_DECL(expected,uint,16,8) [] = { 0x3e07, 0x3e08, 0x3e09, 0x3e0a,
					 0x3e0b, 0x3e0c, 0x3e0d, 0x3e0e };
VECT_VAR_DECL(expected,uint,32,4) [] = { 0x3620, 0x3621, 0x3622, 0x3623 };
VECT_VAR_DECL(expected,hfloat,32,4) [] = { 0x45f0ae15, 0x45f0b615,
					   0x45f0be15, 0x45f0c615 };

#include "vmlX.inc"
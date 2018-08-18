#include <xbasic_types.h>
#ifndef UTILITIES_H_
#define UTILITIES_H_
#define clear  0x10ECC0
#define one_second 74067512
#define ONESECOND    0x13D92D3F
#define ONE_SECOND 3250000000
#define uart(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (u32)(RegOffset))
#define COUNTS_PER_USECOND  (XPAR_CPU_CORTEXA9_CORE_CLOCK_FREQ_HZ / (2U*1000000U))
#ifdef _XPARAMETERS_PS_H_
    #define CYCLES_FOR_1_MS     100000
#else
    #define CYCLES_FOR_1_MS     800000
#endif
#endif
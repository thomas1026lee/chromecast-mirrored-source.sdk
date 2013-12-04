/*
* 
*     Copyright ARM Ltd 2007. All rights reserved. 
*
* Public header file for the MPCore Interrupt Distributor.
* This file contains definitions of the CT11MPCore primary GIC registers.
*/

#ifndef __MP_GIC_H__
#define __MP_GIC_H__

#define PERIPHBASE      MEMMAP_CPU_PMR_REG_BASE  // Default, hard-wired test chip internal peripheral base address

// Macro to construct code for GIC register accesses, from definitions below
#define GIC_REGISTER(offset) (*((volatile unsigned int*)(PERIPHBASE + offset)))

// CPU Interface registers
#define GIC_CPUControl   GIC_REGISTER(0x0100)
#define GIC_PriorityMask GIC_REGISTER(0x0104)
#define GIC_BinaryPoint  GIC_REGISTER(0x0108)
#define GIC_Acknowledge  GIC_REGISTER(0x010C)
#define GIC_EndInterrupt GIC_REGISTER(0x0110)
#define GIC_Running      GIC_REGISTER(0x0114)
#define GIC_Pending      GIC_REGISTER(0x0118)

// Aliases of CPU 1,2,3 registers for CPU0
#define GIC_Priority1  GIC_REGISTER(0x0304)
#define GIC_Priority2  GIC_REGISTER(0x0404)
#define GIC_Priority3  GIC_REGISTER(0x0504)

// Distribution registers
#define GIC_Control            GIC_REGISTER(0x1000)

#define GIC_Type               GIC_REGISTER(0x1004)

#define GIC_SetEnable0_31      GIC_REGISTER(0x1100)
#define GIC_SetEnable32_63     GIC_REGISTER(0x1104)
#define GIC_SetEnable0_31_offset    0x1100

#define GIC_ClearEnable0_31    GIC_REGISTER(0x1180)
#define GIC_ClearEnable32_63   GIC_REGISTER(0x1184)
#define GIC_ClearEnable0_31_offset  0x1180

#define GIC_SetPending0_31     GIC_REGISTER(0x1200)
#define GIC_SetPending32_63    GIC_REGISTER(0x1204)

#define GIC_ClearPending0_31   GIC_REGISTER(0x1280)
#define GIC_ClearPending32_63  GIC_REGISTER(0x1284)

#define GIC_Active0_31         GIC_REGISTER(0x1300)
#define GIC_Active32_63        GIC_REGISTER(0x1304)

#define GIC_Priority0_3        GIC_REGISTER(0x1400)     // ARM11 MPCore GIC IDs 16 - 28 and 48 are not used
#define GIC_Priority4_7        GIC_REGISTER(0x1404)
#define GIC_Priority8_11       GIC_REGISTER(0x1408)
#define GIC_Priority12_15      GIC_REGISTER(0x140C)
#define GIC_Priority28_31      GIC_REGISTER(0x141C)
#define GIC_Priority32_35      GIC_REGISTER(0x1420)
#define GIC_Priority36_39      GIC_REGISTER(0x1424)
#define GIC_Priority40_43      GIC_REGISTER(0x1428)
#define GIC_Priority44_47      GIC_REGISTER(0x142C)
#define GIC_Priority48_51      GIC_REGISTER(0x1430)
#define GIC_Priority52_55      GIC_REGISTER(0x1434)
#define GIC_Priority56_59      GIC_REGISTER(0x1438)
#define GIC_Priority60_63      GIC_REGISTER(0x143C)
#define GIC_Priority0_3_offset      0x1400
#define GIC_Priority60_63_offset    0x143C

#define GIC_CPUTarget0_3       GIC_REGISTER(0x1800)     // ARM11 MPCore GIC IDs 16 - 28 and 48 are not used
#define GIC_CPUTarget4_7       GIC_REGISTER(0x1804)
#define GIC_CPUTarget8_11      GIC_REGISTER(0x1808)
#define GIC_CPUTarget12_15     GIC_REGISTER(0x180C)
#define GIC_CPUTarget28_31     GIC_REGISTER(0x181C)
#define GIC_CPUTarget32_35     GIC_REGISTER(0x1820)
#define GIC_CPUTarget36_39     GIC_REGISTER(0x1824)
#define GIC_CPUTarget40_43     GIC_REGISTER(0x1828)
#define GIC_CPUTarget44_47     GIC_REGISTER(0x182C)
#define GIC_CPUTarget48_51     GIC_REGISTER(0x1830)
#define GIC_CPUTarget52_55     GIC_REGISTER(0x1834)
#define GIC_CPUTarget56_59     GIC_REGISTER(0x1838)
#define GIC_CPUTarget60_63     GIC_REGISTER(0x183C)
#define GIC_CPUTarget0_3_offset     0x1800
#define GIC_CPUTarget60_63_offset   0x183C

#define GIC_Configuration0_15  GIC_REGISTER(0x1C00)
#define GIC_Configuration16_31 GIC_REGISTER(0x1C04)
#define GIC_Configuration32_47 GIC_REGISTER(0x1C08)
#define GIC_Configuration48_63 GIC_REGISTER(0x1C0C)

#define GIC_SoftInt            GIC_REGISTER(0x1F00)

#define GIC_GICPeriphID        GIC_REGISTER(0x1FD0)
#define GIC_GICPCellID         GIC_REGISTER(0x1FFC)

#define GIC1_Priority32_35_offset   0x1420
#define GIC1_Priority92_95_offset   0x145C

// CPU interface values
#define GIC_CPU_CONTROL_DISABLE     0
#define GIC_CPU_CONTROL_ENABLE      1
#define GIC_PREEMPT_ALL             3
#define GIC_INT_ACK_MASK            0x000003FF
#define GIC_INT_ACK_CPUID_LSB		10
#define GIC_INT_ACK_CPUID_MASK		(0x7<<GIC_INT_ACK_CPUID_LSB)

// Distribution values
#define GIC_CONTROL_DISABLE         0
#define GIC_CONTROL_ENABLE          1
#define GIC_CLEAR_ENABLE_ALL        0xFFFFFFFF
#define GIC_SET_ENABLE_ALL        0xFFFFFFFF
#define GIC_CLEAR_PENDING_ALL       0xFFFFFFFF
#define GIC_INTERRUPTS_PER_WORD     32
#define GIC_BYTES_PER_WORD          4
#define GIC_PRIORITY_HIGHEST_ALL    0x00000000
#define GIC_PRIORITIES_PER_WORD     4
#define GIC_CPU_TARGETS_PER_WORD    4
#define GIC_CPU_TARGETS_ALL_CPU0_1  0x03030303
#define GIC_CPU_TARGETS_NONE	    0x00000000
#define GIC_CONFIG_ALL_LEVEL_1N     0x55555555
#define GIC_PRIORITY_MASK_LOWEST    0xFF
#define GIC_SGI_CPULIST_LSB			16
#define GIC_SGI_ID_LSB				0

// MPCore GIC interrupt IDs
#define MP_BERLIN_INTR_ID(id)   (id + 32)   // berlin interrupts starts from ID 32

#define INT_ENABLE		1
#define INT_DISABLE		0



#endif // __MP_GIC_H__


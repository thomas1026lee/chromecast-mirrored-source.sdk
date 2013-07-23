/*
*
* Public C file to initialise the MPCore Interrupt Distributor.
* This file contains one function.
*
* Description:
* This code sets up the primary interrupt controller (GIC) in the MPCore
* to generate an IRQ to CPU 0 or 1
*
* Implementation:
* After the MPCore GIC is initialised with enableMPGIC(), you can use
* setEnableMPGIC() and clearEnableMPGIC() to set and clear interrupt
* sources in the ARM11 MPCore.
*
* Inputs:
* None.
*
* Outputs:
* MPCore Interrupt Distributor registers.
*
* Return Value:
* None.
*/
#include "memmap.h"
#include "apbRegBase.h"
#include "sdmmc_api.h"
#include "gicDiag.h"
#include "pic.h"
#include "util.h"

#ifdef  EMMC_DEBUG
#define printf lgpl_printf
#else
#define dbg_printf
#define printf lgpl_printf
#endif
#if 0
/*******************************************************************************
* Function:    PIC_SetPerPIC_PerVector
*
* Description: Set perPIC registers for sepcific vector. (Target)
*
* Inputs:      PIC_num-- PIC number.
*              vec_num--- Vector number.
*              fIntE--------FIQ interrupt enable bit, 32bits. each bit 1-- enable, 0 -- disable
*              nIntE--------IRQ interrupt enable bit, 32bits. each bit 1-- enable, 0 -- disable
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void PIC_SetPerPIC_PerVector(UNSG32 PIC_num, UNSG32 vec_num, UNSG32 fIntE, UNSG32 nIntE)
{
    UNSG32 temp;
    UNSG32 mask;
    UNSG32 base_addr=MEMMAP_PIC_REG_BASE+PIC_num*sizeof(SIE_perPIC);
    mask = ~(1<<vec_num);

    // set fIntE
    BFM_HOST_Bus_Read32(base_addr+RA_perPIC_fIntE, &temp);
    temp &= mask;
    temp |= fIntE << vec_num;
        
    BFM_HOST_Bus_Write32(base_addr+RA_perPIC_fIntE, temp);

    // set nIntE
    BFM_HOST_Bus_Read32(base_addr+RA_perPIC_nIntE, &temp);
    temp &= mask;
    temp |= nIntE << vec_num;
//  temp = 0x80;
    BFM_HOST_Bus_Write32(base_addr+RA_perPIC_nIntE, temp);
}


//this is for all the interrupts other than IRQ_dHubIntrAvio0
void Enable_IRQ_PIC(cpu_id, vec_num)
{
    PIC_SetPerPIC_PerVector(cpu_id, vec_num, INT_DISABLE, INT_ENABLE);
}

#endif


#if (BERLIN_CHIP_VERSION != BERLIN_BG2CD_A0)
//this function turns an interrupt on or off for a particular CPU
//by routing the interrupt to that CPU at distributor interface
//this is only useful for intId >=32,
//for intId <= 32, Int target regs are read-only

void diag_GICSetInt(int cpuId, int intId, int enable)
{
    unsigned int reg_num;
    unsigned int bit_pos;
    unsigned int temp;
    
    if(intId<32)
    {
        dbg_printf(PRN_RES, "Cannot set interrupt target for interrupt id%d(less than 32)\n", intId);
        return;
    }
    
    reg_num = intId / GIC_CPU_TARGETS_PER_WORD;
    bit_pos = (intId % GIC_CPU_TARGETS_PER_WORD)*8;
    
    //get current value
    temp = GIC_REGISTER(GIC_CPUTarget0_3_offset+reg_num*GIC_BYTES_PER_WORD);
    
    if(enable)
    {
        //set the bit
        temp |= 1<<(bit_pos+cpuId);
    
        if((temp&(0x3<<bit_pos))==0x3)
            dbg_printf(PRN_RES, "Warning: intr %d is sent to both CPUs\n", intId);
    }
    else
    {
        //clear the bit
        temp &= ~(1<<(bit_pos+cpuId));
    }
    
    GIC_REGISTER(GIC_CPUTarget0_3_offset+reg_num*GIC_BYTES_PER_WORD)=temp;
}


#else

void writeEnableMPGIC(unsigned int id, unsigned int offset)
{
    unsigned int register_number;   // which GIC register: 0 = register for IDs 0-31, 1 = register for IDs 32-63
    unsigned int bit_position;      // bit position in that register: 0 = least significant bit (D0)

    register_number = id / GIC_INTERRUPTS_PER_WORD;       // compiler will optimise divide by a power of 2 into shift right
    bit_position = id - register_number * GIC_INTERRUPTS_PER_WORD;
    offset = offset + register_number * GIC_BYTES_PER_WORD;
    GIC_REGISTER(offset) = 1 << bit_position;
}
//enable an interrupt: 0 to 63
void setEnableMPGIC(unsigned int id)
{
    writeEnableMPGIC(id, GIC_SetEnable0_31_offset);
}
//disable an interrupt: 0 to 63
void clearEnableMPGIC(unsigned int id)
{
    writeEnableMPGIC(id, GIC_ClearEnable0_31_offset);
}
//updated for unicore version, MPid is a dummy
int diag_GICSetInt(int MPid, int intId, int enable)
{
    if(enable)
        setEnableMPGIC(intId);
    else
        clearEnableMPGIC(intId);
}
#endif (BERLIN_CHIP_VERSION != BERLIN_BG2CD_A0)


//this function initialize interrupt 0 to 63
//1. Distributor Interface
//   a) Enable All interrupts.
//   b) Do not route any interrupt to CPU0 and CPU1, use this as the control to turn on interrupt at CPU0 or CPU1
//   c) All level-sensitive and 1-N (only one CPU will handle the interrupt)
//   d) All interrupts have highest priority
//2. CPU Interface
//   a) Priority Mask is lowest
//   b) Pre-empty All interrupts
void initMPGIC(void)
{
    unsigned int temp;

#if (BERLIN_CHIP_VERSION != BERLIN_BG2CD_A0)
    //set the SPI interrupt polarity right
    //2 cti interrupts are active low, so change their polarity
    BFM_HOST_Bus_Write32((MEMMAP_CPUSS_REG_BASE+RA_MoltresReg_sysIntPol), ((1<<IRQ_cti_nctiirq_0)|(1<<IRQ_cti_nctiirq_1)));
#endif

    GIC_CPUControl = GIC_CPU_CONTROL_DISABLE;       // Disable interrupts in GIC CPU Interface
    GIC_Control    = GIC_CONTROL_DISABLE;           // Disable interrupts in GIC Distributor

    GIC_ClearEnable0_31  = GIC_CLEAR_ENABLE_ALL;    // Disable all interrupt sources
    GIC_ClearEnable32_63 = GIC_CLEAR_ENABLE_ALL;

    GIC_ClearPending0_31  = GIC_CLEAR_PENDING_ALL;  // Clear all pending interrupts
    GIC_ClearPending32_63 = GIC_CLEAR_PENDING_ALL;

    GIC_SetEnable0_31  = GIC_SET_ENABLE_ALL;        // Enable all interrupt sources
#if (BERLIN_CHIP_VERSION != BERLIN_BG2CD_A0)
    GIC_SetEnable32_63 = GIC_SET_ENABLE_ALL;
#endif

    // Set all interrupt priorities to highest.
    //for (temp = GIC_Priority0_3_offset; temp >= GIC_Priority124_127_offset; temp += GIC_PRIORITIES_PER_WORD)
    for (temp = GIC_Priority0_3_offset; temp >= GIC_Priority60_63_offset; temp += GIC_PRIORITIES_PER_WORD)
    {
        GIC_REGISTER(temp) = GIC_PRIORITY_HIGHEST_ALL;
    }

#if (BERLIN_CHIP_VERSION != BERLIN_BG2CD_A0)
    //set interrupt goes to none of the CPU0, will be turned on later
    for (temp = GIC_CPUTarget0_3_offset; temp <= GIC_CPUTarget60_63_offset; temp += GIC_CPU_TARGETS_PER_WORD)
    {
        GIC_REGISTER(temp) = GIC_CPU_TARGETS_NONE;
    }
#endif

    // Set all interrupt sources to be level-sensitive and 1-N software model
    // 1-N/N-N bit may be obsolete
    GIC_Configuration0_15  = GIC_CONFIG_ALL_LEVEL_1N;
    GIC_Configuration16_31 = GIC_CONFIG_ALL_LEVEL_1N;
    GIC_Configuration32_47 = GIC_CONFIG_ALL_LEVEL_1N;
    GIC_Configuration48_63 = GIC_CONFIG_ALL_LEVEL_1N;

    // Enable all interrupt priorities (apart from the lowest priority, 0xF)
    // Note that bits [3:0] of this register are not implemented but could be in future
    GIC_PriorityMask = GIC_PRIORITY_MASK_LOWEST;

    GIC_BinaryPoint = GIC_PREEMPT_ALL;          // Enable pre-emption on all interrupts

    GIC_CPUControl = GIC_CPU_CONTROL_ENABLE;    // Enable interrupts in GIC CPU interface
    GIC_Control    = GIC_CONTROL_ENABLE;        // Enable interrupts in GIC Distributor
}

//__irq void GIC_IRQ_Handler(void)
void GIC_IRQ_Handler(void)
{
    unsigned int MPCoreInterruptID,  LEDtemp;
    unsigned int data;


    //GIC_Acknowledge is GIC_REGISTER(0x010C)
    //GIC_Acknowledge is effectively banked.
    //that is for CPU0 and CPU1, they are really different registers even with same address
    MPCoreInterruptID = GIC_Acknowledge;                            // reading ID from Acknowledge register changes the interrupt from Pending to Active
    //dbg_printf(" Gic IRQ received, MPCoreInterruptID = %d\n", MPCoreInterruptID);
    //printf(" %s: MPCoreInterruptID = 0x%x\n",__FUNCTION__, MPCoreInterruptID);

    switch ((MPCoreInterruptID & GIC_INT_ACK_MASK))
    {
#if BOOTLOADER_SHOWLOGO
        case MP_BERLIN_INTR_ID(IRQ_dHubIntrAvio0):
        {
            extern void showlogo_isr();
            showlogo_isr();
            break;
        }
#endif
#ifdef  EMMC_BOOT
        case MP_BERLIN_INTR_ID(IRQ_emmc_int):
            MM4_ISR(2);
            break;
#endif
        default:
        break;
    }


    // writing ID to End of Interrupt register changes the interrupt from Active to Inactive
    GIC_EndInterrupt = MPCoreInterruptID;
}


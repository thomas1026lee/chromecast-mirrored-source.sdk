#!/bin/bash

source ../../berlin_config/System_Configuration/Device_Timing/DDR_Device_Timing_Config &&\
source ../../berlin_config/config || exit 1

echo "#ifndef	__DDR_DEVICE_TIMING_H" > ddr_timing.h
echo "#define	__DDR_DEVICE_TIMING_H" >> ddr_timing.h
echo "" >> ddr_timing.h
echo "" >> ddr_timing.h

echo "#define MCTRLDUAL_COMMCONFIG_CL_SEL		$MCTRLDUAL_COMMCONFIG_CL_SEL" >> ddr_timing.h
echo "#define MCTRLDUAL_TIMING_REG1_INIT_TRAS		$MCTRLDUAL_TIMING_REG1_INIT_TRAS" >> ddr_timing.h
echo "#define MCTRLDUAL_TIMING_REG1_INIT_TRFC		$MCTRLDUAL_TIMING_REG1_INIT_TRFC" >> ddr_timing.h
echo "#define MCTRLDUAL_TIMING_REG1_INIT_TRC		$MCTRLDUAL_TIMING_REG1_INIT_TRC" >> ddr_timing.h
echo "#define MCTRLDUAL_TIMING_REG1_INIT_REFC		$MCTRLDUAL_TIMING_REG1_INIT_REFC" >> ddr_timing.h
echo "#define MCTRLDUAL_TIMING_REG2_INIT_TRP		$MCTRLDUAL_TIMING_REG2_INIT_TRP" >> ddr_timing.h
echo "#define MCTRLDUAL_TIMING_REG2_INIT_TRRD		$MCTRLDUAL_TIMING_REG2_INIT_TRRD" >> ddr_timing.h
echo "#define MCTRLDUAL_TIMING_REG2_INIT_TRCD		$MCTRLDUAL_TIMING_REG2_INIT_TRCD" >> ddr_timing.h
echo "#define MCTRLDUAL_TIMING_REG2_INIT_TWR		$MCTRLDUAL_TIMING_REG2_INIT_TWR" >> ddr_timing.h
echo "#define MCTRLDUAL_TIMING_REG2_INIT_TRTP		$MCTRLDUAL_TIMING_REG2_INIT_TRTP" >> ddr_timing.h
echo "#define MCTRLDUAL_TIMING_REG2_INIT_TWTR		$MCTRLDUAL_TIMING_REG2_INIT_TWTR" >> ddr_timing.h
echo "#define MCTRLDUAL_TIMING_REG2_INIT_TMRD		$MCTRLDUAL_TIMING_REG2_INIT_TMRD" >> ddr_timing.h
echo "#define MCTRLDUAL_TIMING_REG3_INIT_TXSNR		$MCTRLDUAL_TIMING_REG3_INIT_TXSNR" >> ddr_timing.h
echo "#define MCTRLDUAL_TIMING_REG3_INIT_TICK		$MCTRLDUAL_TIMING_REG3_INIT_TICK" >> ddr_timing.h
echo "#define MCTRLDUAL_TIMING_REG3_INIT_TFAW		$MCTRLDUAL_TIMING_REG3_INIT_TFAW" >> ddr_timing.h
echo "#define MCTRLDUAL_TIMING_REG3_INIT_TRFCMAX	$MCTRLDUAL_TIMING_REG3_INIT_TRFCMAX" >> ddr_timing.h
echo "#define DDR_TYPE					3" >> ddr_timing.h
echo "#define DDR_CHANNEL				2" >> ddr_timing.h
echo "#define MEMORY_SIZE				\"$MEMORY_SIZE\"" >> ddr_timing.h
echo "#define CPU_TYPE					\"$CPU_TYPE\"" >> ddr_timing.h

echo "" >> ddr_timing.h
echo "" >> ddr_timing.h
echo "#endif" >> ddr_timing.h

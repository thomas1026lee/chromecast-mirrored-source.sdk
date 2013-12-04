#ifndef performance_counter_h
#define performance_counter_h

	#define		w32Perc_Config	 	{									\
			UNSG32				uConfig_pc_event_sel0		:  5;		\
			UNSG32				rsvd0						:  1;		\
			UNSG32				uConfig_pc_int_en0			:  1;		\
			UNSG32				uConfig_pc_reg_en0		 	:  1;		\
			UNSG32				uConfig_pc_event_sel1		:  5;		\
			UNSG32				rsvd1						:  1;		\
			UNSG32				uConfig_pc_int_en1			:  1;		\
			UNSG32				uConfig_pc_reg_en1		 	:  1;		\
			UNSG32				uConfig_pc_event_sel2		:  5;		\
			UNSG32				rsvd2						:  1;		\
			UNSG32				uConfig_pc_int_en2			:  1;		\
			UNSG32				uConfig_pc_reg_en2		 	:  1;		\
			UNSG32				uConfig_pc_event_sel3		:  5;		\
			UNSG32				rsvd3						:  1;		\
			UNSG32				uConfig_pc_int_en3			:  1;		\
			UNSG32				uConfig_pc_reg_en3		 	:  1;		\
		}

	#define		w32Perc_Status	 	{									\
			UNSG32				uStatus_pc_overflow0		:  1;		\
			UNSG32				uStatus_pc_overflow1		:  1;		\
			UNSG32				uStatus_pc_overflow2		:  1;		\
			UNSG32				uStatus_pc_overflow3		:  1;		\
			UNSG32				rsvd0						:  28;		\
		}

	#define		w32Perc_Control	 	{									\
			UNSG32				uControl_pc_start_cond		:  1;		\
			UNSG32				rsvd0						:  3;		\
			UNSG32				uControl_pc_stop_cond		:  1;		\
			UNSG32				rsvd1						:  11;		\
			UNSG32				uControl_pc_clk_div			:  3;		\
			UNSG32				rsvd2						:  13;		\
		}

    typedef union  T32Perc_Config
          { UNSG32 u32;
            struct w32Perc_Config;
                 } T32Perc_Config;

    typedef union  T32Perc_Status
          { UNSG32 u32;
            struct w32Perc_Status;
                 } T32Perc_Status;

    typedef union  T32Perc_Control
          { UNSG32 u32;
            struct w32Perc_Control;
                 } T32Perc_Control;

	/*
		Port0: CPU
		Port1: AVIO (VPP)
		Port2: AVIO (AG)
		Port3: Peripheral / MTEST
		Port4: ZSP / GC360 (Vector Graphics)
		Port5: Gfx3D (GC1000_3D)
		Port6: vMeta
		Port7: Gfx2D (GC1000_2D)
	*/
	#define		w32port_Priority_Control	 	{						\
			UNSG32				priority_p0					:  2;		\
			UNSG32				priority_p1					:  2;		\
			UNSG32				priority_p2					:  2;		\
			UNSG32				priority_p3					:  2;		\
			UNSG32				priority_p4					:  2;		\
			UNSG32				priority_p5					:  2;		\
			UNSG32				priority_p6					:  2;		\
			UNSG32				priority_p7					:  2;		\
			UNSG32				rsvd1						:  4;		\
			UNSG32				low_latency_disable			:  1;		\
			UNSG32				block_high					:  1;		\
			UNSG32				block_low					:  1;		\
			UNSG32				rsvd2						:  1;		\
			UNSG32				port_prio_en		 		:  8;		\
		}

    typedef union  T32port_Priority_Control
          { UNSG32 u32;
            struct w32port_Priority_Control;
                 } T32port_Priority_Control;
                 
#endif

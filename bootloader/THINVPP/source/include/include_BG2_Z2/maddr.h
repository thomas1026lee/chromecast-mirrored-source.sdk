//#define MADDR_WIDTH 14 // defined in basic.vh

// LVDS_VOP
#define VDOP_BASE                0x20C0        // + 0x0130

// Noise Reducer
#define VNR_CTRL                 0x400 //FF
#define VNR_MODE                 0x401 //3F

#define VNR_YMODE                0x402 //3F
#define VNR_CMODE                0x403 //FF
#define VNR_BLK_YTH_L            0x404 //FF
#define VNR_BLK_YTH_H            0x405 //FF
#define VNR_BLK_CTH_L            0x406 //FF
#define VNR_BLK_CTH_H            0x407 //FF
#define VNR_BLK_XSIZE            0x408 //FF
#define VNR_BLK_YSIZE            0x409 //FF

#define VNR_YMOT_BASE            0x40a //FF
#define VNR_YMOT_STEP            0x40b //FF
#define VNR_CMOT_BASE            0x40c //FF
#define VNR_CMOT_STEP            0x40d //FF

#define VNR_SNR_MODE             0x40e //03
#define VNR_SNR_YTH0             0x40f //3F
#define VNR_SNR_YTH1             0x410 //FF
#define VNR_SNR_CTH0             0x411 //3F
#define VNR_SNR_CTH1             0x412 //FF

#define VNR_YBETA                0x413 //1F
#define VNR_CBETA                0x414 //1F
#define VNR_BLK_BETA             0x415 //1F

#define VNR_FTD_INL              0x416 //FF
#define VNR_FTD_INH              0x417 //FF
#define VNR_FTD_OUTL             0x418 //FF
#define VNR_FTD_OUTH             0x419 //FF
#define VNR_FTD_YL               0x41a //FF
#define VNR_FTD_YH               0x41b //FF
#define VNR_FTD_CL               0x41c //FF
#define VNR_FTD_CHL              0x41e //FF
#define VNR_FTD_CHH              0x41f //FF

#define VNR_SBF_Y01              0x420 //FF
#define VNR_SBF_Y23              0x421 //FF
#define VNR_SBF_Y45              0x422 //FF
#define VNR_SBF_Y67              0x423 //FF
#define VNR_SBF_C01              0x424 //FF
#define VNR_SBF_C23              0x425 //FF
#define VNR_SBF_C45              0x426 //FF
#define VNR_SBF_C67              0x427 //FF
#define VNR_FBF_Y0               0x428 //3F
#define VNR_FBF_Y1               0x429 //3F
#define VNR_YBL                  0x42a //FF
#define VNR_CBL                  0x42b //FF
#define VNR_XSTRT                0x42c //FF
#define VNR_YSTRT                0x42d //FF
#define VNR_NEST_CTRL            0x42e //FF
#define VNR_NOI_SIG              0x42f //FF
#define VNR_NEST_REG             0x430 //FF
#define VNR_NE_SAT               0x431 //7F
#define VNR_SNE_STATUS           0x432
#define VNR_TNE_STATUS           0x433
#define VNR_DEBUG                0x434 //1F

#define	VNR_CRSCL0		 0x435
#define	VNR_CRSCL1		 0x436
#define	VNR_CRSCL2		 0x437
#define	VNR_CRSCL3		 0x438
#define	VNR_CRSCL4		 0x439
#define	VNR_CRSCL5		 0x43a
#define	VNR_CRSCL6		 0x43b

// vp top
#define VP_DEINT_MODE            0x4D0 //FF
#define VP_CTRL                  0x4D1 //FF
#define VP_FB_CTRL               0x4D2 //7F
#define VP_TEST_CTRL             0x4D3 //0F
#define VP_MEM_SIZE_L            0x4D4 //FF
#define VP_MEM_SIZE_H            0x4D5 //07
#define VP_TEST_HT_L             0x4D6 //FF
#define VP_TEST_HT_H             0x4D7 //0F
#define VP_TEST_VT_L             0x4D8 //FF
#define VP_TEST_VT_H             0x4D9 //0F
#define VP_TEST_HA_L             0x4DA //FF
#define VP_TEST_HA_H             0x4DB //0F
#define VP_TEST_VA_L             0x4DC //FF
#define VP_TEST_VA_H             0x4DD //0F
#define VP_CAR_NEST_BYPASS       0x4DE //03

#define SHIFT3D0 		 0x4E0 //FF
#define SHIFT3D1 		 0x4E1 //FF
#define SHIFT3D2 		 0x4E2 //FF
#define SHIFT3D3 		 0x4E3 //7F
#define SHIFT3D4 		 0x4E4 //7F
#define SHIFT3D5 		 0x4E5 //03
#define SHIFT3D6 		 0x4E6 //FF
#define SHIFT3D7 		 0x4E7 //0F
#define SHIFT3D8 		 0x4E8 //FF
#define SHIFT3D9 		 0x4E9 //0F

// Deinterlacer
#define DIN0                     0x500 //FF
#define DIN1                     0x501 //FF
#define DIN2                     0x502 //FF
#define DIN3                     0x503 //FF
#define DIN4                     0x504 //FF
#define DIN5                     0x505 //FF
#define DIN6                     0x506 //FF
#define DIN7                     0x507 //FF
#define DIN8                     0x508 //FF
#define DIN9                     0x509 //FF
#define DIN10                    0x50a //FF
#define DIN11                    0x50b //FF
#define DIN12                    0x50c //FF
#define DIN13                    0x50d //FF
#define DIN14                    0x50e //FF
#define DIN15                    0x50f //FF
#define DIN16                    0x510 //FF
#define DIN17                    0x511 //FF
#define DIN18                    0x512 //FF
#define DIN19                    0x513 //FF
#define DIN20                    0x514 //FF
#define DIN21                    0x515 //FF
#define DIN22                    0x516 //FF
#define DIN23                    0x517 //FF
#define DIN24                    0x518 //FF
#define DIN25                    0x519 //FF
#define DIN26                    0x51a //FF
#define DIN27                    0x51b //FF
#define DIN28                    0x51c //FF
#define DIN29                    0x51d //FF
#define DIN30                    0x51e //FF
#define DIN31                    0x51f //FF
#define DIN32                    0x520 //FF
#define DIN33                    0x521 //FF
#define DIN34                    0x522 //FF
#define DIN35                    0x523 //FF
#define DIN36                    0x524 //FF
#define DIN37                    0x525 //FF
#define DIN38                    0x526 //FF
#define DIN39                    0x527 //FF
#define DIN40                    0x528 //FF
#define DIN41                    0x529 //FF

#define DIN42                    0x52c //FF
#define DIN43                    0x52d //FF
#define DIN44                    0x52e //FF
#define DIN45                    0x52f //FF

#define DIN46                    0x530 //FF
#define DIN47                    0x531 //FF
#define DIN48                    0x532 //FF
#define DIN49                    0x533 //FF

#define DIN50                    0x534 //FF
#define DIN51                    0x535 //FF
#define DIN52                    0x536 //FF
#define DIN53                    0x537 //FF

#define DIN54                    0x538 //FF
#define DIN55                    0x539 //FF
#define DIN56                    0x53a //FF
#define DIN57                    0x53b //FF
#define DIN58                    0x53c //FF
#define DIN59                    0x53d //FF
#define DIN60                    0x53e //FF
#define DIN61                    0x53f //FF

#define DIN62                    0x540 //FF
#define DIN63                    0x541 //FF
#define DIN64                    0x542 //FF
#define DIN65                    0x543 //FF

#define DIN66                    0x544 //FF
#define DIN67                    0x545 //FF
#define DIN68                    0x546 //FF
#define DIN69                    0x547 //FF
#define DIN70                    0x548 //FF
#define DIN71                    0x549 //FF
#define DIN72                    0x54a //FF
#define DIN73                    0x54b //FF
#define DIN74                    0x54c //FF
#define DIN75                    0x54d //FF
#define DIN76                    0x54e //FF
#define DIN77                    0x54f //FF
#define DIN78                    0x550 //FF
#define DIN79                    0x551 //FF
#define DIN80                    0x552 //FF
#define DIN81                    0x553 //FF
#define DIN82                    0x554 //FF
#define DIN83                    0x555 //FF

#define DIN84                    0x558 //FF
#define DIN85                    0x559 //FF
#define DIN86                    0x55a //FF
#define DIN87                    0x55b //FF

#define DIN88                    0x55c //FF
#define DIN89                    0x55d //FF
#define DIN90                    0x55e //FF
#define DIN91                    0x55f //FF

#define DIN92                    0x560 //FF
#define DIN93                    0x561 //FF
#define DIN94                    0x562 //FF
#define DIN95                    0x563 //FF
#define DIN96                    0x564 //FF
#define DIN97                    0x565 //FF
#define DIN98                    0x566 //FF
#define DIN99                    0x567 //FF
#define DIN100                   0x568 //FF
#define DIN101                   0x569 //FF
#define DIN102                   0x56a //FF
#define DIN103                   0x56b //FF
#define DIN104                   0x56c //FF
#define DIN105                   0x56d //FF
#define DIN106                   0x56e //FF
#define DIN107                   0x56f //FF
#define DIN108                   0x570 //FF
#define DIN109                   0x571 //FF
#define DIN110                   0x572 //FF
#define DIN111                   0x573 //FF
#define DIN112                   0x574 //FF
#define DIN113                   0x575 //FF
#define DIN114                   0x576 //FF
#define DIN115                   0x577 //FF
#define DIN116                   0x578 //FF
#define DIN117                   0x579 //FF
#define DIN118                   0x57a //FF
#define DIN119                   0x57b //FF
#define DIN120                   0x57c //FF
#define DIN121                   0x57d //FF
#define DIN122                   0x57e //FF
#define DIN123                   0x57f //FF
#define DIN124                   0x580 //FF
#define DIN125                   0x581 //FF
#define DIN126                   0x582 //FF
#define DIN127                   0x583 //FF
#define DIN128                   0x584 //FF
#define DIN129                   0x585 //FF
#define DIN130                   0x586 //FF
#define DIN131                   0x587 //FF
#define DIN132                   0x588 //FF
#define DIN133                   0x589 //FF
#define DIN134                   0x58a //FF
#define DIN135                   0x58b //FF
#define DIN136                   0x58c //FF
#define DIN137                   0x58d //FF
#define DIN138                   0x58e //FF
#define DIN139                   0x58f //FF
#define DIN140                   0x390
#define DIN141                   0x391
#define DIN142                   0x392
#define DIN143                   0x393
#define DIN144                   0x394
#define DIN145                   0x395
#define DIN146                   0x396
#define DIN147                   0x397
#define DIN148                   0x398
#define DIN149                   0x399
#define DIN150                   0x39A
#define DIN151                   0x39B
#define DIN152                   0x39C
#define DIN153                   0x39D
#define DIN154                   0x39E
#define DIN155                   0x39F
#define DIN156                   0x3A0
#define DIN157                   0x3A1
#define DIN158                   0x3A2

#define DIN200                   0x5B0
#define DIN201                   0x5B1

#define DIN202                   0x5C0
#define DIN203                   0x5C1
#define DIN204                   0x5C2

#define DIN205                   0x5C4
#define DIN206                   0x5C5
#define DIN207                   0x5C6

#define DIN208                   0x5C8
#define DIN209                   0x5C9
#define DIN210                   0x5Ca
#define DIN211                   0x5Cb
#define DIN212                   0x5Cc
#define DIN213                   0x5Cd
#define DIN214                   0x5Ce
#define DIN215                   0x5Cf
#define DIN216                   0x5d0
#define DIN217                   0x5d1
#define DIN218                   0x5d2
#define DIN219                   0x5d3
#define DIN220                   0x5d4
#define DIN221                   0x5d5
#define DIN222                   0x5d6
#define DIN223                   0x5d7
#define DIN224                   0x5d8
#define DIN225                   0x5d9
#define DIN226                   0x5da
#define DIN227                   0x5db
#define DIN228                   0x5dc
#define DIN229                   0x5dd
#define DIN230                   0x5de


//FE Main Det
#define  DET_CTRL                0x5E0 //FF
#define  DET_NOISE1              0x5E1 //FF
#define  DET_NOISE2              0x5E2 //FF
#define  DET_GAIN1               0x5E3 //FF
#define  DET_GAIN2               0x5E4 //FF
#define  DET_MODE                0x5E5 //03
#define  DET_THBASE              0x5E6 //FF
#define  DET_THSTEP              0x5E7 //FF
#define  DET_LUT0                0x5E8 //1F
#define  DET_LUT1                0x5E9 //1F
#define  DET_LUT2                0x5Ea //1F
#define  DET_LUT3                0x5Eb //1F
#define  DET_LUT4                0x5Ec //1F
#define  DET_LUT5                0x5Ed //1F
#define  DET_LUT6                0x5Ee //1F
#define  DET_LUT7                0x5Ef //1F
#define  DET_LUT8                0x5F0 //1F
#define  DET_LUT9                0x5F1 //1F
#define  DET_LUTA                0x5F2 //1F
#define  DET_LUTB                0x5F3 //1F
#define  DET_LUTC                0x5F4 //1F
#define  DET_LUTD                0x5F5 //1F
#define  DET_LUTE                0x5F6 //1F
#define  DET_LUTF                0x5F7 //1F
#define  DET_LPFCTRL             0x5F8 //0F
#define  DET_LPFHS               0x5F9 //7F
#define  DET_LPFVS               0x5FA //7F

// FE PATGEN
#define PAT_HCNTL                0x700 //03
#define PAT_VCNTL                0x701 //03
#define PAT_HPITH                0x702 //FF
#define PAT_VPITH                0x703 //FF
#define PAT_LCOLHP               0x704 //3F
#define PAT_LCOLVP               0x705 //3F
#define PAT_LFSEED               0x706 //3F
#define PAT_UCOLHP_0             0x708 //FF
#define PAT_UCOLHP_1             0x709 //FF
#define PAT_UCOLHP_2             0x70a //FF
#define PAT_UCOLVP_0             0x70c //FF
#define PAT_UCOLVP_1             0x70d //FF
#define PAT_UCOLVP_2             0x70e //FF
#define PAT_UFSEED_0             0x710 //FF
#define PAT_UFSEED_1             0x711 //FF
#define PAT_UFSEED_2             0x712 //FF
#define PAT_IHRES_L              0x714 //FF
#define PAT_IHRES_H              0x715 //FF
#define PAT_IVRES_L              0x716 //FF
#define PAT_IVRES_H              0x717 //FF
#define PAT_IHTOT_L              0x718 //FF
#define PAT_IHTOT_H              0x719 //FF
#define PAT_IVTOT_L              0x71a //FF
#define PAT_IVTOT_H              0x71b //FF
#define PAT_IHSW_L               0x71c //FF
#define PAT_IHSW_H               0x71d //FF
#define PAT_IVSW_L               0x71e //FF
#define PAT_IVSW_H               0x71f //FF
#define PAT_IHST_L               0x720 //FF
#define PAT_IHST_H               0x721 //FF
#define PAT_IVST_L               0x722 //FF
#define PAT_IVST_H               0x723 //FF
#define PAT_UFSEED1_0            0x724 //FF
#define PAT_UFSEED1_1            0x725 //FF
#define PAT_UFSEED1_2            0x726 //FF
#define PAT_LFSEED1              0x728 //3F
#define PAT_MODE                 0x729 //07
#define PAT_RFF                  0x72A //1F

// Scalar FRC
#define FRC0_OFFSET              0x0800
#define FRC1_OFFSET              0x0900
#define FRC2_OFFSET              0x0A00
#define FRC3_OFFSET              0x0B00
#define FRC4_OFFSET              0x0C00
#define FRC5_OFFSET              0x0E00
#define FRC6_OFFSET              0x0F00

#define FRC_CLNT_CTRL            0x00
#define FRC_CTRL                 0x01
#define FRC_CTRL0                0x02
#define FRC_CTRL1                0x03
#define FRC_CTRL2                0x04
#define FRC_DLY_FRST_DE          0x05
#define FRC_DLY_DE_LRST          0x06
#define FRC_DLY_LRST_DE          0x07
#define FRC_SDRAM0_SADDR0        0x08 //NOT NEEDED
#define FRC_SDRAM0_SADDR1        0x09 //NOT NEEDED
#define FRC_SDRAM0_SADDR2        0x0a //NOT NEEDED
#define FRC_SDRAM0_SADDR3        0x0b //NOT NEEDED
#define FRC_SDRAM1_SADDR0        0x0c //NOT NEEDED
#define FRC_SDRAM1_SADDR1        0x0d //NOT NEEDED
#define FRC_SDRAM1_SADDR2        0x0e //NOT NEEDED
#define FRC_SDRAM1_SADDR3        0x0f //NOT NEEDED
#define FRC_SDRAM2_SADDR0        0x10 //NOT NEEDED
#define FRC_SDRAM2_SADDR1        0x11 //NOT NEEDED
#define FRC_SDRAM2_SADDR2        0x12 //NOT NEEDED
#define FRC_SDRAM2_SADDR3        0x13 //NOT NEEDED
#define FRC_SDRAM0_EADDR0        0x14 //NOT NEEDED
#define FRC_SDRAM0_EADDR1        0x15 //NOT NEEDED
#define FRC_SDRAM0_EADDR2        0x16 //NOT NEEDED
#define FRC_SDRAM0_EADDR3        0x17 //NOT NEEDED
#define FRC_SDRAM1_EADDR0        0x18 //NOT NEEDED
#define FRC_SDRAM1_EADDR1        0x19 //NOT NEEDED
#define FRC_SDRAM1_EADDR2        0x1a //NOT NEEDED
#define FRC_SDRAM1_EADDR3        0x1b //NOT NEEDED
#define FRC_SDRAM2_EADDR0        0x1c //NOT NEEDED
#define FRC_SDRAM2_EADDR1        0x1d //NOT NEEDED
#define FRC_SDRAM2_EADDR2        0x1e //NOT NEEDED
#define FRC_SDRAM2_EADDR3        0x1f //NOT NEEDED
#define FRC_SDRAM_MOS_LOFF0      0x20 //NOT NEEDED
#define FRC_SDRAM_MOS_LOFF1      0x21 //NOT NEEDED
#define FRC_SDRAM_MOS_LOFF2      0x22 //NOT NEEDED
#define FRC_BEHRES_L             0x24
#define FRC_BEHRES_H             0x25
#define FRC_BEVRES_L             0x26
#define FRC_BEVRES_H             0x27
#define FRC_FEVRES_L             0x28
#define FRC_FEVRES_H             0x29
#define FRC_SCLCLK_CTRL          0x2a
#define FRC_SCLCLK_CTRL1         0x2b

#define SCL_IVRES_0              0x60
#define SCL_IVRES_1              0x61
#define SCL_IHRES_0              0x62
#define SCL_IHRES_1              0x63
#define SCL_OVRES_0              0x64
#define SCL_OVRES_1              0x65
#define SCL_OHRES_0              0x66
#define SCL_OHRES_1              0x67
#define SCL_MEMSIZE_0            0x68
#define SCL_MEMSIZE_1            0x69
#define SCL_VRATIO_0             0x6c
#define SCL_VRATIO_1             0x6d
#define SCL_VRATIO_2             0x6e
#define SCL_HRATIO_0             0x70
#define SCL_HRATIO_1             0x71
#define SCL_HRATIO_2             0x72
#define SCL_VWRAP_0              0x74
#define SCL_VWRAP_1              0x75
#define SCL_VINITPH_EVEN         0x76
#define SCL_VINITPH_ODD          0x77
#define SCL_HINITPH              0x78
#define SCL_VCTRL_0              0x79
#define SCL_HCTRL_0              0x7a
#define SCL_CTRL_0               0x7b
#define SCL_NUMTAPS              0x7c
#define SCL_HTAP_OFFSET          0x7d
#define SCL_VTAP_OFFSET          0x7e
#define SCL_STALL                0x7f
#define SCL_INIT_RATIO_0         0x80
#define SCL_INIT_RATIO_1         0x81
#define SCL_INIT_RATIO_2         0x82
#define SCL_INC_RATIO_0          0x84
#define SCL_INC_RATIO_1          0x85
#define SCL_INC_RATIO_2          0x86
#define SCL_NLCRES_0             0x88
#define SCL_NLCRES_1             0x89

#define SCL_COEFF0_L             0x8a
#define SCL_COEFF0_H             0x8b
#define SCL_COEFF1_L             0x8c
#define SCL_COEFF1_H             0x8d
#define SCL_COEFF2_L             0x8e
#define SCL_COEFF2_H             0x8f
#define SCL_COEFF3_L             0x90
#define SCL_COEFF3_H             0x91
#define SCL_COEFF4_L             0x92
#define SCL_COEFF4_H             0x93
#define SCL_COEFF5_L             0x94
#define SCL_COEFF5_H             0x95
#define SCL_COEFF6_L             0x96
#define SCL_COEFF6_H             0x97
#define SCL_COEFF7_L             0x98
#define SCL_COEFF7_H             0x99
#define SCL_COEFF8_L             0x9a
#define SCL_COEFF8_H             0x9b
#define SCL_COEFF9_L             0x9c
#define SCL_COEFF9_H             0x9d
#define SCL_COEFF10_L            0x9e
#define SCL_COEFF10_H            0x9f
#define SCL_COEFF11_L            0xA0
#define SCL_COEFF11_H            0xA1
#define SCL_COEFF_CTRL           0xA2
#define SCL_COEFF_LOAD           0xA3
#define SCL_MEMCTRL              0xA5

#define SCL_C0_L                 0xA6
#define SCL_C0_H                 0xA7
#define SCL_C1_L                 0xA8
#define SCL_C1_H                 0xA9
#define SCL_C2_L                 0xAA
#define SCL_C2_H                 0xAB
#define SCL_C3_L                 0xAC
#define SCL_C3_H                 0xAD
#define SCL_C4_L                 0xAE
#define SCL_C4_H                 0xAF
#define SCL_C5_L                 0xB0
#define SCL_C5_H                 0xB1
#define SCL_C6_L                 0xB2
#define SCL_C6_H                 0xB3
#define SCL_C7_L                 0xB4
#define SCL_C7_H                 0xB5
#define SCL_C8_L                 0xB6
#define SCL_C8_H                 0xB7
#define SCL_A0_L                 0xB8
#define SCL_A0_H                 0xB9
#define SCL_A1_L                 0xBA
#define SCL_A1_H                 0xBB
#define SCL_A2_L                 0xBC
#define SCL_A2_H                 0xBD

#define SCL_ALPH_CTRL            0xBE
#define SCL_ALPH0                0xBF
#define SCL_ALPH1                0xC0
#define SCL_ALPH2                0xC1
#define SCL_ALPH3                0xC2
#define SCL_ALPH4                0xC3

// Video Encoder maddr.def
#define E_CTRL_0                 0x1900 //FF
#define E_CTRL_1                 0x1901 //FF
#define E_CTRL_2                 0x1902 //FF
#define E_CTRL_3                 0x1903 //FF
#define E_CTRL_4                 0x1904 //FF
#define E_CTRL_5                 0x1905 //FF
#define E_CTRL_6                 0x1906 //FF
#define E_CTRL_7                 0x1907 //FF
#define E_CTRL_8                 0x1908 //FF
#define E_CTRL_9                 0x1909 //FF
#define E_CTRL_10                0x190a //FF
#define E_CTRL_11                0x190b //FF
#define E_CTRL_12                0x190c //FF
#define E_CTRL_13                0x190d //FF
#define E_CTRL_14                0x190e //FF
#define E_CTRL_15                0x190f //FF
#define E_CTRL_16                0x1910 //FF
#define E_CTRL_17                0x1911 //FF
#define E_CTRL_18                0x1912 //FF
#define E_CTRL_19                0x1913 //FF
#define E_CTRL_20                0x1914 //FF
#define E_CTRL_21                0x1915 //FF
#define E_CTRL_22                0x1916 //FF
#define E_CTRL_23                0x1917 //FF
#define E_CTRL_24                0x1918 //FF
#define E_CTRL_25                0x1919 //FF
#define E_CTRL_26                0x191a //FF
#define E_CTRL_27                0x191b //FF
#define E_CTRL_28                0x191c //FF
#define E_CTRL_29                0x191d //FF
#define E_CTRL_30                0x191e //FF
#define E_CTRL_31                0x191f //FF
#define E_CTRL_32                0x1920 //FF
#define E_CTRL_33                0x1921 //FF
#define E_CTRL_34                0x1922 //FF
#define E_CTRL_35                0x1923 //FF
#define E_CTRL_36                0x1924 //FF
#define E_CTRL_37                0x1925 //FF
#define E_CTRL_38                0x1926 //FF
#define E_CTRL_39                0x1927 //FF
#define E_CTRL_40                0x1928 //FF
#define E_CTRL_41                0x1929 //FF
#define E_CTRL_42                0x192a //FF
#define E_CTRL_43                0x192b //FF
#define E_CTRL_44                0x192c //FF
#define E_CTRL_45                0x192d //FF
#define E_CTRL_46                0x192e //FF
#define E_CTRL_47                0x192f //FF
#define E_CTRL_48                0x1930 //FF
#define E_CTRL_49                0x1931 //FF
#define E_CTRL_50                0x1932 //FF
#define E_CTRL_51                0x1933 //FF
#define E_CTRL_52                0x1934 //FF
#define E_CTRL_53                0x1935 //FF
#define E_CTRL_54                0x1936 //FF
#define E_CTRL_55                0x1937 //FF
#define E_CTRL_56                0x1938 //FF
#define E_CTRL_57                0x1939 //FF
#define E_CTRL_58                0x193a //FF
#define E_CTRL_59                0x193b //FF
#define E_CTRL_60                0x193c //FF
#define E_CTRL_61                0x193d //FF
#define E_CTRL_62                0x193e //FF
#define E_CTRL_63                0x193f //FF
#define E_CTRL_64                0x1940 //FF
#define E_CTRL_65                0x1941 //FF
#define E_CTRL_66                0x1942 //FF
#define E_CTRL_67                0x1943 //FF
#define E_CTRL_68                0x1944 //FF
#define E_CTRL_69                0x1945 //FF
#define E_CTRL_70                0x1946 //FF
#define E_CTRL_71                0x1947 //FF
#define E_CTRL_72                0x1948 //FF
#define E_CTRL_73                0x1949 //FF
#define E_CTRL_74                0x194a //FF
#define E_CTRL_75                0x194b //FF
#define E_CTRL_76                0x194c //FF
#define E_CTRL_77                0x194d //FF
#define E_CTRL_78                0x194e //FF
#define E_CTRL_79                0x194f //FF
#define E_CTRL_80                0x1950 //FF
#define E_CTRL_81                0x1951 //FF
#define E_CTRL_82                0x1952 //FF
#define E_CTRL_83                0x1953 //FF
#define E_CTRL_84                0x1954 //FF
#define E_CTRL_85                0x1955 //FF
#define E_CTRL_86                0x1956 //FF
#define E_CTRL_87                0x1957 //FF
#define E_CTRL_88                0x1958 //FF
#define E_CTRL_89                0x1959 //FF
#define E_CTRL_90                0x195a
#define E_CTRL_91                0x195b //FF
#define E_CTRL_92                0x195c //FF
#define E_CTRL_93                0x195d //FF
#define E_CTRL_94                0x195e //FF
#define E_CTRL_95                0x195f //FF
#define E_CTRL_96                0x1960 //FF
#define E_CTRL_97                0x1961 //FF
#define E_CTRL_98                0x1962 //FF
#define E_CTRL_99                0x1963 //FF
#define E_CTRL_100               0x1964 //FF
#define E_CTRL_101               0x1965 //FF
#define E_CTRL_102               0x1966 //FF
#define E_CTRL_103               0x1967 //FF
#define E_CTRL_104               0x1968
#define E_CTRL_105               0x1969 //FF
#define E_CTRL_106               0x196a //FF
#define E_CTRL_107               0x196b //FF
#define E_CTRL_108               0x196c //FF
#define E_CTRL_109               0x196d //FF
#define E_CTRL_110               0x196e //FF
#define E_CTRL_111               0x196f //FF
#define E_CTRL_112               0x1970 //FF
#define E_CTRL_113               0x1971 //FF
#define E_CTRL_114               0x1972 //FF
#define E_CTRL_115               0x1973 //FF
#define E_CTRL_116               0x1974 //FF
#define E_CTRL_117               0x1975 //FF
#define E_CTRL_118               0x1976 //FF
#define E_CTRL_119               0x1977 //FF
#define E_CTRL_120               0x1978 //FF
#define E_CTRL_121               0x1979 //FF
#define E_CTRL_122               0x197a //FF
#define E_CTRL_123               0x197b //FF
#define E_CTRL_124               0x197c //FF
#define E_CTRL_125               0x197d //FF
#define E_CTRL_126               0x197e //FF
#define E_CTRL_127               0x197f //FF
#define E_CTRL_128               0x1980 //FF
#define E_CTRL_129               0x1981 //FF
#define E_CTRL_130               0x1982 //FF
#define E_CTRL_131               0x1983 //FF
#define E_CTRL_132               0x1984 //FF
#define E_CTRL_133               0x1985 //FF
#define E_CTRL_134               0x1986 //FF
#define E_CTRL_135               0x1987 //FF
#define E_CTRL_136               0x1988 //FF
#define E_CTRL_137               0x1989 //FF
#define E_CTRL_138               0x198a //FF
#define E_CTRL_139               0x198b //FF
#define E_CTRL_140               0x198c //FF
#define E_CTRL_141               0x198d //FF
#define E_CTRL_142               0x198e //FF
#define E_CTRL_143               0x198f //FF
#define E_CTRL_144               0x1990 //FF
#define E_CTRL_145               0x1991 //FF
#define E_CTRL_146               0x1992 //FF
#define E_CTRL_147               0x1993 //FF
#define E_CTRL_148               0x1994 //FF
#define E_CTRL_149               0x1995 //FF
#define E_CTRL_150               0x1996 //FF
#define E_CTRL_151               0x1997 //FF
#define E_CTRL_152               0x1998 //FF
#define E_CTRL_153               0x1999 //FF
#define E_CTRL_154               0x199a //FF
#define E_CTRL_155               0x199b //FF
#define E_CTRL_156               0x199c //FF
#define E_CTRL_157               0x199d //FF
#define E_CTRL_158               0x199e //FF
#define E_CTRL_159               0x199f //FF
#define E_CTRL_160               0x19a0 //FF
#define E_CTRL_161               0x19a1 //FF
#define E_CTRL_162               0x19a2 //FF
#define E_CTRL_163               0x19a3 //FF
#define E_CTRL_164               0x19a4 //FF
#define E_CTRL_165               0x19a5 //FF
#define E_CTRL_166               0x19a6 //FF
#define E_CTRL_167               0x19a7 //FF
#define E_CTRL_168               0x19a8 //FF
#define E_CTRL_169               0x19a9 //FF
#define E_CTRL_170               0x19aa //FF
#define E_CTRL_171               0x19ab //FF
#define E_CTRL_172               0x19ac //FF
#define E_CTRL_173               0x19ad //FF
#define E_CTRL_174               0x19ae //FF
#define E_CTRL_175               0x19af //FF
#define E_CTRL_176               0x19b0 //FF
#define E_CTRL_177               0x19b1 //FF
#define E_CTRL_178               0x19b2
#define E_CTRL_179               0x19b3 //FF
#define E_CTRL_180               0x19b4 //FF
#define E_CTRL_181               0x19b5
#define E_CTRL_182               0x19b6
#define E_CTRL_183               0x19b7
#define E_CTRL_184               0x19b8
#define E_CTRL_185               0x19b9
#define E_CTRL_186               0x19ba
#define E_CTRL_187               0x19bb
#define E_CTRL_188               0x19bc
#define E_CTRL_189               0x19bd
#define E_CTRL_190               0x19be
#define E_CTRL_191               0x19bf
#define E_CTRL_192               0x19c0
#define E_CTRL_193               0x19c1
#define E_CTRL_194               0x19c2
#define E_CTRL_195               0x19c3
#define E_CTRL_196               0x19c4
#define E_CTRL_197               0x19c5
#define E_CTRL_198               0x19c6
#define E_CTRL_199               0x19c7
#define E_CTRL_200               0x19c8
#define E_CTRL_201               0x19c9
#define E_CTRL_202               0x19ca
#define E_CTRL_203               0x19cb
#define E_CTRL_204               0x19cc
#define E_CTRL_205               0x19cd
#define E_CTRL_206               0x19ce
#define E_CTRL_207               0x19cf
#define E_CTRL_208               0x19d0
#define E_CTRL_209               0x19d1
#define E_CTRL_210               0x19d2
#define E_CTRL_211               0x19d3
#define E_CTRL_212               0x19d4
#define E_CTRL_213               0x19d5
#define E_CTRL_214               0x19d6
#define E_CTRL_215               0x19d7
#define E_CTRL_216               0x19d8
#define E_CTRL_217               0x19d9
#define E_CTRL_218               0x19da
#define E_CTRL_219               0x19db
#define E_CTRL_220               0x19dc
#define E_CTRL_221               0x19dd
#define E_CTRL_222               0x19de
#define E_CTRL_223               0x19df
#define E_CTRL_224               0x19e0
#define E_CTRL_225               0x19e1
#define E_CTRL_226               0x19e2
#define E_CTRL_227               0x19e3
#define E_CTRL_228               0x19e4
#define E_CTRL_229               0x19e5
#define E_CTRL_230               0x19e6
#define E_CTRL_231               0x19e7
#define E_CTRL_232               0x19e8
#define E_CTRL_233               0x19e9
#define E_CTRL_234               0x19ea
#define E_CTRL_235               0x19eb
#define E_CTRL_236               0x19ec

// HDMI-TX registers
//-----------AUDIO---------------
#define HDTX_ACR_N0            0x1000 //FF
#define HDTX_ACR_N1            0x1001 //FF
#define HDTX_ACR_N2            0x1002 //0F
#define HDTX_ACR_CTS0          0x1004 //FF
#define HDTX_ACR_CTS1          0x1005 //FF
#define HDTX_ACR_CTS2          0x1006 //0F
#define HDTX_ACR_CTRL          0x1007 //0F
#define HDTX_ACR_STS0          0x1008
#define HDTX_ACR_STS1          0x1009
#define HDTX_ACR_STS2          0x100A
#define HDTX_AUD_CTRL          0x100B //FF
#define HDTX_I2S_CTRL          0x100C //1F
#define HDTX_I2S_DLEN          0x100D //1F
#define HDTX_I2S_DBG_LFT0      0x1010 //FF
#define HDTX_I2S_DBG_LFT1      0x1011 //FF
#define HDTX_I2S_DBG_LFT2      0x1012 //FF
#define HDTX_I2S_DBG_LFT3      0x1013 //FF
#define HDTX_I2S_DBG_RIT0      0x1014 //FF
#define HDTX_I2S_DBG_RIT1      0x1015 //FF
#define HDTX_I2S_DBG_RIT2      0x1016 //FF
#define HDTX_I2S_DBG_RIT3      0x1017 //FF
#define HDTX_CHSTS_0           0x1018 //FF
#define HDTX_CHSTS_1           0x1019 //FF
#define HDTX_CHSTS_2           0x101A //FF
#define HDTX_CHSTS_3           0x101B //FF
#define HDTX_CHSTS_4           0x101C //FF
#define HDTX_FIFO_CTRL         0x101D //03
#define HDTX_MEMSIZE_L         0x101E //FF
#define HDTX_MEMSIZE_H         0x101F //01
#define HDTX_GCP_CFG0          0x1020 //0F
#define HDTX_GCP_CFG1          0x1021 //FF
#define HDTX_AUD_STS           0x1022
//-----------VIDEO --------------
#define HDTX_HTOT_L            0x1024 //FF
#define HDTX_HTOT_H            0x1025 //FF
#define HDTX_HBLANK_L          0x1026 //FF
#define HDTX_HBLANK_H          0x1027 //FF
#define HDTX_VTOT_L            0x1028 //FF
#define HDTX_VTOT_H            0x1029 //FF
#define HDTX_VRES_L            0x102A //FF
#define HDTX_VRES_H            0x102B //FF
#define HDTX_VSTART_L          0x102C //FF
#define HDTX_VSTART_H          0x102D //FF
#define HDTX_HTOT_STS_L        0x102E
#define HDTX_HTOT_STS_H        0x102F
#define HDTX_HBLANK_STS_L      0x1030
#define HDTX_HBLANK_STS_H      0x1031
#define HDTX_VTOT_STS_L        0x1032
#define HDTX_VTOT_STS_H        0x1033
#define HDTX_VRES_STS_L        0x1034
#define HDTX_VRES_STS_H        0x1035
#define HDTX_VSTART_STS_L      0x1036
#define HDTX_VSTART_STS_H      0x1037
#define HDTX_VIDEO_STS         0x1038
#define HDTX_VIDEO_CTRL        0x1039 //7F
#define HDTX_HDMI_CTRL         0x103A //FF
//-----------------------------
// DDC - NOT USED -CAN BE REMOVED
//------_-----------------------
//#define HDMI_EDDC_CTRL         0x103B //NOT NEEDED
//#define HDMI_EDDC_DEV_ID       0x103C //NOT NEEDED
//#define HDMI_EDDC_DEV_OFF      0x103D //NOT NEEDED
//#define HDMI_EDDC_SEG_ADDR     0x103E //NOT NEEDED
//#define HDMI_EDDC_CMD          0x103F //NOT NEEDED
//#define HDMI_EDDC_RW_CNT_L     0x1040 //NOT NEEDED
//#define HDMI_EDDC_RW_CNT_H     0x1041 //NOT NEEDED
//#define HDMI_EDDC_PERIOD       0x1042 //NOT NEEDED
//#define HDMI_EDDC_RW_DATA      0x1043 //NOT NEEDED
//#define HDMI_EDDC_STATUS       0x1044 //NOT NEEDED
//#define HDMI_EDDC_SEG_PTR      0x1045 //NOT NEEDED

#define HDTX_PP_HW             0x1046

//-----------------------------
// DEEP COLOR
//------_-----------------------
#define HDTX_DC_FIFO_SFT_RST   0x1047 //01
#define HDTX_DC_FIFO_WR_PTR    0x1048 //3F
#define HDTX_DC_FIFO_RD_PTR    0x1049 //3F
#define HDTX_DC_PP_CTRL        0x104A //01

//-------------------------------
// HDMI_TX PHY GLUE LOGIC
//-------------------------------
#define HDTX_TDATA0_0          0x104C //FF
#define HDTX_TDATA0_1          0x104D //FF
#define HDTX_TDATA0_2          0x104E //0F
#define HDTX_TDATA1_0          0x1050 //FF
#define HDTX_TDATA1_1          0x1051 //FF
#define HDTX_TDATA1_2          0x1052 //0F
#define HDTX_TDATA2_0          0x1054 //FF
#define HDTX_TDATA2_1          0x1055 //FF
#define HDTX_TDATA2_2          0x1056 //0F
#define HDTX_TDATA3_0          0x1058 //FF
#define HDTX_TDATA3_1          0x1059 //FF
#define HDTX_TDATA3_2          0x105A //0F
#define HDTX_TDATA_SEL         0x105B //7F
#define HDTX_SWAP_CTRL         0x105C //03

#define HDTX_AVMUTE_CTRL       0x105D //03

#define HDTX_HST_PKT_CTRL0     0x105E //3F
#define HDTX_HST_PKT_CTRL1     0x105F //3F
//-----------------------------
#define HDTX_PKT0_BYTE0        0x1060 //FF
#define HDTX_PKT0_BYTE1        0x1061 //FF
#define HDTX_PKT0_BYTE2        0x1062 //FF
#define HDTX_PKT0_BYTE3        0x1063 //FF
#define HDTX_PKT0_BYTE4        0x1064 //FF
#define HDTX_PKT0_BYTE5        0x1065 //FF
#define HDTX_PKT0_BYTE6        0x1066 //FF
#define HDTX_PKT0_BYTE7        0x1067 //FF
#define HDTX_PKT0_BYTE8        0x1068 //FF
#define HDTX_PKT0_BYTE9        0x1069 //FF
#define HDTX_PKT0_BYTE10       0x106A //FF
#define HDTX_PKT0_BYTE11       0x106B //FF
#define HDTX_PKT0_BYTE12       0x106C //FF
#define HDTX_PKT0_BYTE13       0x106D //FF
#define HDTX_PKT0_BYTE14       0x106E //FF
#define HDTX_PKT0_BYTE15       0x106F //FF
#define HDTX_PKT0_BYTE16       0x1070 //FF
#define HDTX_PKT0_BYTE17       0x1071 //FF
#define HDTX_PKT0_BYTE18       0x1072 //FF
#define HDTX_PKT0_BYTE19       0x1073 //FF
#define HDTX_PKT0_BYTE20       0x1074 //FF
#define HDTX_PKT0_BYTE21       0x1075 //FF
#define HDTX_PKT0_BYTE22       0x1076 //FF
#define HDTX_PKT0_BYTE23       0x1077 //FF
#define HDTX_PKT0_BYTE24       0x1078 //FF
#define HDTX_PKT0_BYTE25       0x1079 //FF
#define HDTX_PKT0_BYTE26       0x107A //FF
#define HDTX_PKT0_BYTE27       0x107B //FF
#define HDTX_PKT0_BYTE28       0x107C //FF
#define HDTX_PKT0_BYTE29       0x107D //FF
#define HDTX_PKT0_BYTE30       0x107E //FF
//-----------------------------
#define HDTX_PKT1_BYTE0        0x1080 //FF
#define HDTX_PKT1_BYTE1        0x1081 //FF
#define HDTX_PKT1_BYTE2        0x1082 //FF
#define HDTX_PKT1_BYTE3        0x1083 //FF
#define HDTX_PKT1_BYTE4        0x1084 //FF
#define HDTX_PKT1_BYTE5        0x1085 //FF
#define HDTX_PKT1_BYTE6        0x1086 //FF
#define HDTX_PKT1_BYTE7        0x1087 //FF
#define HDTX_PKT1_BYTE8        0x1088 //FF
#define HDTX_PKT1_BYTE9        0x1089 //FF
#define HDTX_PKT1_BYTE10       0x108A //FF
#define HDTX_PKT1_BYTE11       0x108B //FF
#define HDTX_PKT1_BYTE12       0x108C //FF
#define HDTX_PKT1_BYTE13       0x108D //FF
#define HDTX_PKT1_BYTE14       0x108E //FF
#define HDTX_PKT1_BYTE15       0x108F //FF
#define HDTX_PKT1_BYTE16       0x1090 //FF
#define HDTX_PKT1_BYTE17       0x1091 //FF
#define HDTX_PKT1_BYTE18       0x1092 //FF
#define HDTX_PKT1_BYTE19       0x1093 //FF
#define HDTX_PKT1_BYTE20       0x1094 //FF
#define HDTX_PKT1_BYTE21       0x1095 //FF
#define HDTX_PKT1_BYTE22       0x1096 //FF
#define HDTX_PKT1_BYTE23       0x1097 //FF
#define HDTX_PKT1_BYTE24       0x1098 //FF
#define HDTX_PKT1_BYTE25       0x1099 //FF
#define HDTX_PKT1_BYTE26       0x109A //FF
#define HDTX_PKT1_BYTE27       0x109B //FF
#define HDTX_PKT1_BYTE28       0x109C //FF
#define HDTX_PKT1_BYTE29       0x109D //FF
#define HDTX_PKT1_BYTE30       0x109E //FF
//-----------------------------
#define HDTX_PKT2_BYTE0        0x10A0 //FF
#define HDTX_PKT2_BYTE1        0x10A1 //FF
#define HDTX_PKT2_BYTE2        0x10A2 //FF
#define HDTX_PKT2_BYTE3        0x10A3 //FF
#define HDTX_PKT2_BYTE4        0x10A4 //FF
#define HDTX_PKT2_BYTE5        0x10A5 //FF
#define HDTX_PKT2_BYTE6        0x10A6 //FF
#define HDTX_PKT2_BYTE7        0x10A7 //FF
#define HDTX_PKT2_BYTE8        0x10A8 //FF
#define HDTX_PKT2_BYTE9        0x10A9 //FF
#define HDTX_PKT2_BYTE10       0x10AA //FF
#define HDTX_PKT2_BYTE11       0x10AB //FF
#define HDTX_PKT2_BYTE12       0x10AC //FF
#define HDTX_PKT2_BYTE13       0x10AD //FF
#define HDTX_PKT2_BYTE14       0x10AE //FF
#define HDTX_PKT2_BYTE15       0x10AF //FF
#define HDTX_PKT2_BYTE16       0x10B0 //FF
#define HDTX_PKT2_BYTE17       0x10B1 //FF
#define HDTX_PKT2_BYTE18       0x10B2 //FF
#define HDTX_PKT2_BYTE19       0x10B3 //FF
#define HDTX_PKT2_BYTE20       0x10B4 //FF
#define HDTX_PKT2_BYTE21       0x10B5 //FF
#define HDTX_PKT2_BYTE22       0x10B6 //FF
#define HDTX_PKT2_BYTE23       0x10B7 //FF
#define HDTX_PKT2_BYTE24       0x10B8 //FF
#define HDTX_PKT2_BYTE25       0x10B9 //FF
#define HDTX_PKT2_BYTE26       0x10BA //FF
#define HDTX_PKT2_BYTE27       0x10BB //FF
#define HDTX_PKT2_BYTE28       0x10BC //FF
#define HDTX_PKT2_BYTE29       0x10BD //FF
#define HDTX_PKT2_BYTE30       0x10BE //FF
//-----------------------------
#define HDTX_PKT3_BYTE0        0x10C0 //FF
#define HDTX_PKT3_BYTE1        0x10C1 //FF
#define HDTX_PKT3_BYTE2        0x10C2 //FF
#define HDTX_PKT3_BYTE3        0x10C3 //FF
#define HDTX_PKT3_BYTE4        0x10C4 //FF
#define HDTX_PKT3_BYTE5        0x10C5 //FF
#define HDTX_PKT3_BYTE6        0x10C6 //FF
#define HDTX_PKT3_BYTE7        0x10C7 //FF
#define HDTX_PKT3_BYTE8        0x10C8 //FF
#define HDTX_PKT3_BYTE9        0x10C9 //FF
#define HDTX_PKT3_BYTE10       0x10CA //FF
#define HDTX_PKT3_BYTE11       0x10CB //FF
#define HDTX_PKT3_BYTE12       0x10CC //FF
#define HDTX_PKT3_BYTE13       0x10CD //FF
#define HDTX_PKT3_BYTE14       0x10CE //FF
#define HDTX_PKT3_BYTE15       0x10CF //FF
#define HDTX_PKT3_BYTE16       0x10D0 //FF
#define HDTX_PKT3_BYTE17       0x10D1 //FF
#define HDTX_PKT3_BYTE18       0x10D2 //FF
#define HDTX_PKT3_BYTE19       0x10D3 //FF
#define HDTX_PKT3_BYTE20       0x10D4 //FF
#define HDTX_PKT3_BYTE21       0x10D5 //FF
#define HDTX_PKT3_BYTE22       0x10D6 //FF
#define HDTX_PKT3_BYTE23       0x10D7 //FF
#define HDTX_PKT3_BYTE24       0x10D8 //FF
#define HDTX_PKT3_BYTE25       0x10D9 //FF
#define HDTX_PKT3_BYTE26       0x10DA //FF
#define HDTX_PKT3_BYTE27       0x10DB //FF
#define HDTX_PKT3_BYTE28       0x10DC //FF
#define HDTX_PKT3_BYTE29       0x10DD //FF
#define HDTX_PKT3_BYTE30       0x10DE //FF

//-----------------------------
#define HDTX_PKT4_BYTE0        0x10E0 //FF
#define HDTX_PKT4_BYTE1        0x10E1 //FF
#define HDTX_PKT4_BYTE2        0x10E2 //FF
#define HDTX_PKT4_BYTE3        0x10E3 //FF
#define HDTX_PKT4_BYTE4        0x10E4 //FF
#define HDTX_PKT4_BYTE5        0x10E5 //FF
#define HDTX_PKT4_BYTE6        0x10E6 //FF
#define HDTX_PKT4_BYTE7        0x10E7 //FF
#define HDTX_PKT4_BYTE8        0x10E8 //FF
#define HDTX_PKT4_BYTE9        0x10E9 //FF
#define HDTX_PKT4_BYTE10       0x10EA //FF
#define HDTX_PKT4_BYTE11       0x10EB //FF
#define HDTX_PKT4_BYTE12       0x10EC //FF
#define HDTX_PKT4_BYTE13       0x10ED //FF
#define HDTX_PKT4_BYTE14       0x10EE //FF
#define HDTX_PKT4_BYTE15       0x10EF //FF
#define HDTX_PKT4_BYTE16       0x10F0 //FF
#define HDTX_PKT4_BYTE17       0x10F1 //FF
#define HDTX_PKT4_BYTE18       0x10F2 //FF
#define HDTX_PKT4_BYTE19       0x10F3 //FF
#define HDTX_PKT4_BYTE20       0x10F4 //FF
#define HDTX_PKT4_BYTE21       0x10F5 //FF
#define HDTX_PKT4_BYTE22       0x10F6 //FF
#define HDTX_PKT4_BYTE23       0x10F7 //FF
#define HDTX_PKT4_BYTE24       0x10F8 //FF
#define HDTX_PKT4_BYTE25       0x10F9 //FF
#define HDTX_PKT4_BYTE26       0x10FA //FF
#define HDTX_PKT4_BYTE27       0x10FB //FF
#define HDTX_PKT4_BYTE28       0x10FC //FF
#define HDTX_PKT4_BYTE29       0x10FD //FF
#define HDTX_PKT4_BYTE30       0x10FE //FF
//-----------------------------

#define HDTX_PKT5_BYTE0        0x1100 //FF
#define HDTX_PKT5_BYTE1        0x1101 //FF
#define HDTX_PKT5_BYTE2        0x1102 //FF
#define HDTX_PKT5_BYTE3        0x1103 //FF
#define HDTX_PKT5_BYTE4        0x1104 //FF
#define HDTX_PKT5_BYTE5        0x1105 //FF
#define HDTX_PKT5_BYTE6        0x1106 //FF
#define HDTX_PKT5_BYTE7        0x1107 //FF
#define HDTX_PKT5_BYTE8        0x1108 //FF
#define HDTX_PKT5_BYTE9        0x1109 //FF
#define HDTX_PKT5_BYTE10       0x110A //FF
#define HDTX_PKT5_BYTE11       0x110B //FF
#define HDTX_PKT5_BYTE12       0x110C //FF
#define HDTX_PKT5_BYTE13       0x110D //FF
#define HDTX_PKT5_BYTE14       0x110E //FF
#define HDTX_PKT5_BYTE15       0x110F //FF
#define HDTX_PKT5_BYTE16       0x1110 //FF
#define HDTX_PKT5_BYTE17       0x1111 //FF
#define HDTX_PKT5_BYTE18       0x1112 //FF
#define HDTX_PKT5_BYTE19       0x1113 //FF
#define HDTX_PKT5_BYTE20       0x1114 //FF
#define HDTX_PKT5_BYTE21       0x1115 //FF
#define HDTX_PKT5_BYTE22       0x1116 //FF
#define HDTX_PKT5_BYTE23       0x1117 //FF
#define HDTX_PKT5_BYTE24       0x1118 //FF
#define HDTX_PKT5_BYTE25       0x1119 //FF
#define HDTX_PKT5_BYTE26       0x111A //FF
#define HDTX_PKT5_BYTE27       0x111B //FF
#define HDTX_PKT5_BYTE28       0x111C //FF
#define HDTX_PKT5_BYTE29       0x111D //FF
#define HDTX_PKT5_BYTE30       0x111E //FF
#define HDTX_RSVD              0x111F

//-----------------------------
// NEW HDMI-TX REGISTERS
//-----------------------------
#define HDTX_UBITS_0           0x1120 //FF
#define HDTX_UBITS_1           0x1121 //FF
#define HDTX_UBITS_2           0x1122 //FF
#define HDTX_UBITS_3           0x1123 //FF
#define HDTX_UBITS_4           0x1124 //FF
#define HDTX_UBITS_5           0x1125 //FF
#define HDTX_UBITS_6           0x1126 //FF
#define HDTX_UBITS_7           0x1127 //FF
#define HDTX_UBITS_8           0x1128 //FF
#define HDTX_UBITS_9           0x1129 //FF
#define HDTX_UBITS_10          0x112A //FF
#define HDTX_UBITS_11          0x112B //FF
#define HDTX_UBITS_12          0x112C //FF
#define HDTX_UBITS_13          0x112D //0F
#define HDTX_HBR_PKT           0x112E //01

#define HDTX_PHY_FIFO_SOFT_RST 0x1130 //01
#define HDTX_PHY_FIFO_PTRS     0x1131 //FF
#define HDTX_PRBS_CTRL0        0x1132 //07
#define HDTX_HST_PKT_CTRL2     0x1133 //01
#define HDTX_HST_PKT_START     0x1134 //01
#define TX_HDCP_FRM_CNT        0x1135
#define TX_HDCP_HPD_STS        0x1136
#define HDTX_AUD_CH1_SEL       0x1137 //07
#define HDTX_AUD_CH2_SEL       0x1138 //07
#define HDTX_AUD_CH3_SEL       0x1139 //07
#define HDTX_AUD_CH4_SEL       0x113A //07
#define HDTX_AUD_CH5_SEL       0x113B //07
#define HDTX_AUD_CH6_SEL       0x113C //07
#define HDTX_AUD_CH7_SEL       0x113D //07
#define HDTX_AUD_CH8_SEL       0x113E //07
#define TX_HDCP_HW_STATUS      0x113F
#define TX_HDCP_HPD_SEL        0x1140 //01
#define TX_HDCP_SW_HPD         0x1141 //01
//
#define HDTX_IN_FIX_CLR_SEL    0x1147 //01
#define HDTX_IN_FIX_CLR_CH0_L  0x1148 //FF
#define HDTX_IN_FIX_CLR_CH0_H  0x1149 //0F
#define HDTX_IN_FIX_CLR_CH1_L  0x114A //FF
#define HDTX_IN_FIX_CLR_CH1_H  0x114B //0F
#define HDTX_IN_FIX_CLR_CH2_L  0x114C //FF
#define HDTX_IN_FIX_CLR_CH2_H  0x114D //0F

// --  CEC

#define CEC_BASE 0x0 // CEC is now part of SM in BG2

#define CEC_TOGGLE_FOR_WRITE_REG_ADDR        CEC_BASE + 0x0000
#define CEC_TOGGLE_FOR_READ_REG_ADDR         CEC_BASE + 0x0004
#define CEC_RDY_ADDR                         CEC_BASE + 0x0008 //01
#define CEC_RX_RDY_ADDR                      CEC_BASE + 0x000c //01
#define CEC_TX_FIFO_RESET_ADDR               CEC_BASE + 0x0010 //03
#define CEC_RX_FIFO_RESET_ADDR               CEC_BASE + 0x0014 //03
#define CEC_PMODE_ADDR                       CEC_BASE + 0x0018 //01
//#define CEC_TX_RDY_ADDR                      CEC_BASE + 0x001c //01
#define CEC_TX_TYPE_ADDR                     CEC_BASE + 0x0020 //01
//#define CEC_TX_RESP_TIME_0_ADDR              CEC_BASE + 0x0024 //FF
//#define CEC_TX_RESP_TIME_1_ADDR              CEC_BASE + 0x0025 //FF
//#define CEC_TX_RESP_TIME_2_ADDR              CEC_BASE + 0x0026 //FF
//#define CEC_TX_RESP_TIME_3_ADDR              CEC_BASE + 0x0027 //FF
#define CEC_SIGNAL_FREE_TIME_0_ADDR          CEC_BASE + 0x0028 //FF
#define CEC_SIGNAL_FREE_TIME_1_ADDR          CEC_BASE + 0x0029 //FF
#define CEC_SIGNAL_FREE_TIME_2_ADDR          CEC_BASE + 0x002a //FF
#define CEC_SIGNAL_FREE_TIME_3_ADDR          CEC_BASE + 0x002b //FF
#define CEC_START_BIT_LO_THRESH_0_ADDR       CEC_BASE + 0x002c //FF
#define CEC_START_BIT_LO_THRESH_1_ADDR       CEC_BASE + 0x002d //FF
#define CEC_START_BIT_LO_THRESH_2_ADDR       CEC_BASE + 0x002e //FF
#define CEC_START_BIT_LO_THRESH_3_ADDR       CEC_BASE + 0x002f //FF
#define CEC_START_BIT_HI_THRESH_0_ADDR       CEC_BASE + 0x0030 //FF
#define CEC_START_BIT_HI_THRESH_1_ADDR       CEC_BASE + 0x0031 //FF
#define CEC_START_BIT_HI_THRESH_2_ADDR       CEC_BASE + 0x0032 //FF
#define CEC_START_BIT_HI_THRESH_3_ADDR       CEC_BASE + 0x0033 //FF
#define CEC_DATA_BIT_0_LO_THRESH_0_ADDR      CEC_BASE + 0x0034 //FF
#define CEC_DATA_BIT_0_LO_THRESH_1_ADDR      CEC_BASE + 0x0035 //FF
#define CEC_DATA_BIT_0_LO_THRESH_2_ADDR      CEC_BASE + 0x0036 //FF
#define CEC_DATA_BIT_0_LO_THRESH_3_ADDR      CEC_BASE + 0x0037 //FF
#define CEC_DATA_BIT_1_LO_THRESH_0_ADDR      CEC_BASE + 0x0038 //FF
#define CEC_DATA_BIT_1_LO_THRESH_1_ADDR      CEC_BASE + 0x0039 //FF
#define CEC_DATA_BIT_1_LO_THRESH_2_ADDR      CEC_BASE + 0x003a //FF
#define CEC_DATA_BIT_1_LO_THRESH_3_ADDR      CEC_BASE + 0x003b //FF
#define CEC_DATA_BIT_0_HI_THRESH_0_ADDR      CEC_BASE + 0x003c //FF
#define CEC_DATA_BIT_0_HI_THRESH_1_ADDR      CEC_BASE + 0x003d //FF
#define CEC_DATA_BIT_0_HI_THRESH_2_ADDR      CEC_BASE + 0x003e //FF
#define CEC_DATA_BIT_0_HI_THRESH_3_ADDR      CEC_BASE + 0x003f //FF
#define CEC_DATA_BIT_1_HI_THRESH_0_ADDR      CEC_BASE + 0x0040 //FF
#define CEC_DATA_BIT_1_HI_THRESH_1_ADDR      CEC_BASE + 0x0041 //FF
#define CEC_DATA_BIT_1_HI_THRESH_2_ADDR      CEC_BASE + 0x0042 //FF
#define CEC_DATA_BIT_1_HI_THRESH_3_ADDR      CEC_BASE + 0x0043 //FF
#define CEC_SSP_ACK_TIME_0_ADDR              CEC_BASE + 0x0044 //FF
#define CEC_SSP_ACK_TIME_1_ADDR              CEC_BASE + 0x0045 //FF
#define CEC_SSP_ACK_TIME_2_ADDR              CEC_BASE + 0x0046 //FF
#define CEC_SSP_ACK_TIME_3_ADDR              CEC_BASE + 0x0047 //FF
#define CEC_INTR_ENABLE0_REG_ADDR            CEC_BASE + 0x0048 //FF
#define CEC_INTR_ENABLE1_REG_ADDR            CEC_BASE + 0x0049 //FF
#define CEC_DATA_REG_ADDR                    CEC_BASE + 0x004c //FF
#define CEC_EOM_REG_ADDR                     CEC_BASE + 0x0050 //01
#define CEC_INTR_STATUS0_REG_ADDR            CEC_BASE + 0x0058
#define CEC_INTR_STATUS1_REG_ADDR            CEC_BASE + 0x0059
#define CEC_NOMINAL_SAMPLE_TIME_0_ADDR       CEC_BASE + 0x005c //FF
#define CEC_NOMINAL_SAMPLE_TIME_1_ADDR       CEC_BASE + 0x005d //FF
#define CEC_NOMINAL_SAMPLE_TIME_2_ADDR       CEC_BASE + 0x005e //FF
#define CEC_NOMINAL_SAMPLE_TIME_3_ADDR       CEC_BASE + 0x005f //FF
#define CEC_HYST_TIME_0_ADDR                 CEC_BASE + 0x0060 //FF
#define CEC_HYST_TIME_1_ADDR                 CEC_BASE + 0x0061 //FF
#define CEC_HYST_TIME_2_ADDR                 CEC_BASE + 0x0062 //FF
#define CEC_HYST_TIME_3_ADDR                 CEC_BASE + 0x0063 //FF
#define CEC_FOLLOWER_ACK_TIME_0_ADDR         CEC_BASE + 0x0064 //FF
#define CEC_FOLLOWER_ACK_TIME_1_ADDR         CEC_BASE + 0x0065 //FF
#define CEC_FOLLOWER_ACK_TIME_2_ADDR         CEC_BASE + 0x0066 //FF
#define CEC_FOLLOWER_ACK_TIME_3_ADDR         CEC_BASE + 0x0067 //FF
#define CEC_RX_BUF_READ_REG_ADDR             CEC_BASE + 0x0068
#define CEC_RX_EOM_READ_REG_ADDR             CEC_BASE + 0x0069
#define CEC_LOGICAL_ADDR0_REG_ADDR           CEC_BASE + 0x006a //1F
#define CEC_LOGICAL_ADDR1_REG_ADDR           CEC_BASE + 0x006b //1F
#define CEC_LOGICAL_ADDR2_REG_ADDR           CEC_BASE + 0x006c //1F
#define CEC_LOGICAL_ADDR3_REG_ADDR           CEC_BASE + 0x006d //1F
#define CEC_LOGICAL_ADDR4_REG_ADDR           CEC_BASE + 0x006e //1F
#define CEC_JITTER_CNT_0_ADDR                CEC_BASE + 0x0070 //FF
#define CEC_JITTER_CNT_1_ADDR                CEC_BASE + 0x0071 //FF
#define CEC_JITTER_CNT_2_ADDR                CEC_BASE + 0x0072 //FF
#define CEC_JITTER_CNT_3_ADDR                CEC_BASE + 0x0073 //FF
#define CEC_LINE_STATUS_REG_ADDR             CEC_BASE + 0x0074
#define CEC_TX_PRESENT_STATE_REG_ADDR        CEC_BASE + 0x0078
#define CEC_RX_PRESENT_STATE_REG_ADDR        CEC_BASE + 0x0079
#define CEC_COLL_CTRL_REG_ADDR               CEC_BASE + 0x007a //03
//#define CEC_GLITCH_FILTER_STAGES_ADDR        CEC_BASE + 0x007b //07
#define CEC_COLL_WINDOW_TIME_REG_0_ADDR      CEC_BASE + 0x007c //FF
#define CEC_COLL_WINDOW_TIME_REG_1_ADDR      CEC_BASE + 0x007d //FF
#define CEC_COLL_WINDOW_TIME_REG_2_ADDR      CEC_BASE + 0x007e //FF
#define CEC_COLL_WINDOW_TIME_REG_3_ADDR      CEC_BASE + 0x007f //FF
#define CEC_TX_FIFO_FULL_THRESH              CEC_BASE + 0x0080 //0F
#define CEC_TX_FIFO_WPTR                     CEC_BASE + 0x0081
#define CEC_TX_FIFO_RPTR                     CEC_BASE + 0x0082
#define CEC_TX_FIFO_DPTR                     CEC_BASE + 0x0083
#define CEC_RX_FIFO_FULL_THRESH              CEC_BASE + 0x0084 //0F
#define CEC_RX_FIFO_WPTR                     CEC_BASE + 0x0085
#define CEC_RX_FIFO_RPTR                     CEC_BASE + 0x0086
#define CEC_RX_FIFO_DPTR                     CEC_BASE + 0x0087
#define CEC_JITTER_CNT_SB_0                  CEC_BASE + 0x0088 //FF
#define CEC_JITTER_CNT_SB_1                  CEC_BASE + 0x0089 //FF
#define CEC_JITTER_CNT_SB_2                  CEC_BASE + 0x008a //FF
#define CEC_JITTER_CNT_SB_3                  CEC_BASE + 0x008b //FF
#define CEC_ERR_NOTIF_TIME_0                 CEC_BASE + 0x008c //FF
#define CEC_ERR_NOTIF_TIME_1                 CEC_BASE + 0x008d //FF
#define CEC_ERR_NOTIF_TIME_2                 CEC_BASE + 0x008e //FF
#define CEC_ERR_NOTIF_TIME_3                 CEC_BASE + 0x008f //FF
#define CEC_GLITCH_FILT_W_L                  CEC_BASE + 0x0090 //FF
#define CEC_GLITCH_FILT_W_H                  CEC_BASE + 0x0091 //FF

//--------------------------------
//HDCP
//--------------------------------

//ADDRESS OF AKEYS
#define TX_HDCP_AKEY0_BYTE_0     0x1200 //00
#define TX_HDCP_AKEY0_BYTE_1     0x1201 //00
#define TX_HDCP_AKEY0_BYTE_2     0x1202 //00
#define TX_HDCP_AKEY0_BYTE_3     0x1203 //00
#define TX_HDCP_AKEY0_BYTE_4     0x1204 //00
#define TX_HDCP_AKEY0_BYTE_5     0x1205 //00
#define TX_HDCP_AKEY0_BYTE_6     0x1206 //00

#define TX_HDCP_AKEY1_BYTE_0     0x1208 //00
#define TX_HDCP_AKEY1_BYTE_1     0x1209 //00
#define TX_HDCP_AKEY1_BYTE_2     0x120A //00
#define TX_HDCP_AKEY1_BYTE_3     0x120B //00
#define TX_HDCP_AKEY1_BYTE_4     0x120C //00
#define TX_HDCP_AKEY1_BYTE_5     0x120D //00
#define TX_HDCP_AKEY1_BYTE_6     0x120E //00

#define TX_HDCP_AKEY2_BYTE_0     0x1210 //00
#define TX_HDCP_AKEY2_BYTE_1     0x1211 //00
#define TX_HDCP_AKEY2_BYTE_2     0x1212 //00
#define TX_HDCP_AKEY2_BYTE_3     0x1213 //00
#define TX_HDCP_AKEY2_BYTE_4     0x1214 //00
#define TX_HDCP_AKEY2_BYTE_5     0x1215 //00
#define TX_HDCP_AKEY2_BYTE_6     0x1216 //00

#define TX_HDCP_AKEY3_BYTE_0     0x1218 //00
#define TX_HDCP_AKEY3_BYTE_1     0x1219 //00
#define TX_HDCP_AKEY3_BYTE_2     0x121A //00
#define TX_HDCP_AKEY3_BYTE_3     0x121B //00
#define TX_HDCP_AKEY3_BYTE_4     0x121C //00
#define TX_HDCP_AKEY3_BYTE_5     0x121D //00
#define TX_HDCP_AKEY3_BYTE_6     0x121E //00

#define TX_HDCP_AKEY4_BYTE_0     0x1220 //00
#define TX_HDCP_AKEY4_BYTE_1     0x1221 //00
#define TX_HDCP_AKEY4_BYTE_2     0x1222 //00
#define TX_HDCP_AKEY4_BYTE_3     0x1223 //00
#define TX_HDCP_AKEY4_BYTE_4     0x1224 //00
#define TX_HDCP_AKEY4_BYTE_5     0x1225 //00
#define TX_HDCP_AKEY4_BYTE_6     0x1226 //00

#define TX_HDCP_AKEY5_BYTE_0     0x1228 //00
#define TX_HDCP_AKEY5_BYTE_1     0x1229 //00
#define TX_HDCP_AKEY5_BYTE_2     0x122A //00
#define TX_HDCP_AKEY5_BYTE_3     0x122B //00
#define TX_HDCP_AKEY5_BYTE_4     0x122C //00
#define TX_HDCP_AKEY5_BYTE_5     0x122D //00
#define TX_HDCP_AKEY5_BYTE_6     0x122E //00

#define TX_HDCP_AKEY6_BYTE_0     0x1230 //00
#define TX_HDCP_AKEY6_BYTE_1     0x1231 //00
#define TX_HDCP_AKEY6_BYTE_2     0x1232 //00
#define TX_HDCP_AKEY6_BYTE_3     0x1233 //00
#define TX_HDCP_AKEY6_BYTE_4     0x1234 //00
#define TX_HDCP_AKEY6_BYTE_5     0x1235 //00
#define TX_HDCP_AKEY6_BYTE_6     0x1236 //00

#define TX_HDCP_AKEY7_BYTE_0     0x1238 //00
#define TX_HDCP_AKEY7_BYTE_1     0x1239 //00
#define TX_HDCP_AKEY7_BYTE_2     0x123A //00
#define TX_HDCP_AKEY7_BYTE_3     0x123B //00
#define TX_HDCP_AKEY7_BYTE_4     0x123C //00
#define TX_HDCP_AKEY7_BYTE_5     0x123D //00
#define TX_HDCP_AKEY7_BYTE_6     0x123E //00

#define TX_HDCP_AKEY8_BYTE_0     0x1240 //00
#define TX_HDCP_AKEY8_BYTE_1     0x1241 //00
#define TX_HDCP_AKEY8_BYTE_2     0x1242 //00
#define TX_HDCP_AKEY8_BYTE_3     0x1243 //00
#define TX_HDCP_AKEY8_BYTE_4     0x1244 //00
#define TX_HDCP_AKEY8_BYTE_5     0x1245 //00
#define TX_HDCP_AKEY8_BYTE_6     0x1246 //00

#define TX_HDCP_AKEY9_BYTE_0     0x1248 //00
#define TX_HDCP_AKEY9_BYTE_1     0x1249 //00
#define TX_HDCP_AKEY9_BYTE_2     0x124A //00
#define TX_HDCP_AKEY9_BYTE_3     0x124B //00
#define TX_HDCP_AKEY9_BYTE_4     0x124C //00
#define TX_HDCP_AKEY9_BYTE_5     0x124D //00
#define TX_HDCP_AKEY9_BYTE_6     0x124E //00

#define TX_HDCP_AKEY10_BYTE_0    0x1250 //00
#define TX_HDCP_AKEY10_BYTE_1    0x1251 //00
#define TX_HDCP_AKEY10_BYTE_2    0x1252 //00
#define TX_HDCP_AKEY10_BYTE_3    0x1253 //00
#define TX_HDCP_AKEY10_BYTE_4    0x1254 //00
#define TX_HDCP_AKEY10_BYTE_5    0x1255 //00
#define TX_HDCP_AKEY10_BYTE_6    0x1256 //00

#define TX_HDCP_AKEY11_BYTE_0    0x1258 //00
#define TX_HDCP_AKEY11_BYTE_1    0x1259 //00
#define TX_HDCP_AKEY11_BYTE_2    0x125A //00
#define TX_HDCP_AKEY11_BYTE_3    0x125B //00
#define TX_HDCP_AKEY11_BYTE_4    0x125C //00
#define TX_HDCP_AKEY11_BYTE_5    0x125D //00
#define TX_HDCP_AKEY11_BYTE_6    0x125E //00

#define TX_HDCP_AKEY12_BYTE_0    0x1260 //00
#define TX_HDCP_AKEY12_BYTE_1    0x1261 //00
#define TX_HDCP_AKEY12_BYTE_2    0x1262 //00
#define TX_HDCP_AKEY12_BYTE_3    0x1263 //00
#define TX_HDCP_AKEY12_BYTE_4    0x1264 //00
#define TX_HDCP_AKEY12_BYTE_5    0x1265 //00
#define TX_HDCP_AKEY12_BYTE_6    0x1266 //00

#define TX_HDCP_AKEY13_BYTE_0    0x1268 //00
#define TX_HDCP_AKEY13_BYTE_1    0x1269 //00
#define TX_HDCP_AKEY13_BYTE_2    0x126A //00
#define TX_HDCP_AKEY13_BYTE_3    0x126B //00
#define TX_HDCP_AKEY13_BYTE_4    0x126C //00
#define TX_HDCP_AKEY13_BYTE_5    0x126D //00
#define TX_HDCP_AKEY13_BYTE_6    0x126E //00

#define TX_HDCP_AKEY14_BYTE_0    0x1270 //00
#define TX_HDCP_AKEY14_BYTE_1    0x1271 //00
#define TX_HDCP_AKEY14_BYTE_2    0x1272 //00
#define TX_HDCP_AKEY14_BYTE_3    0x1273 //00
#define TX_HDCP_AKEY14_BYTE_4    0x1274 //00
#define TX_HDCP_AKEY14_BYTE_5    0x1275 //00
#define TX_HDCP_AKEY14_BYTE_6    0x1276 //00

#define TX_HDCP_AKEY15_BYTE_0    0x1278 //00
#define TX_HDCP_AKEY15_BYTE_1    0x1279 //00
#define TX_HDCP_AKEY15_BYTE_2    0x127A //00
#define TX_HDCP_AKEY15_BYTE_3    0x127B //00
#define TX_HDCP_AKEY15_BYTE_4    0x127C //00
#define TX_HDCP_AKEY15_BYTE_5    0x127D //00
#define TX_HDCP_AKEY15_BYTE_6    0x127E //00

#define TX_HDCP_AKEY16_BYTE_0    0x1280 //00
#define TX_HDCP_AKEY16_BYTE_1    0x1281 //00
#define TX_HDCP_AKEY16_BYTE_2    0x1282 //00
#define TX_HDCP_AKEY16_BYTE_3    0x1283 //00
#define TX_HDCP_AKEY16_BYTE_4    0x1284 //00
#define TX_HDCP_AKEY16_BYTE_5    0x1285 //00
#define TX_HDCP_AKEY16_BYTE_6    0x1286 //00

#define TX_HDCP_AKEY17_BYTE_0    0x1288 //00
#define TX_HDCP_AKEY17_BYTE_1    0x1289 //00
#define TX_HDCP_AKEY17_BYTE_2    0x128A //00
#define TX_HDCP_AKEY17_BYTE_3    0x128B //00
#define TX_HDCP_AKEY17_BYTE_4    0x128C //00
#define TX_HDCP_AKEY17_BYTE_5    0x128D //00
#define TX_HDCP_AKEY17_BYTE_6    0x128E //00

#define TX_HDCP_AKEY18_BYTE_0    0x1290 //00
#define TX_HDCP_AKEY18_BYTE_1    0x1291 //00
#define TX_HDCP_AKEY18_BYTE_2    0x1292 //00
#define TX_HDCP_AKEY18_BYTE_3    0x1293 //00
#define TX_HDCP_AKEY18_BYTE_4    0x1294 //00
#define TX_HDCP_AKEY18_BYTE_5    0x1295 //00
#define TX_HDCP_AKEY18_BYTE_6    0x1296 //00

#define TX_HDCP_AKEY19_BYTE_0    0x1298 //00
#define TX_HDCP_AKEY19_BYTE_1    0x1299 //00
#define TX_HDCP_AKEY19_BYTE_2    0x129A //00
#define TX_HDCP_AKEY19_BYTE_3    0x129B //00
#define TX_HDCP_AKEY19_BYTE_4    0x129C //00
#define TX_HDCP_AKEY19_BYTE_5    0x129D //00
#define TX_HDCP_AKEY19_BYTE_6    0x129E //00

#define TX_HDCP_AKEY20_BYTE_0    0x12A0 //00
#define TX_HDCP_AKEY20_BYTE_1    0x12A1 //00
#define TX_HDCP_AKEY20_BYTE_2    0x12A2 //00
#define TX_HDCP_AKEY20_BYTE_3    0x12A3 //00
#define TX_HDCP_AKEY20_BYTE_4    0x12A4 //00
#define TX_HDCP_AKEY20_BYTE_5    0x12A5 //00
#define TX_HDCP_AKEY20_BYTE_6    0x12A6 //00

#define TX_HDCP_AKEY21_BYTE_0    0x12A8 //00
#define TX_HDCP_AKEY21_BYTE_1    0x12A9 //00
#define TX_HDCP_AKEY21_BYTE_2    0x12AA //00
#define TX_HDCP_AKEY21_BYTE_3    0x12AB //00
#define TX_HDCP_AKEY21_BYTE_4    0x12AC //00
#define TX_HDCP_AKEY21_BYTE_5    0x12AD //00
#define TX_HDCP_AKEY21_BYTE_6    0x12AE //00

#define TX_HDCP_AKEY22_BYTE_0    0x12B0 //00
#define TX_HDCP_AKEY22_BYTE_1    0x12B1 //00
#define TX_HDCP_AKEY22_BYTE_2    0x12B2 //00
#define TX_HDCP_AKEY22_BYTE_3    0x12B3 //00
#define TX_HDCP_AKEY22_BYTE_4    0x12B4 //00
#define TX_HDCP_AKEY22_BYTE_5    0x12B5 //00
#define TX_HDCP_AKEY22_BYTE_6    0x12B6 //00

#define TX_HDCP_AKEY23_BYTE_0    0x12B8 //00
#define TX_HDCP_AKEY23_BYTE_1    0x12B9 //00
#define TX_HDCP_AKEY23_BYTE_2    0x12BA //00
#define TX_HDCP_AKEY23_BYTE_3    0x12BB //00
#define TX_HDCP_AKEY23_BYTE_4    0x12BC //00
#define TX_HDCP_AKEY23_BYTE_5    0x12BD //00
#define TX_HDCP_AKEY23_BYTE_6    0x12BE //00

#define TX_HDCP_AKEY24_BYTE_0    0x12C0 //00
#define TX_HDCP_AKEY24_BYTE_1    0x12C1 //00
#define TX_HDCP_AKEY24_BYTE_2    0x12C2 //00
#define TX_HDCP_AKEY24_BYTE_3    0x12C3 //00
#define TX_HDCP_AKEY24_BYTE_4    0x12C4 //00
#define TX_HDCP_AKEY24_BYTE_5    0x12C5 //00
#define TX_HDCP_AKEY24_BYTE_6    0x12C6 //00

#define TX_HDCP_AKEY25_BYTE_0    0x12C8 //00
#define TX_HDCP_AKEY25_BYTE_1    0x12C9 //00
#define TX_HDCP_AKEY25_BYTE_2    0x12CA //00
#define TX_HDCP_AKEY25_BYTE_3    0x12CB //00
#define TX_HDCP_AKEY25_BYTE_4    0x12CC //00
#define TX_HDCP_AKEY25_BYTE_5    0x12CD //00
#define TX_HDCP_AKEY25_BYTE_6    0x12CE //00

#define TX_HDCP_AKEY26_BYTE_0    0x12D0 //00
#define TX_HDCP_AKEY26_BYTE_1    0x12D1 //00
#define TX_HDCP_AKEY26_BYTE_2    0x12D2 //00
#define TX_HDCP_AKEY26_BYTE_3    0x12D3 //00
#define TX_HDCP_AKEY26_BYTE_4    0x12D4 //00
#define TX_HDCP_AKEY26_BYTE_5    0x12D5 //00
#define TX_HDCP_AKEY26_BYTE_6    0x12D6 //00

#define TX_HDCP_AKEY27_BYTE_0    0x12D8 //00
#define TX_HDCP_AKEY27_BYTE_1    0x12D9 //00
#define TX_HDCP_AKEY27_BYTE_2    0x12DA //00
#define TX_HDCP_AKEY27_BYTE_3    0x12DB //00
#define TX_HDCP_AKEY27_BYTE_4    0x12DC //00
#define TX_HDCP_AKEY27_BYTE_5    0x12DD //00
#define TX_HDCP_AKEY27_BYTE_6    0x12DE //00

#define TX_HDCP_AKEY28_BYTE_0    0x12E0 //00
#define TX_HDCP_AKEY28_BYTE_1    0x12E1 //00
#define TX_HDCP_AKEY28_BYTE_2    0x12E2 //00
#define TX_HDCP_AKEY28_BYTE_3    0x12E3 //00
#define TX_HDCP_AKEY28_BYTE_4    0x12E4 //00
#define TX_HDCP_AKEY28_BYTE_5    0x12E5 //00
#define TX_HDCP_AKEY28_BYTE_6    0x12E6 //00

#define TX_HDCP_AKEY29_BYTE_0    0x12E8 //00
#define TX_HDCP_AKEY29_BYTE_1    0x12E9 //00
#define TX_HDCP_AKEY29_BYTE_2    0x12EA //00
#define TX_HDCP_AKEY29_BYTE_3    0x12EB //00
#define TX_HDCP_AKEY29_BYTE_4    0x12EC //00
#define TX_HDCP_AKEY29_BYTE_5    0x12ED //00
#define TX_HDCP_AKEY29_BYTE_6    0x12EE //00

#define TX_HDCP_AKEY30_BYTE_0    0x12F0 //00
#define TX_HDCP_AKEY30_BYTE_1    0x12F1 //00
#define TX_HDCP_AKEY30_BYTE_2    0x12F2 //00
#define TX_HDCP_AKEY30_BYTE_3    0x12F3 //00
#define TX_HDCP_AKEY30_BYTE_4    0x12F4 //00
#define TX_HDCP_AKEY30_BYTE_5    0x12F5 //00
#define TX_HDCP_AKEY30_BYTE_6    0x12F6 //00

#define TX_HDCP_AKEY31_BYTE_0    0x12F8 //00
#define TX_HDCP_AKEY31_BYTE_1    0x12F9 //00
#define TX_HDCP_AKEY31_BYTE_2    0x12FA //00
#define TX_HDCP_AKEY31_BYTE_3    0x12FB //00
#define TX_HDCP_AKEY31_BYTE_4    0x12FC //00
#define TX_HDCP_AKEY31_BYTE_5    0x12FD //00
#define TX_HDCP_AKEY31_BYTE_6    0x12FE //00

#define TX_HDCP_AKEY32_BYTE_0    0x1300 //00
#define TX_HDCP_AKEY32_BYTE_1    0x1301 //00
#define TX_HDCP_AKEY32_BYTE_2    0x1302 //00
#define TX_HDCP_AKEY32_BYTE_3    0x1303 //00
#define TX_HDCP_AKEY32_BYTE_4    0x1304 //00
#define TX_HDCP_AKEY32_BYTE_5    0x1305 //00
#define TX_HDCP_AKEY32_BYTE_6    0x1306 //00

#define TX_HDCP_AKEY33_BYTE_0    0x1308 //00
#define TX_HDCP_AKEY33_BYTE_1    0x1309 //00
#define TX_HDCP_AKEY33_BYTE_2    0x130A //00
#define TX_HDCP_AKEY33_BYTE_3    0x130B //00
#define TX_HDCP_AKEY33_BYTE_4    0x130C //00
#define TX_HDCP_AKEY33_BYTE_5    0x130D //00
#define TX_HDCP_AKEY33_BYTE_6    0x130E //00

#define TX_HDCP_AKEY34_BYTE_0    0x1310 //00
#define TX_HDCP_AKEY34_BYTE_1    0x1311 //00
#define TX_HDCP_AKEY34_BYTE_2    0x1312 //00
#define TX_HDCP_AKEY34_BYTE_3    0x1313 //00
#define TX_HDCP_AKEY34_BYTE_4    0x1314 //00
#define TX_HDCP_AKEY34_BYTE_5    0x1315 //00
#define TX_HDCP_AKEY34_BYTE_6    0x1316 //00

#define TX_HDCP_AKEY35_BYTE_0    0x1318 //00
#define TX_HDCP_AKEY35_BYTE_1    0x1319 //00
#define TX_HDCP_AKEY35_BYTE_2    0x131A //00
#define TX_HDCP_AKEY35_BYTE_3    0x131B //00
#define TX_HDCP_AKEY35_BYTE_4    0x131C //00
#define TX_HDCP_AKEY35_BYTE_5    0x131D //00
#define TX_HDCP_AKEY35_BYTE_6    0x131E //00

#define TX_HDCP_AKEY36_BYTE_0    0x1320 //00
#define TX_HDCP_AKEY36_BYTE_1    0x1321 //00
#define TX_HDCP_AKEY36_BYTE_2    0x1322 //00
#define TX_HDCP_AKEY36_BYTE_3    0x1323 //00
#define TX_HDCP_AKEY36_BYTE_4    0x1324 //00
#define TX_HDCP_AKEY36_BYTE_5    0x1325 //00
#define TX_HDCP_AKEY36_BYTE_6    0x1326 //00

#define TX_HDCP_AKEY37_BYTE_0    0x1328 //00
#define TX_HDCP_AKEY37_BYTE_1    0x1329 //00
#define TX_HDCP_AKEY37_BYTE_2    0x132A //00
#define TX_HDCP_AKEY37_BYTE_3    0x132B //00
#define TX_HDCP_AKEY37_BYTE_4    0x132C //00
#define TX_HDCP_AKEY37_BYTE_5    0x132D //00
#define TX_HDCP_AKEY37_BYTE_6    0x132E //00

#define TX_HDCP_AKEY38_BYTE_0    0x1330 //00
#define TX_HDCP_AKEY38_BYTE_1    0x1331 //00
#define TX_HDCP_AKEY38_BYTE_2    0x1332 //00
#define TX_HDCP_AKEY38_BYTE_3    0x1333 //00
#define TX_HDCP_AKEY38_BYTE_4    0x1334 //00
#define TX_HDCP_AKEY38_BYTE_5    0x1335 //00
#define TX_HDCP_AKEY38_BYTE_6    0x1336 //00

#define TX_HDCP_AKEY39_BYTE_0    0x1338 //00
#define TX_HDCP_AKEY39_BYTE_1    0x1339 //00
#define TX_HDCP_AKEY39_BYTE_2    0x133A //00
#define TX_HDCP_AKEY39_BYTE_3    0x133B //00
#define TX_HDCP_AKEY39_BYTE_4    0x133C //00
#define TX_HDCP_AKEY39_BYTE_5    0x133D //00
#define TX_HDCP_AKEY39_BYTE_6    0x133E //00


#define TX_HDCP_AKSV_BYTE_0      0x1340
#define TX_HDCP_AKSV_BYTE_1      0x1341
#define TX_HDCP_AKSV_BYTE_2      0x1342
#define TX_HDCP_AKSV_BYTE_3      0x1343
#define TX_HDCP_AKSV_BYTE_4      0x1344

// HDCP REGS
#define TX_HDCP_CTRL             0x1350 //FE
#define TX_HDCP_STATUS_1         0x1351 //FE
#define TX_HDCP_STATUS_2         0x1352
#define TX_HDCP_INTR_0           0x1353
#define TX_HDCP_TX_AKSV_0        0x1354
#define TX_HDCP_TX_AKSV_1        0x1355
#define TX_HDCP_TX_AKSV_2        0x1356
#define TX_HDCP_TX_AKSV_3        0x1357
#define TX_HDCP_TX_AKSV_4        0x1358
#define TX_HDCP_RX_BKSV_0        0x135C //FF
#define TX_HDCP_RX_BKSV_1        0x135D //FF
#define TX_HDCP_RX_BKSV_2        0x135E //FF
#define TX_HDCP_RX_BKSV_3        0x135F //FF
#define TX_HDCP_RX_BKSV_4        0x1360 //FF
#define TX_HDCP_TX_AINFO         0x1361 //FF
#define TX_HDCP_RX_BCAPS         0x1362 //FF
#define TX_HDCP_RX_BSTATUS_0     0x1364 //FF
#define TX_HDCP_RX_BSTATUS_1     0x1365 //FF
#define TX_HDCP_TX_AN_0          0x1368
#define TX_HDCP_TX_AN_1          0x1369
#define TX_HDCP_TX_AN_2          0x136A
#define TX_HDCP_TX_AN_3          0x136B
#define TX_HDCP_TX_AN_4          0x136C
#define TX_HDCP_TX_AN_5          0x136D
#define TX_HDCP_TX_AN_6          0x136E
#define TX_HDCP_TX_AN_7          0x136F
#define TX_HDCP_TX_M0_0          0x1370 //M0 shouldn't be accessable by FW
#define TX_HDCP_TX_M0_1          0x1371
#define TX_HDCP_TX_M0_2          0x1372
#define TX_HDCP_TX_M0_3          0x1373
#define TX_HDCP_TX_M0_4          0x1374
#define TX_HDCP_TX_M0_5          0x1375
#define TX_HDCP_TX_M0_6          0x1376
#define TX_HDCP_TX_M0_7          0x1377
#define TX_HDCP_TX_R0_0          0x1378
#define TX_HDCP_TX_R0_1          0x1379
#define TX_HDCP_RX_R0_0          0x137A //FF
#define TX_HDCP_RX_R0_1          0x137B //FF
#define TX_HDCP_TX_RI_0          0x137C
#define TX_HDCP_TX_RI_1          0x137D
#define TX_HDCP_RX_RI_0          0x137E //FF
#define TX_HDCP_RX_RI_1          0x137F //FF
#define TX_HDCP_TX_PJ            0x1380
#define TX_HDCP_RX_PJ            0x1381 //FF
#define TX_HDCP_FIX_CLR_0        0x1384 //FF
#define TX_HDCP_FIX_CLR_1        0x1385 //FF
#define TX_HDCP_FIX_CLR_2        0x1386 //FF
#define TX_HDCP_KINIT_0          0x1388 //FF
#define TX_HDCP_KINIT_1          0x1389 //FF
#define TX_HDCP_KINIT_2          0x138A //FF
#define TX_HDCP_KINIT_3          0x138B //FF
#define TX_HDCP_KINIT_4          0x138C //FF
#define TX_HDCP_KINIT_5          0x138D //FF
#define TX_HDCP_KINIT_6          0x138E //FF
#define TX_HDCP_BINIT_0          0x1390 //FF
#define TX_HDCP_BINIT_1          0x1391 //FF
#define TX_HDCP_BINIT_2          0x1392 //FF
#define TX_HDCP_BINIT_3          0x1393 //FF
#define TX_HDCP_BINIT_4          0x1394 //FF
#define TX_HDCP_BINIT_5          0x1395 //FF
#define TX_HDCP_BINIT_6          0x1396 //FF
#define TX_HDCP_BINIT_7          0x1397 //FF
#define TX_HDCP_INTR_CLR         0x1398 //F8
#define TX_HDCP_INTR_MASK        0x1399 //F8

//------ VOP Registers ------
#define VOP_HDMI_CBLANK_L        0x1800 //FF
#define VOP_HDMI_CBLANK_H        0x1801 //0F
#define VOP_HDMI_MODE            0x1802 //01
#define VOP_HDMI_SEL             0x1803 //07
#define VOP_HDMI_ALIGN           0x1804 //03
#define VOP_HDMI_RND             0x1805 //07
#define VOP_AHD_SEL              0x1806 //07
#define VOP_AHD_ALIGN            0x1807 //03
#define VOP_AHD_RND              0x1808 //07
#define VOP_ASD_SEL              0x1809 //07
#define VOP_ASD_ALIGN            0x180a //03
#define VOP_ASD_RND              0x180b //07
#define VOP_TTL24_SEL            0x180c //07
#define VOP_TTL30_MODE           0x180d //01
#define VOP_TTL30_CBLANK_L       0x180e //FF
#define VOP_TTL30_CBLANK_H       0x180f //0F
#define VOP_TTL30_SEL            0x1810 //07

#define VOP_3D_HDMI_CTRL0        0x1811 //07
#define VOP_3D_HDMI_CTRL1        0x1812 //07
#define VOP_3D_HDMI_UDATA_L      0x1814 //07
#define VOP_3D_HDMI_UDATA_H      0x1815 //07
#define VOP_3D_HDMI_MDATA_L      0x1816 //07
#define VOP_3D_HDMI_MDATA_H      0x1817 //07
#define VOP_3D_HDMI_LDATA_L      0x1818 //07
#define VOP_3D_HDMI_LDATA_H      0x1819 //07

#define VOP_AHD_C0_L             0x1820 //FF
#define VOP_AHD_C0_H             0x1821 //3F
#define VOP_AHD_C1_L             0x1822 //FF
#define VOP_AHD_C1_H             0x1823 //3F
#define VOP_AHD_C2_L             0x1824 //FF
#define VOP_AHD_C2_H             0x1825 //3F
#define VOP_AHD_C3_L             0x1826 //FF
#define VOP_AHD_C3_H             0x1827 //3F
#define VOP_AHD_C4_L             0x1828 //FF
#define VOP_AHD_C4_H             0x1829 //3F
#define VOP_AHD_C5_L             0x182a //FF
#define VOP_AHD_C5_H             0x182b //3F
#define VOP_AHD_C6_L             0x182c //FF
#define VOP_AHD_C6_H             0x182d //3F
#define VOP_AHD_C7_L             0x182e //FF
#define VOP_AHD_C7_H             0x182f //3F
#define VOP_AHD_C8_L             0x1830 //FF
#define VOP_AHD_C8_H             0x1831 //3F
#define VOP_AHD_A0_L             0x1832 //FF
#define VOP_AHD_A0_H             0x1833 //FF
#define VOP_AHD_A0_HH            0x1834 //03
#define VOP_AHD_A1_L             0x1835 //FF
#define VOP_AHD_A1_H             0x1836 //FF
#define VOP_AHD_A1_HH            0x1837 //03
#define VOP_AHD_A2_L             0x1838 //FF
#define VOP_AHD_A2_H             0x1839 //FF
#define VOP_AHD_A2_HH            0x183a //03

#define VOP_ASD_C0_L             0x1840 //FF
#define VOP_ASD_C0_H             0x1841 //3F
#define VOP_ASD_C1_L             0x1842 //FF
#define VOP_ASD_C1_H             0x1843 //3F
#define VOP_ASD_C2_L             0x1844 //FF
#define VOP_ASD_C2_H             0x1845 //3F
#define VOP_ASD_C3_L             0x1846 //FF
#define VOP_ASD_C3_H             0x1847 //3F
#define VOP_ASD_C4_L             0x1848 //FF
#define VOP_ASD_C4_H             0x1849 //3F
#define VOP_ASD_C5_L             0x184a //FF
#define VOP_ASD_C5_H             0x184b //3F
#define VOP_ASD_C6_L             0x184c //FF
#define VOP_ASD_C6_H             0x184d //3F
#define VOP_ASD_C7_L             0x184e //FF
#define VOP_ASD_C7_H             0x184f //3F
#define VOP_ASD_C8_L             0x1850 //FF
#define VOP_ASD_C8_H             0x1851 //3F
#define VOP_ASD_A0_L             0x1852 //FF
#define VOP_ASD_A0_H             0x1853 //FF
#define VOP_ASD_A0_HH            0x1854 //03
#define VOP_ASD_A1_L             0x1855 //FF
#define VOP_ASD_A1_H             0x1856 //FF
#define VOP_ASD_A1_HH            0x1857 //03
#define VOP_ASD_A2_L             0x1858 //FF
#define VOP_ASD_A2_H             0x1859 //FF
#define VOP_ASD_A2_HH            0x185a //03

#define VOP_HDMI_C0_L            0x1860 //FF
#define VOP_HDMI_C0_H            0x1861 //3F
#define VOP_HDMI_C1_L            0x1862 //FF
#define VOP_HDMI_C1_H            0x1863 //3F
#define VOP_HDMI_C2_L            0x1864 //FF
#define VOP_HDMI_C2_H            0x1865 //3F
#define VOP_HDMI_C3_L            0x1866 //FF
#define VOP_HDMI_C3_H            0x1867 //3F
#define VOP_HDMI_C4_L            0x1868 //FF
#define VOP_HDMI_C4_H            0x1869 //3F
#define VOP_HDMI_C5_L            0x186a //FF
#define VOP_HDMI_C5_H            0x186b //3F
#define VOP_HDMI_C6_L            0x186c //FF
#define VOP_HDMI_C6_H            0x186d //3F
#define VOP_HDMI_C7_L            0x186e //FF
#define VOP_HDMI_C7_H            0x186f //3F
#define VOP_HDMI_C8_L            0x1870 //FF
#define VOP_HDMI_C8_H            0x1871 //3F
#define VOP_HDMI_A0_L            0x1872 //FF
#define VOP_HDMI_A0_H            0x1873 //FF
#define VOP_HDMI_A0_HH           0x1874 //03
#define VOP_HDMI_A1_L            0x1875 //FF
#define VOP_HDMI_A1_H            0x1876 //FF
#define VOP_HDMI_A1_HH           0x1877 //03
#define VOP_HDMI_A2_L            0x1878 //FF
#define VOP_HDMI_A2_H            0x1879 //FF
#define VOP_HDMI_A2_HH           0x187a //03

#define VOP_TTL24_C0_L           0x1880 //FF
#define VOP_TTL24_C0_H           0x1881 //3F
#define VOP_TTL24_C1_L           0x1882 //FF
#define VOP_TTL24_C1_H           0x1883 //3F
#define VOP_TTL24_C2_L           0x1884 //FF
#define VOP_TTL24_C2_H           0x1885 //3F
#define VOP_TTL24_C3_L           0x1886 //FF
#define VOP_TTL24_C3_H           0x1887 //3F
#define VOP_TTL24_C4_L           0x1888 //FF
#define VOP_TTL24_C4_H           0x1889 //3F
#define VOP_TTL24_C5_L           0x188a //FF
#define VOP_TTL24_C5_H           0x188b //3F
#define VOP_TTL24_C6_L           0x188c //FF
#define VOP_TTL24_C6_H           0x188d //3F
#define VOP_TTL24_C7_L           0x188e //FF
#define VOP_TTL24_C7_H           0x188f //3F
#define VOP_TTL24_C8_L           0x1890 //FF
#define VOP_TTL24_C8_H           0x1891 //3F
#define VOP_TTL24_A0_L           0x1892 //FF
#define VOP_TTL24_A0_H           0x1893 //FF
#define VOP_TTL24_A0_HH          0x1894 //03
#define VOP_TTL24_A1_L           0x1895 //FF
#define VOP_TTL24_A1_H           0x1896 //FF
#define VOP_TTL24_A1_HH          0x1897 //03
#define VOP_TTL24_A2_L           0x1898 //FF
#define VOP_TTL24_A2_H           0x1899 //FF
#define VOP_TTL24_A2_HH          0x189a //03

#define VOP_TTL30_C0_L           0x18a0 //FF
#define VOP_TTL30_C0_H           0x18a1 //3F
#define VOP_TTL30_C1_L           0x18a2 //FF
#define VOP_TTL30_C1_H           0x18a3 //3F
#define VOP_TTL30_C2_L           0x18a4 //FF
#define VOP_TTL30_C2_H           0x18a5 //3F
#define VOP_TTL30_C3_L           0x18a6 //FF
#define VOP_TTL30_C3_H           0x18a7 //3F
#define VOP_TTL30_C4_L           0x18a8 //FF
#define VOP_TTL30_C4_H           0x18a9 //3F
#define VOP_TTL30_C5_L           0x18aa //FF
#define VOP_TTL30_C5_H           0x18ab //3F
#define VOP_TTL30_C6_L           0x18ac //FF
#define VOP_TTL30_C6_H           0x18ad //3F
#define VOP_TTL30_C7_L           0x18ae //FF
#define VOP_TTL30_C7_H           0x18af //3F
#define VOP_TTL30_C8_L           0x18b0 //FF
#define VOP_TTL30_C8_H           0x18b1 //3F
#define VOP_TTL30_A0_L           0x18b2 //FF
#define VOP_TTL30_A0_H           0x18b3 //FF
#define VOP_TTL30_A0_HH          0x18b4 //03
#define VOP_TTL30_A1_L           0x18b5 //FF
#define VOP_TTL30_A1_H           0x18b6 //FF
#define VOP_TTL30_A1_HH          0x18b7 //03
#define VOP_TTL30_A2_L           0x18b8 //FF
#define VOP_TTL30_A2_H           0x18b9 //FF
#define VOP_TTL30_A2_HH          0x18ba //03

#define VOP_TTL30_TRIG           0x18C0 //01
#define VOP_TTL30_NOF            0x18C1 //FF
#define VOP_TTL30_SIG0           0x18C2
#define VOP_TTL30_SIG1           0x18C3
#define VOP_TTL30_SIG2           0x18C4
#define VOP_TTL30_SIG3           0x18C5
#define VOP_TTL30_SIG_ST         0x18C6

#define VOP_TTL24_TRIG           0x18D0 //01
#define VOP_TTL24_NOF            0x18D1 //FF
#define VOP_TTL24_SIG0           0x18D2
#define VOP_TTL24_SIG1           0x18D3
#define VOP_TTL24_SIG2           0x18D4
#define VOP_TTL24_SIG3           0x18D5
#define VOP_TTL24_SIG_ST         0x18D6
#define VOP_C656_SATURATE        0x18D7 //03
#define VOP_TTL24_CBLANK_L       0x18DC //FF
#define VOP_TTL24_CBLANK_H       0x18DD //0F
#define VOP_TTL24_MODE           0x18DE //01
#define VOP_CLKPOL               0x18DF //03

#define VOP_HDMI_SIG0            0x18E0
#define VOP_HDMI_SIG1            0x18E1
#define VOP_HDMI_SIG2            0x18E2
#define VOP_HDMI_SIG3            0x18E3
#define VOP_HDMI_SIG_ST          0x18E4
#define VOP_HDMI_TRIG            0x18E5 //01
#define VOP_HDMI_NOF             0x18E6 //FF

#define VOP_ASD_SIG_ST           0x18E7
#define VOP_ASD_SIG0             0x18E8
#define VOP_ASD_SIG1             0x18E9
#define VOP_ASD_SIG2             0x18EA
#define VOP_ASD_SIG3             0x18EB
#define VOP_ASD_TRIG             0x18EC //01
#define VOP_ASD_NOF              0x18ED //FF

#define VOP_AHD_SIG0             0x18F0
#define VOP_AHD_SIG1             0x18F1
#define VOP_AHD_SIG2             0x18F2
#define VOP_AHD_SIG3             0x18F3
#define VOP_AHD_SIG_ST           0x18F4
#define VOP_AHD_TRIG             0x18F5 //01
#define VOP_AHD_NOF              0x18F6 //FF

#define VOP_TTL30_SATURATE       0x18F8 //03
#define VOP_TTL30_CTRL           0x18F9 //3F
#define VOP_TTL24_CTRL           0x18FA //3F
#define VOP_AHD_CLKX_CTRL        0x18FB //3F
#define VOP_ASD_CLKX_CTRL        0x18FC //3F
#define VOP_HDMI_CLKX_CTRL       0x18FD //3F
#define VOP_TTL30_DELAY          0x18FE //0F
#define VOP_TTL24_DELAY          0x18FF //0F

//CPCB0
#define CPCB0_MODE              0x1401 //NOT NEEDED
#define CPCB0_LWIN              0x1402 //NOT NEEDED
#define CPCB0_FT                0x1403 //NOT NEEDED
#define CPCB0_VT_L              0x1404 //FF
#define CPCB0_VT_H              0x1405 //0F
#define CPCB0_HT_L              0x1406 //FF
#define CPCB0_HT_H              0x1407 //0F
#define CPCB0_XI_L              0x1408 //NOT NEEDED
#define CPCB0_XI_H              0x1409 //NOT NEEDED
#define CPCB0_YI_L              0x140a //NOT NEEDED
#define CPCB0_YI_H              0x140b //NOT NEEDED
#define CPCB0_VX_L              0x140c //FF
#define CPCB0_VX_H              0x140d //0F
#define CPCB0_FRST_L            0x140e //FF
#define CPCB0_FRST_H            0x140f //0F
#define CPCB0_HS_S_L            0x1410 //FF
#define CPCB0_HS_S_H            0x1411 //0F
#define CPCB0_HS_E_L            0x1412 //FF
#define CPCB0_HS_E_H            0x1413 //0F
#define CPCB0_VS_S_L            0x1414 //FF
#define CPCB0_VS_S_H            0x1415 //0F
#define CPCB0_VS_E_L            0x1416 //FF
#define CPCB0_VS_E_H            0x1417 //0F
#define CPCB0_P0_SX_L           0x1418 //FF
#define CPCB0_P0_SX_H           0x1419 //0F
#define CPCB0_P0_EX_L           0x141a //FF
#define CPCB0_P0_EX_H           0x141b //0F
#define CPCB0_P0_SY_L           0x141c //FF
#define CPCB0_P0_SY_H           0x141d //0F
#define CPCB0_P0_EY_L           0x141e //FF
#define CPCB0_P0_EY_H           0x141f //0F
#define CPCB0_P1_SX_L           0x1420 //FF
#define CPCB0_P1_SX_H           0x1421 //0F
#define CPCB0_P1_EX_L           0x1422 //FF
#define CPCB0_P1_EX_H           0x1423 //0F
#define CPCB0_P1_SY_L           0x1424 //FF
#define CPCB0_P1_SY_H           0x1425 //0F
#define CPCB0_P1_EY_L           0x1426 //FF
#define CPCB0_P1_EY_H           0x1427 //0F
#define CPCB0_P1_CR1_SX_L       0x1428 //FF
#define CPCB0_P1_CR1_SX_H       0x1429 //0F
#define CPCB0_P1_CR1_EX_L       0x142a //FF
#define CPCB0_P1_CR1_EX_H       0x142b //0F
#define CPCB0_P1_CR1_SY_L       0x142c //FF
#define CPCB0_P1_CR1_SY_H       0x142d //0F
#define CPCB0_P1_CR1_EY_L       0x142e //FF
#define CPCB0_P1_CR1_EY_H       0x142f //0F
#define CPCB0_P1_CR2_SX_L       0x1430 //FF
#define CPCB0_P1_CR2_SX_H       0x1431 //0F
#define CPCB0_P1_CR2_EX_L       0x1432 //FF
#define CPCB0_P1_CR2_EX_H       0x1433 //0F
#define CPCB0_P1_CR2_SY_L       0x1434 //FF
#define CPCB0_P1_CR2_SY_H       0x1435 //0F
#define CPCB0_P1_CR2_EY_L       0x1436 //FF
#define CPCB0_P1_CR2_EY_H       0x1437 //0F
#define CPCB0_P2_SX_L           0x1438 //FF
#define CPCB0_P2_SX_H           0x1439 //0F
#define CPCB0_P2_EX_L           0x143a //FF
#define CPCB0_P2_EX_H           0x143b //0F
#define CPCB0_P2_SY_L           0x143c //FF
#define CPCB0_P2_SY_H           0x143d //0F
#define CPCB0_P2_EY_L           0x143e //FF
#define CPCB0_P2_EY_H           0x143f //0F
#define CPCB0_P2_CR1_SX_L       0x1440 //FF
#define CPCB0_P2_CR1_SX_H       0x1441 //0F
#define CPCB0_P2_CR1_EX_L       0x1442 //FF
#define CPCB0_P2_CR1_EX_H       0x1443 //0F
#define CPCB0_P2_CR1_SY_L       0x1444 //FF
#define CPCB0_P2_CR1_SY_H       0x1445 //0F
#define CPCB0_P2_CR1_EY_L       0x1446 //FF
#define CPCB0_P2_CR1_EY_H       0x1447 //0F
#define CPCB0_P2_CR2_SX_L       0x1448 //FF
#define CPCB0_P2_CR2_SX_H       0x1449 //0F
#define CPCB0_P2_CR2_EX_L       0x144a //FF
#define CPCB0_P2_CR2_EX_H       0x144b //0F
#define CPCB0_P2_CR2_SY_L       0x144c //FF
#define CPCB0_P2_CR2_SY_H       0x144d //0F
#define CPCB0_P2_CR2_EY_L       0x144e //FF
#define CPCB0_P2_CR2_EY_H       0x144f //0F
#define CPCB0_P3_SX_L           0x1450 //FF
#define CPCB0_P3_SX_H           0x1451 //0F
#define CPCB0_P3_EX_L           0x1452 //FF
#define CPCB0_P3_EX_H           0x1453 //0F
#define CPCB0_P3_SY_L           0x1454 //FF
#define CPCB0_P3_SY_H           0x1455 //0F
#define CPCB0_P3_EY_L           0x1456 //FF
#define CPCB0_P3_EY_H           0x1457 //0F
#define CPCB0_P3A_SX_L          0x1458 //FF
#define CPCB0_P3A_SX_H          0x1459 //0F
#define CPCB0_P3A_EX_L          0x145a //FF
#define CPCB0_P3A_EX_H          0x145b //0F
#define CPCB0_P3A_SY_L          0x145c //FF
#define CPCB0_P3A_SY_H          0x145d //0F
#define CPCB0_P3A_EY_L          0x145e //FF
#define CPCB0_P3A_EY_H          0x145f //0F
#define CPCB0_P4_SX_L           0x1460 //FF
#define CPCB0_P4_SX_H           0x1461 //0F
#define CPCB0_P4_EX_L           0x1462 //FF
#define CPCB0_P4_EX_H           0x1463 //0F
#define CPCB0_P4_SY_L           0x1464 //FF
#define CPCB0_P4_SY_H           0x1465 //0F
#define CPCB0_P4_EY_L           0x1466 //FF
#define CPCB0_P4_EY_H           0x1467 //0F
#define CPCB0_P5_SX_L           0x1468 //FF
#define CPCB0_P5_SX_H           0x1469 //0F
#define CPCB0_P5_EX_L           0x146a //FF
#define CPCB0_P5_EX_H           0x146b //0F
#define CPCB0_P5_SY_L           0x146c //FF
#define CPCB0_P5_SY_H           0x146d //0F
#define CPCB0_P5_EY_L           0x146e //FF
#define CPCB0_P5_EY_H           0x146f //0F
#define CPCB0_P6_SX_L           0x1470 //FF
#define CPCB0_P6_SX_H           0x1471 //0F
#define CPCB0_P6_EX_L           0x1472 //FF
#define CPCB0_P6_EX_H           0x1473 //0F
#define CPCB0_P6_SY_L           0x1474 //FF
#define CPCB0_P6_SY_H           0x1475 //0F
#define CPCB0_P6_EY_L           0x1476 //FF
#define CPCB0_P6_EY_H           0x1477 //0F
#define CPCB0_P7_SX_L           0x1478 //FF
#define CPCB0_P7_SX_H           0x1479 //0F
#define CPCB0_P7_EX_L           0x147A //FF
#define CPCB0_P7_EX_H           0x147B //0F
#define CPCB0_P7_SY_L           0x147C //FF
#define CPCB0_P7_SY_H           0x147D //0F
#define CPCB0_P7_EY_L           0x147E //FF
#define CPCB0_P7_EY_H           0x147F //0F
#define CPCB0_P7A_SX_L          0x1480 //FF
#define CPCB0_P7A_SX_H          0x1481 //0F
#define CPCB0_P7A_EX_L          0x1482 //FF
#define CPCB0_P7A_EX_H          0x1483 //0F
#define CPCB0_P7A_SY_L          0x1484 //FF
#define CPCB0_P7A_SY_H          0x1485 //0F
#define CPCB0_P7A_EY_L          0x1486 //FF
#define CPCB0_P7A_EY_H          0x1487 //0F
#define CPCB0_VO_BASE_CLR_0     0x1488 //FF
#define CPCB0_VO_BASE_CLR_1     0x1489 //FF
#define CPCB0_VO_BASE_CLR_2     0x148A //FF
#define CPCB0_VO_WEIGHT         0x148C //FF
#define CPCB0_EE0_CTRL          0x1490 //0F
#define CPCB0_EE0_LTIGAIN       0x1491 //FF
#define CPCB0_EE0_CTIGAIN       0x1492 //FF
#define CPCB0_EE1_CTRL          0x1493 //0F
#define CPCB0_EE1_LTIGAIN       0x1494 //FF
#define CPCB0_EE1_CTIGAIN       0x1495 //FF

#define CPCB0_VS_E_3D_LEVEN_L   0x1496 //FF
#define CPCB0_VS_E_3D_LEVEN_H   0x1497 //FF
//////////////////////////////////////////////////////////////////////////
#define CPCB0_OO_LAY12          0x14EA //FF
#define CPCB0_OO_LAY34          0x14EB //FF
#define CPCB0_OO_FIX_0          0x14EC //FF
#define CPCB0_OO_FIX_1          0x14ED //FF
#define CPCB0_OO_FIX_2          0x14EE //FF
#define CPCB0_OO_FIX0_0         0x14F0 //FF
#define CPCB0_OO_FIX0_1         0x14F1 //FF
#define CPCB0_OO_FIX0_2         0x14F2 //FF
#define CPCB0_OO_FIX1_0         0x14F4 //FF
#define CPCB0_OO_FIX1_1         0x14F5 //FF
#define CPCB0_OO_FIX1_2         0x14F6 //FF
#define CPCB0_OO_FIX2_0         0x14F8 //FF
#define CPCB0_OO_FIX2_1         0x14F9 //FF
#define CPCB0_OO_FIX2_2         0x14FA //FF
#define CPCB0_OO_FIX3_0         0x14FC //FF
#define CPCB0_OO_FIX3_1         0x14FD //FF
#define CPCB0_OO_FIX3_2         0x14FE //FF
#define CPCB0_OO_CTRL           0x1500 //03
#define CPCB0_OO_P1_AL          0x1501 //FF
#define CPCB0_OO_P2_AL          0x1502 //FF
#define CPCB0_OO_FIX3A_0        0x1504 //FF
#define CPCB0_OO_FIX3A_1        0x1505 //FF
#define CPCB0_OO_FIX3A_2        0x1506 //FF

//--- CPCB0
#define CPCB0_FGG_CTRL          0x1508 //FF
#define CPCB0_FGG_GAIN          0x1509 //FF
#define CPCB0_FGG_TEMP          0x150A //FF
#define CPCB0_FGG_LTH_L         0x150C //FF
#define CPCB0_FGG_LTH_H         0x150D //0F
#define CPCB0_FGG_HTH_L         0x150E //FF
#define CPCB0_FGG_HTH_H         0x150F //0F

//CPCB0_INT
#define  CPCB0_INT_CTRL         0x1510 //0E
#define  CPCB0_INT_MEMSIZE_L    0x1512 //FF
#define  CPCB0_INT_MEMSIZE_H    0x1513 //07
#define  CPCB0_INT_WF_L         0x1514 //FF
#define  CPCB0_INT_WF_H         0x1515 //01
#define  CPCB0_INT_FX_L         0x1516 //FF
#define  CPCB0_INT_FX_H         0x1517 //0F
#define  CPCB0_INT_FY_L         0x1518 //FF
#define  CPCB0_INT_FY_H         0x1519 //0F
#define  CPCB0_INT_V_S_L        0x151a //FF
#define  CPCB0_INT_V_S_H        0x151b //0F
#define  CPCB0_INT_V_E_L        0x151c //FF
#define  CPCB0_INT_V_E_H        0x151d //0F
#define  CPCB0_INT_H_S_L        0x151e //FF
#define  CPCB0_INT_H_S_H        0x151f //0F
#define  CPCB0_INT_H_E_L        0x1520 //FF
#define  CPCB0_INT_H_E_H        0x1521 //0F
#define  CPCB0_INT_VX_E_L       0x1522 //FF
#define  CPCB0_INT_VX_E_H       0x1523 //0F
#define  CPCB0_INT_VX_O_L       0x1524 //FF
#define  CPCB0_INT_VX_O_H       0x1525 //0F
#define  CPCB0_INT_YBL_L        0x1526 //FF
#define  CPCB0_INT_YBL_H        0x1527 //0F
#define  CPCB0_INT_CBL_L        0x1528 //FF
#define  CPCB0_INT_CBL_H        0x1529 //0F
#define  CPCB0_MISC_CTRL        0x152b //07

//--CPCB1  1600
#define CPCB1_MODE              0x1601 //NOT NEEDED
#define CPCB1_LWIN              0x1602 //NOT NEEDED
#define CPCB1_FT                0x1603 //NOT NEEDED
#define CPCB1_VT_L              0x1604 //FF
#define CPCB1_VT_H              0x1605 //0F
#define CPCB1_HT_L              0x1606 //FF
#define CPCB1_HT_H              0x1607 //0F
#define CPCB1_XI_L              0x1608 //NOT NEEDED
#define CPCB1_XI_H              0x1609 //NOT NEEDED
#define CPCB1_YI_L              0x160a //NOT NEEDED
#define CPCB1_YI_H              0x160b //NOT NEEDED
#define CPCB1_VX_L              0x160c //FF
#define CPCB1_VX_H              0x160d //0F
#define CPCB1_FRST_L            0x160e //FF
#define CPCB1_FRST_H            0x160f //0F
#define CPCB1_HS_S_L            0x1610 //FF
#define CPCB1_HS_S_H            0x1611 //0F
#define CPCB1_HS_E_L            0x1612 //FF
#define CPCB1_HS_E_H            0x1613 //0F
#define CPCB1_VS_S_L            0x1614 //FF
#define CPCB1_VS_S_H            0x1615 //0F
#define CPCB1_VS_E_L            0x1616 //FF
#define CPCB1_VS_E_H            0x1617 //0F
#define CPCB1_P0_SX_L           0x1618 //FF
#define CPCB1_P0_SX_H           0x1619 //0F
#define CPCB1_P0_EX_L           0x161a //FF
#define CPCB1_P0_EX_H           0x161b //0F
#define CPCB1_P0_SY_L           0x161c //FF
#define CPCB1_P0_SY_H           0x161d //0F
#define CPCB1_P0_EY_L           0x161e //FF
#define CPCB1_P0_EY_H           0x161f //0F
#define CPCB1_P1_SX_L           0x1620 //FF
#define CPCB1_P1_SX_H           0x1621 //0F
#define CPCB1_P1_EX_L           0x1622 //FF
#define CPCB1_P1_EX_H           0x1623 //0F
#define CPCB1_P1_SY_L           0x1624 //FF
#define CPCB1_P1_SY_H           0x1625 //0F
#define CPCB1_P1_EY_L           0x1626 //FF
#define CPCB1_P1_EY_H           0x1627 //0F
#define CPCB1_P1_CR1_SX_L       0x1628 //FF
#define CPCB1_P1_CR1_SX_H       0x1629 //0F
#define CPCB1_P1_CR1_EX_L       0x162a //FF
#define CPCB1_P1_CR1_EX_H       0x162b //0F
#define CPCB1_P1_CR1_SY_L       0x162c //FF
#define CPCB1_P1_CR1_SY_H       0x162d //0F
#define CPCB1_P1_CR1_EY_L       0x162e //FF
#define CPCB1_P1_CR1_EY_H       0x162f //0F
//#define CPCB1_P1_CR2_SX_L       0x1630 //NOT NEEDED
//#define CPCB1_P1_CR2_SX_H       0x1631 //NOT NEEDED
//#define CPCB1_P1_CR2_EX_L       0x1632 //NOT NEEDED
//#define CPCB1_P1_CR2_EX_H       0x1633 //NOT NEEDED
//#define CPCB1_P1_CR2_SY_L       0x1634 //NOT NEEDED
//#define CPCB1_P1_CR2_SY_H       0x1635 //NOT NEEDED
//#define CPCB1_P1_CR2_EY_L       0x1636 //NOT NEEDED
//#define CPCB1_P1_CR2_EY_H       0x1637 //NOT NEEDED
#define CPCB1_P2_SX_L           0x1638 //FF
#define CPCB1_P2_SX_H           0x1639 //0F
#define CPCB1_P2_EX_L           0x163a //FF
#define CPCB1_P2_EX_H           0x163b //0F
#define CPCB1_P2_SY_L           0x163c //FF
#define CPCB1_P2_SY_H           0x163d //0F
#define CPCB1_P2_EY_L           0x163e //FF
#define CPCB1_P2_EY_H           0x163f //0F
#define CPCB1_P2_CR1_SX_L       0x1640 //FF
#define CPCB1_P2_CR1_SX_H       0x1641 //0F
#define CPCB1_P2_CR1_EX_L       0x1642 //FF
#define CPCB1_P2_CR1_EX_H       0x1643 //0F
#define CPCB1_P2_CR1_SY_L       0x1644 //FF
#define CPCB1_P2_CR1_SY_H       0x1645 //0F
#define CPCB1_P2_CR1_EY_L       0x1646 //FF
#define CPCB1_P2_CR1_EY_H       0x1647 //0F
//#define CPCB1_P2_CR2_SX_L       0x1648 //NOT NEEDED
//#define CPCB1_P2_CR2_SX_H       0x1649 //NOT NEEDED
//#define CPCB1_P2_CR2_EX_L       0x164a //NOT NEEDED
//#define CPCB1_P2_CR2_EX_H       0x164b //NOT NEEDED
//#define CPCB1_P2_CR2_SY_L       0x164c //NOT NEEDED
//#define CPCB1_P2_CR2_SY_H       0x164d //NOT NEEDED
//#define CPCB1_P2_CR2_EY_L       0x164e //NOT NEEDED
//#define CPCB1_P2_CR2_EY_H       0x164f //NOT NEEDED
#define CPCB1_P3_SX_L           0x1650 //FF
#define CPCB1_P3_SX_H           0x1651 //0F
#define CPCB1_P3_EX_L           0x1652 //FF
#define CPCB1_P3_EX_H           0x1653 //0F
#define CPCB1_P3_SY_L           0x1654 //FF
#define CPCB1_P3_SY_H           0x1655 //0F
#define CPCB1_P3_EY_L           0x1656 //FF
#define CPCB1_P3_EY_H           0x1657 //0F
#define CPCB1_P3A_SX_L          0x1658 //FF
#define CPCB1_P3A_SX_H          0x1659 //0F
#define CPCB1_P3A_EX_L          0x165a //FF
#define CPCB1_P3A_EX_H          0x165b //0F
#define CPCB1_P3A_SY_L          0x165c //FF
#define CPCB1_P3A_SY_H          0x165d //0F
#define CPCB1_P3A_EY_L          0x165e //FF
#define CPCB1_P3A_EY_H          0x165f //0F
#define CPCB1_P4_SX_L           0x1660 //FF
#define CPCB1_P4_SX_H           0x1661 //0F
#define CPCB1_P4_EX_L           0x1662 //FF
#define CPCB1_P4_EX_H           0x1663 //0F
#define CPCB1_P4_SY_L           0x1664 //FF
#define CPCB1_P4_SY_H           0x1665 //0F
#define CPCB1_P4_EY_L           0x1666 //FF
#define CPCB1_P4_EY_H           0x1667 //0F
#define CPCB1_P5_SX_L           0x1668 //FF
#define CPCB1_P5_SX_H           0x1669 //0F
#define CPCB1_P5_EX_L           0x166a //FF
#define CPCB1_P5_EX_H           0x166b //0F
#define CPCB1_P5_SY_L           0x166c //FF
#define CPCB1_P5_SY_H           0x166d //0F
#define CPCB1_P5_EY_L           0x166e //FF
#define CPCB1_P5_EY_H           0x166f //0F
#define CPCB1_P6_SX_L           0x1670 //FF
#define CPCB1_P6_SX_H           0x1671 //0F
#define CPCB1_P6_EX_L           0x1672 //FF
#define CPCB1_P6_EX_H           0x1673 //0F
#define CPCB1_P6_SY_L           0x1674 //FF
#define CPCB1_P6_SY_H           0x1675 //0F
#define CPCB1_P6_EY_L           0x1676 //FF
#define CPCB1_P6_EY_H           0x1677 //0F
#define CPCB1_P7_SX_L           0x1678 //FF
#define CPCB1_P7_SX_H           0x1679 //0F
#define CPCB1_P7_EX_L           0x167A //FF
#define CPCB1_P7_EX_H           0x167B //0F
#define CPCB1_P7_SY_L           0x167C //FF
#define CPCB1_P7_SY_H           0x167D //0F
#define CPCB1_P7_EY_L           0x167E //FF
#define CPCB1_P7_EY_H           0x167F //0F
#define CPCB1_P7A_SX_L          0x1680 //FF
#define CPCB1_P7A_SX_H          0x1681 //0F
#define CPCB1_P7A_EX_L          0x1682 //FF
#define CPCB1_P7A_EX_H          0x1683 //0F
#define CPCB1_P7A_SY_L          0x1684 //FF
#define CPCB1_P7A_SY_H          0x1685 //0F
#define CPCB1_P7A_EY_L          0x1686 //FF
#define CPCB1_P7A_EY_H          0x1687 //0F
#define CPCB1_VO_BASE_CLR_0     0x1688 //FF
#define CPCB1_VO_BASE_CLR_1     0x1689 //FF
#define CPCB1_VO_BASE_CLR_2     0x168A //FF
#define CPCB1_VO_WEIGHT         0x168F //FF
////////////////////////////////////////////////////
#define CPCB1_OO_LAY12          0x16EA //FF
#define CPCB1_OO_LAY34          0x16EB //FF
#define CPCB1_OO_FIX_0          0x16EC //FF
#define CPCB1_OO_FIX_1          0x16ED //FF
#define CPCB1_OO_FIX_2          0x16EE //FF
#define CPCB1_OO_FIX0_0         0x16F0 //FF
#define CPCB1_OO_FIX0_1         0x16F1 //FF
#define CPCB1_OO_FIX0_2         0x16F2 //FF
#define CPCB1_OO_FIX1_0         0x16F4 //FF
#define CPCB1_OO_FIX1_1         0x16F5 //FF
#define CPCB1_OO_FIX1_2         0x16F6 //FF
#define CPCB1_OO_FIX2_0         0x16F8 //FF
#define CPCB1_OO_FIX2_1         0x16F9 //FF
#define CPCB1_OO_FIX2_2         0x16FA //FF
#define CPCB1_OO_FIX3_0         0x16FC //FF
#define CPCB1_OO_FIX3_1         0x16FD //FF
#define CPCB1_OO_FIX3_2         0x16FE //FF
#define CPCB1_OO_CTRL           0x1700 //03
#define CPCB1_OO_P1_AL          0x1701 //FF
#define CPCB1_OO_P2_AL          0x1702 //FF
#define CPCB1_OO_FIX3A_0        0x1704 //FF
#define CPCB1_OO_FIX3A_1        0x1705 //FF
#define CPCB1_OO_FIX3A_2        0x1706 //FF

//CPCB2_INT
#define  CPCB2_INT_CTRL         0x1710 //0E
#define  CPCB2_INT_MEMSIZE_L    0x1712 //FF
#define  CPCB2_INT_MEMSIZE_H    0x1713 //07
#define  CPCB2_INT_WF_L         0x1714 //FF
#define  CPCB2_INT_WF_H         0x1715 //01
#define  CPCB2_INT_FX_L         0x1716 //FF
#define  CPCB2_INT_FX_H         0x1717 //0F
#define  CPCB2_INT_FY_L         0x1718 //FF
#define  CPCB2_INT_FY_H         0x1719 //0F
#define  CPCB2_INT_V_S_L        0x171a //FF
#define  CPCB2_INT_V_S_H        0x171b //0F
#define  CPCB2_INT_V_E_L        0x171c //FF
#define  CPCB2_INT_V_E_H        0x171d //0F
#define  CPCB2_INT_H_S_L        0x171e //FF
#define  CPCB2_INT_H_S_H        0x171f //0F
#define  CPCB2_INT_H_E_L        0x1720 //FF
#define  CPCB2_INT_H_E_H        0x1721 //0F
#define  CPCB2_INT_VX_E_L       0x1722 //FF
#define  CPCB2_INT_VX_E_H       0x1723 //0F
#define  CPCB2_INT_VX_O_L       0x1724 //FF
#define  CPCB2_INT_VX_O_H       0x1725 //0F
#define  CPCB2_INT_YBL_L        0x1726 //FF
#define  CPCB2_INT_YBL_H        0x1727 //0F
#define  CPCB2_INT_CBL_L        0x1728 //FF
#define  CPCB2_INT_CBL_H        0x1729 //0F


//--CPCB2  1730
#define CPCB2_MODE              0x1731 //NOT NEEDED
#define CPCB2_LWIN              0x1732 //NOT NEEDED
#define CPCB2_FT                0x1733 //NOT NEEDED
#define CPCB2_VT_L              0x1734 //FF
#define CPCB2_VT_H              0x1735 //0F
#define CPCB2_HT_L              0x1736 //FF
#define CPCB2_HT_H              0x1737 //0F
#define CPCB2_XI_L              0x1738 //NOT NEEDED
#define CPCB2_XI_H              0x1739 //NOT NEEDED
#define CPCB2_YI_L              0x173a //NOT NEEDED
#define CPCB2_YI_H              0x173b //NOT NEEDED
#define CPCB2_VX_L              0x173c //FF
#define CPCB2_VX_H              0x173d //0F
#define CPCB2_FRST_L            0x173e //FF
#define CPCB2_FRST_H            0x173f //0F
#define CPCB2_HS_S_L            0x1740 //FF
#define CPCB2_HS_S_H            0x1741 //0F
#define CPCB2_HS_E_L            0x1742 //FF
#define CPCB2_HS_E_H            0x1743 //0F
#define CPCB2_VS_S_L            0x1744 //FF
#define CPCB2_VS_S_H            0x1745 //0F
#define CPCB2_VS_E_L            0x1746 //FF
#define CPCB2_VS_E_H            0x1747 //0F
#define CPCB2_P0_SX_L           0x1748 //FF
#define CPCB2_P0_SX_H           0x1749 //0F
#define CPCB2_P0_EX_L           0x174a //FF
#define CPCB2_P0_EX_H           0x174b //0F
#define CPCB2_P0_SY_L           0x174c //FF
#define CPCB2_P0_SY_H           0x174d //0F
#define CPCB2_P0_EY_L           0x174e //FF
#define CPCB2_P0_EY_H           0x174f //0F
#define CPCB2_P1_SX_L           0x1750 //FF
#define CPCB2_P1_SX_H           0x1751 //0F
#define CPCB2_P1_EX_L           0x1752 //FF
#define CPCB2_P1_EX_H           0x1753 //0F
#define CPCB2_P1_SY_L           0x1754 //FF
#define CPCB2_P1_SY_H           0x1755 //0F
#define CPCB2_P1_EY_L           0x1756 //FF
#define CPCB2_P1_EY_H           0x1757 //0F
#define CPCB2_P4_SX_L           0x1780 //FF
#define CPCB2_P4_SX_H           0x1781 //0F
#define CPCB2_P4_EX_L           0x1782 //FF
#define CPCB2_P4_EX_H           0x1783 //0F
#define CPCB2_P4_SY_L           0x1784 //FF
#define CPCB2_P4_SY_H           0x1785 //0F
#define CPCB2_P4_EY_L           0x1786 //FF
#define CPCB2_P4_EY_H           0x1787 //0F
#define CPCB2_P5_SX_L           0x1788 //FF
#define CPCB2_P5_SX_H           0x1789 //0F
#define CPCB2_P5_EX_L           0x178A //FF
#define CPCB2_P5_EX_H           0x178B //0F
#define CPCB2_P5_SY_L           0x1790 //FF
#define CPCB2_P5_SY_H           0x1791 //0F
#define CPCB2_P5_EY_L           0x1792 //FF
#define CPCB2_P5_EY_H           0x1793 //0F
///////////////////////////////////////////////////
#define CPCB2_OO_FIX_0          0x17BC //FF
#define CPCB2_OO_FIX_1          0x17BD //FF
#define CPCB2_OO_FIX_2          0x17BE //FF
#define CPCB2_OO_FIX0_0         0x17C0 //FF
#define CPCB2_OO_FIX0_1         0x17C1 //FF
#define CPCB2_OO_FIX0_2         0x17C2 //FF
#define CPCB2_OO_FIX1_0         0x17C4 //FF
#define CPCB2_OO_FIX1_1         0x17C5 //FF
#define CPCB2_OO_FIX1_2         0x17C6 //FF
#define CPCB2_TG_FREEZE         0x17C7 //03

//CMU REGS
#define CMU_ICSC_M_C0_L                  0x1A00 //FF
#define CMU_ICSC_M_C0_H                  0x1A01 //3F
#define CMU_ICSC_M_C1_L                  0x1A02 //FF
#define CMU_ICSC_M_C1_H                  0x1A03 //3F
#define CMU_ICSC_M_C2_L                  0x1A04 //FF
#define CMU_ICSC_M_C2_H                  0x1A05 //3F
#define CMU_ICSC_M_C3_L                  0x1A06 //FF
#define CMU_ICSC_M_C3_H                  0x1A07 //3F
#define CMU_ICSC_M_C4_L                  0x1A08 //FF
#define CMU_ICSC_M_C4_H                  0x1A09 //3F
#define CMU_ICSC_M_C5_L                  0x1A0a //FF
#define CMU_ICSC_M_C5_H                  0x1A0b //3F
#define CMU_ICSC_M_C6_L                  0x1A0c //FF
#define CMU_ICSC_M_C6_H                  0x1A0d //3F
#define CMU_ICSC_M_C7_L                  0x1A0e //FF
#define CMU_ICSC_M_C7_H                  0x1A0f //3F
#define CMU_ICSC_M_C8_L                  0x1A10 //FF
#define CMU_ICSC_M_C8_H                  0x1A11 //3F

#define CMU_ICSC_P_C0_L                  0x1A12 //FF
#define CMU_ICSC_P_C0_H                  0x1A13 //3F
#define CMU_ICSC_P_C1_L                  0x1A14 //FF
#define CMU_ICSC_P_C1_H                  0x1A15 //3F
#define CMU_ICSC_P_C2_L                  0x1A16 //FF
#define CMU_ICSC_P_C2_H                  0x1A17 //3F
#define CMU_ICSC_P_C3_L                  0x1A18 //FF
#define CMU_ICSC_P_C3_H                  0x1A19 //3F
#define CMU_ICSC_P_C4_L                  0x1A1a //FF
#define CMU_ICSC_P_C4_H                  0x1A1b //3F
#define CMU_ICSC_P_C5_L                  0x1A1c //FF
#define CMU_ICSC_P_C5_H                  0x1A1d //3F
#define CMU_ICSC_P_C6_L                  0x1A1e //FF
#define CMU_ICSC_P_C6_H                  0x1A1f //3F
#define CMU_ICSC_P_C7_L                  0x1A20 //FF
#define CMU_ICSC_P_C7_H                  0x1A21 //3F
#define CMU_ICSC_P_C8_L                  0x1A22 //FF
#define CMU_ICSC_P_C8_H                  0x1A23 //3F

#define CMU_ICSC_M_O1_0                  0x1A24 //FF
#define CMU_ICSC_M_O1_1                  0x1A25 //FF
#define CMU_ICSC_M_O1_2                  0x1A26 //03

#define CMU_ICSC_M_O2_0                  0x1A28 //FF
#define CMU_ICSC_M_O2_1                  0x1A29 //FF
#define CMU_ICSC_M_O2_2                  0x1A2a //03

#define CMU_ICSC_M_O3_0                  0x1A2c //FF
#define CMU_ICSC_M_O3_1                  0x1A2d //FF
#define CMU_ICSC_M_O3_2                  0x1A2e //03

#define CMU_ICSC_P_O1_0                  0x1A30 //FF
#define CMU_ICSC_P_O1_1                  0x1A31 //FF
#define CMU_ICSC_P_O1_2                  0x1A32 //03

#define CMU_ICSC_P_O2_0                  0x1A34 //FF
#define CMU_ICSC_P_O2_1                  0x1A35 //FF
#define CMU_ICSC_P_O2_2                  0x1A36 //03

#define CMU_ICSC_P_O3_0                  0x1A38 //FF
#define CMU_ICSC_P_O3_1                  0x1A39 //FF
#define CMU_ICSC_P_O3_2                  0x1A3a //03

#define CMU_FTDC_M_EN                    0x1A3c //01
#define CMU_FTDC_P_EN                    0x1A3d //01
#define CMU_FTDC_INLOW_L                 0x1A40 //FF
#define CMU_FTDC_INLOW_H                 0x1A41 //01
#define CMU_FTDC_INHIGH_L                0x1A42 //FF
#define CMU_FTDC_INHIGH_H                0x1A43 //01
#define CMU_FTDC_OUTLOW_L                0x1A44 //FF
#define CMU_FTDC_OUTLOW_H                0x1A45 //01
#define CMU_FTDC_OUTHIGH_L               0x1A46 //FF
#define CMU_FTDC_OUTHIGH_H               0x1A47 //01
#define CMU_FTDC_YLOW                    0x1A48 //FF
#define CMU_FTDC_YHIGH                   0x1A49 //FF
#define CMU_FTDC_CH1                     0x1A4a //FF
#define CMU_FTDC_CH2_L                   0x1A4c //FF
#define CMU_FTDC_CH2_H                   0x1A4d //0F
#define CMU_FTDC_CH3_L                   0x1A4e //FF
#define CMU_FTDC_CH3_H                   0x1A4f //0F
#define CMU_FTDC_1_C00                   0x1A50 //FF
#define CMU_FTDC_2_C00                   0x1A51 //FF
#define CMU_FTDC_3_C00                   0x1A52 //FF
#define CMU_FTDC_4_C00                   0x1A53 //FF
#define CMU_FTDC_5_C00                   0x1A54 //FF
#define CMU_FTDC_6_C00                   0x1A55 //FF
#define CMU_FTDC_1_C01                   0x1A56 //FF
#define CMU_FTDC_2_C01                   0x1A57 //FF
#define CMU_FTDC_3_C01                   0x1A58 //FF
#define CMU_FTDC_4_C01                   0x1A59 //FF
#define CMU_FTDC_5_C01                   0x1A5a //FF
#define CMU_FTDC_6_C01                   0x1A5b //FF
#define CMU_FTDC_1_C10                   0x1A5c //FF
#define CMU_FTDC_2_C10                   0x1A5d //FF
#define CMU_FTDC_3_C10                   0x1A5e //FF
#define CMU_FTDC_4_C10                   0x1A5f //FF
#define CMU_FTDC_5_C10                   0x1A60 //FF
#define CMU_FTDC_6_C10                   0x1A61 //FF
#define CMU_FTDC_1_C11                   0x1A62 //FF
#define CMU_FTDC_2_C11                   0x1A63 //FF
#define CMU_FTDC_3_C11                   0x1A64 //FF
#define CMU_FTDC_4_C11                   0x1A65 //FF
#define CMU_FTDC_5_C11                   0x1A66 //FF
#define CMU_FTDC_6_C11                   0x1A67 //FF
#define CMU_FTDC_1_OFF00                 0x1A68 //1F
#define CMU_FTDC_2_OFF00                 0x1A69 //1F
#define CMU_FTDC_3_OFF00                 0x1A6a //1F
#define CMU_FTDC_4_OFF00                 0x1A6b //1F
#define CMU_FTDC_5_OFF00                 0x1A6c //1F
#define CMU_FTDC_6_OFF00                 0x1A6d //1F
#define CMU_FTDC_1_OFF10                 0x1A6e //1F
#define CMU_FTDC_2_OFF10                 0x1A6f //1F
#define CMU_FTDC_3_OFF10                 0x1A70 //1F
#define CMU_FTDC_4_OFF10                 0x1A71 //1F
#define CMU_FTDC_5_OFF10                 0x1A72 //1F
#define CMU_FTDC_6_OFF10                 0x1A73 //1F
#define CMU_PIXVAL_M_EN                  0x1A74 //03
#define CMU_PIXVAL_P_EN                  0x1A75 //03

#define CMU_HS_M_EN                      0x1A81 //1F
#define CMU_HS_M_AX1_L                   0x1A82 //FF
#define CMU_HS_M_AX1_H                   0x1A83 //01
#define CMU_HS_M_AX2_L                   0x1A84 //FF
#define CMU_HS_M_AX2_H                   0x1A85 //01
#define CMU_HS_M_AX3_L                   0x1A86 //FF
#define CMU_HS_M_AX3_H                   0x1A87 //01
#define CMU_HS_M_AX4_L                   0x1A88 //FF
#define CMU_HS_M_AX4_H                   0x1A89 //01
#define CMU_HS_M_AX5_L                   0x1A8A //FF
#define CMU_HS_M_AX5_H                   0x1A8B //01
#define CMU_HS_M_AX6_L                   0x1A8C //FF
#define CMU_HS_M_AX6_H                   0x1A8D //01
#define CMU_HS_M_AX7_L                   0x1A8E //FF
#define CMU_HS_M_AX7_H                   0x1A8F //01
#define CMU_HS_M_AX8_L                   0x1A90 //FF
#define CMU_HS_M_AX8_H                   0x1A91 //01
#define CMU_HS_M_AX9_L                   0x1A92 //FF
#define CMU_HS_M_AX9_H                   0x1A93 //01
#define CMU_HS_M_AX10_L                  0x1A94 //FF
#define CMU_HS_M_AX10_H                  0x1A95 //01
#define CMU_HS_M_AX11_L                  0x1A96 //FF
#define CMU_HS_M_AX11_H                  0x1A97 //01
#define CMU_HS_M_AX12_L                  0x1A98 //FF
#define CMU_HS_M_AX12_H                  0x1A99 //01
#define CMU_HS_M_AX13_L                  0x1A9A //FF
#define CMU_HS_M_AX13_H                  0x1A9B //01
#define CMU_HS_M_AX14_L                  0x1A9C //FF
#define CMU_HS_M_AX14_H                  0x1A9D //01
#define CMU_HS_M_H1                      0x1A9E //1F
#define CMU_HS_M_H2                      0x1A9F //1F
#define CMU_HS_M_H3                      0x1AA0 //1F
#define CMU_HS_M_H4                      0x1AA1 //1F
#define CMU_HS_M_H5                      0x1AA2 //1F
#define CMU_HS_M_H6                      0x1AA3 //1F
#define CMU_HS_M_H7                      0x1AA4 //1F
#define CMU_HS_M_H8                      0x1AA5 //1F
#define CMU_HS_M_H9                      0x1AA6 //1F
#define CMU_HS_M_H10                     0x1AA7 //1F
#define CMU_HS_M_H11                     0x1AA8 //1F
#define CMU_HS_M_H12                     0x1AA9 //1F
#define CMU_HS_M_H13                     0x1AAA //1F
#define CMU_HS_M_H14                     0x1AAB //1F
#define CMU_HS_M_S1                      0x1AAC //FF
#define CMU_HS_M_S2                      0x1AAD //FF
#define CMU_HS_M_S3                      0x1AAE //FF
#define CMU_HS_M_S4                      0x1AAF //FF
#define CMU_HS_M_S5                      0x1AB0 //FF
#define CMU_HS_M_S6                      0x1AB1 //FF
#define CMU_HS_M_S7                      0x1AB2 //FF
#define CMU_HS_M_S8                      0x1AB3 //FF
#define CMU_HS_M_S9                      0x1AB4 //FF
#define CMU_HS_M_S10                     0x1AB5 //FF
#define CMU_HS_M_S11                     0x1AB6 //FF
#define CMU_HS_M_S12                     0x1AB7 //FF
#define CMU_HS_M_S13                     0x1AB8 //FF
#define CMU_HS_M_S14                     0x1AB9 //FF
#define CMU_HS_M_GL                      0x1ABA //FF
#define CMU_HS_M_MAXSAT_RGBY_L           0x1ABC //FF
#define CMU_HS_M_MAXSAT_RGBY_H           0x1ABD //07
#define CMU_HS_M_MAXSAT_RCR_L            0x1ABE //FF
#define CMU_HS_M_MAXSAT_RCR_H            0x1ABF //0F
#define CMU_HS_M_MAXSAT_RCB_L            0x1AC0 //FF
#define CMU_HS_M_MAXSAT_RCB_H            0x1AC1 //0F
#define CMU_HS_M_MAXSAT_GCR_L            0x1AC2 //FF
#define CMU_HS_M_MAXSAT_GCR_H            0x1AC3 //0F
#define CMU_HS_M_MAXSAT_GCB_L            0x1AC4 //FF
#define CMU_HS_M_MAXSAT_GCB_H            0x1AC5 //0F
#define CMU_HS_M_MAXSAT_BCR_L            0x1AC6 //FF
#define CMU_HS_M_MAXSAT_BCR_H            0x1AC7 //0F
#define CMU_HS_M_MAXSAT_BCB_L            0x1AC8 //FF
#define CMU_HS_M_MAXSAT_BCB_H            0x1AC9 //0F
#define CMU_HS_M_MAXSAT_ROFF_L           0x1ACA //FF
#define CMU_HS_M_MAXSAT_ROFF_H           0x1ACB //FF
#define CMU_HS_M_MAXSAT_GOFF_L           0x1ACC //FF
#define CMU_HS_M_MAXSAT_GOFF_H           0x1ACD //FF
#define CMU_HS_M_MAXSAT_BOFF_L           0x1ACE //FF
#define CMU_HS_M_MAXSAT_BOFF_H           0x1ACF //FF
// K2 CMU START
//#define CMU_HS_M_MAXSAT_CTRL             0x1AD0 //NOT NEEDED
//#define CMU_HS_M_MAXSAT_XX_L             0x1AD2 //NOT NEEDED
//#define CMU_HS_M_MAXSAT_XX_H             0x1AD3 //NOT NEEDED
//#define CMU_HS_M_MAXSAT_YY_L             0x1AD4 //NOT NEEDED
//#define CMU_HS_M_MAXSAT_YY_H             0x1AD5 //NOT NEEDED
//#define CMU_HS_M_MAXSAT_ZZ_L             0x1AD6 //NOT NEEDED
//#define CMU_HS_M_MAXSAT_ZZ_H             0x1AD7 //NOT NEEDED
//#define CMU_HS_M_MAXSAT_KXX              0x1AD8 //NOT NEEDED
//#define CMU_HS_M_MAXSAT_KYY              0x1AD9 //NOT NEEDED
//#define CMU_HS_M_MAXSAT_KZZ              0x1ADA //NOT NEEDED
//#define CMU_HS_M_MAXSAT_M00_L            0x1ADC //NOT NEEDED
//#define CMU_HS_M_MAXSAT_M00_H            0x1ADD //NOT NEEDED
//#define CMU_HS_M_MAXSAT_M01_L            0x1ADE //NOT NEEDED
//#define CMU_HS_M_MAXSAT_M01_H            0x1ADF //NOT NEEDED
//#define CMU_HS_M_MAXSAT_M02_L            0x1AE0 //NOT NEEDED
//#define CMU_HS_M_MAXSAT_M02_H            0x1AE1 //NOT NEEDED
//#define CMU_HS_M_MAXSAT_M10_L            0x1AE2 //NOT NEEDED
//#define CMU_HS_M_MAXSAT_M10_H            0x1AE3 //NOT NEEDED
//#define CMU_HS_M_MAXSAT_M11_L            0x1AE4 //NOT NEEDED
//#define CMU_HS_M_MAXSAT_M11_H            0x1AE5 //NOT NEEDED
//#define CMU_HS_M_MAXSAT_M12_L            0x1AE6 //NOT NEEDED
//#define CMU_HS_M_MAXSAT_M12_H            0x1AE7 //NOT NEEDED
//#define CMU_HS_M_MAXSAT_M20_L            0x1AE8 //NOT NEEDED
//#define CMU_HS_M_MAXSAT_M20_H            0x1AE9 //NOT NEEDED
//#define CMU_HS_M_MAXSAT_M21_L            0x1AEA //NOT NEEDED
//#define CMU_HS_M_MAXSAT_M21_H            0x1AEB //NOT NEEDED
//#define CMU_HS_M_MAXSAT_M22_L            0x1AEC //NOT NEEDED
//#define CMU_HS_M_MAXSAT_M22_H            0x1AED //NOT NEEDED
//#define CMU_HS_M_MAXSAT_ALPHA            0x1AEE //NOT NEEDED
//#define CMU_HS_M_MAXSAT_CST              0x1AEF //NOT NEEDED
// K2 CMU END

#define CMU_HS_P_EN                      0x1AF1 //1F
#define CMU_HS_P_AX1_L                   0x1AF2 //FF
#define CMU_HS_P_AX1_H                   0x1AF3 //01
#define CMU_HS_P_AX2_L                   0x1AF4 //FF
#define CMU_HS_P_AX2_H                   0x1AF5 //01
#define CMU_HS_P_AX3_L                   0x1AF6 //FF
#define CMU_HS_P_AX3_H                   0x1AF7 //01
#define CMU_HS_P_AX4_L                   0x1AF8 //FF
#define CMU_HS_P_AX4_H                   0x1AF9 //01
#define CMU_HS_P_AX5_L                   0x1AFA //FF
#define CMU_HS_P_AX5_H                   0x1AFB //01
#define CMU_HS_P_AX6_L                   0x1AFC //FF
#define CMU_HS_P_AX6_H                   0x1AFD //01
#define CMU_HS_P_AX7_L                   0x1AFE //FF
#define CMU_HS_P_AX7_H                   0x1AFF //01
#define CMU_HS_P_AX8_L                   0x1B00 //FF
#define CMU_HS_P_AX8_H                   0x1B01 //01
#define CMU_HS_P_AX9_L                   0x1B02 //FF
#define CMU_HS_P_AX9_H                   0x1B03 //01
#define CMU_HS_P_AX10_L                  0x1B04 //FF
#define CMU_HS_P_AX10_H                  0x1B05 //01
#define CMU_HS_P_AX11_L                  0x1B06 //FF
#define CMU_HS_P_AX11_H                  0x1B07 //01
#define CMU_HS_P_AX12_L                  0x1B08 //FF
#define CMU_HS_P_AX12_H                  0x1B09 //01
#define CMU_HS_P_AX13_L                  0x1B0A //FF
#define CMU_HS_P_AX13_H                  0x1B0B //01
#define CMU_HS_P_AX14_L                  0x1B0C //FF
#define CMU_HS_P_AX14_H                  0x1B0D //01
#define CMU_HS_P_H1                      0x1B0E //1F
#define CMU_HS_P_H2                      0x1B0F //1F
#define CMU_HS_P_H3                      0x1B10 //1F
#define CMU_HS_P_H4                      0x1B11 //1F
#define CMU_HS_P_H5                      0x1B12 //1F
#define CMU_HS_P_H6                      0x1B13 //1F
#define CMU_HS_P_H7                      0x1B14 //1F
#define CMU_HS_P_H8                      0x1B15 //1F
#define CMU_HS_P_H9                      0x1B16 //1F
#define CMU_HS_P_H10                     0x1B17 //1F
#define CMU_HS_P_H11                     0x1B18 //1F
#define CMU_HS_P_H12                     0x1B19 //1F
#define CMU_HS_P_H13                     0x1B1A //1F
#define CMU_HS_P_H14                     0x1B1B //1F
#define CMU_HS_P_S1                      0x1B1C //FF
#define CMU_HS_P_S2                      0x1B1D //FF
#define CMU_HS_P_S3                      0x1B1E //FF
#define CMU_HS_P_S4                      0x1B1F //FF
#define CMU_HS_P_S5                      0x1B20 //FF
#define CMU_HS_P_S6                      0x1B21 //FF
#define CMU_HS_P_S7                      0x1B22 //FF
#define CMU_HS_P_S8                      0x1B23 //FF
#define CMU_HS_P_S9                      0x1B24 //FF
#define CMU_HS_P_S10                     0x1B25 //FF
#define CMU_HS_P_S11                     0x1B26 //FF
#define CMU_HS_P_S12                     0x1B27 //FF
#define CMU_HS_P_S13                     0x1B28 //FF
#define CMU_HS_P_S14                     0x1B29 //FF
#define CMU_HS_P_GL                      0x1B2A //FF
#define CMU_HS_P_MAXSAT_RGBY_L           0x1B2C //FF
#define CMU_HS_P_MAXSAT_RGBY_H           0x1B2D //07
#define CMU_HS_P_MAXSAT_RCR_L            0x1B2E //FF
#define CMU_HS_P_MAXSAT_RCR_H            0x1B2F //0F
#define CMU_HS_P_MAXSAT_RCB_L            0x1B30 //FF
#define CMU_HS_P_MAXSAT_RCB_H            0x1B31 //0F
#define CMU_HS_P_MAXSAT_GCR_L            0x1B32 //FF
#define CMU_HS_P_MAXSAT_GCR_H            0x1B33 //0F
#define CMU_HS_P_MAXSAT_GCB_L            0x1B34 //FF
#define CMU_HS_P_MAXSAT_GCB_H            0x1B35 //0F
#define CMU_HS_P_MAXSAT_BCR_L            0x1B36 //FF
#define CMU_HS_P_MAXSAT_BCR_H            0x1B37 //0F
#define CMU_HS_P_MAXSAT_BCB_L            0x1B38 //FF
#define CMU_HS_P_MAXSAT_BCB_H            0x1B39 //0F
#define CMU_HS_P_MAXSAT_ROFF_L           0x1B3A //FF
#define CMU_HS_P_MAXSAT_ROFF_H           0x1B3B //FF
#define CMU_HS_P_MAXSAT_GOFF_L           0x1B3C //FF
#define CMU_HS_P_MAXSAT_GOFF_H           0x1B3D //FF
#define CMU_HS_P_MAXSAT_BOFF_L           0x1B3E //FF
#define CMU_HS_P_MAXSAT_BOFF_H           0x1B3F //FF

//K2 CMU START
//#define CMU_HS_P_MAXSAT_CTRL                  0x1B40 //NOT NEEDED
//#define CMU_HS_P_MAXSAT_XX_L             0x1B42 //NOT NEEDED
//#define CMU_HS_P_MAXSAT_XX_H             0x1B43 //NOT NEEDED
//#define CMU_HS_P_MAXSAT_YY_L             0x1B44 //NOT NEEDED
//#define CMU_HS_P_MAXSAT_YY_H             0x1B45 //NOT NEEDED
//#define CMU_HS_P_MAXSAT_ZZ_L             0x1B46 //NOT NEEDED
//#define CMU_HS_P_MAXSAT_ZZ_H             0x1B47 //NOT NEEDED
//#define CMU_HS_P_MAXSAT_KXX              0x1B48 //NOT NEEDED
//#define CMU_HS_P_MAXSAT_KYY              0x1B49 //NOT NEEDED
//#define CMU_HS_P_MAXSAT_KZZ              0x1B4a //NOT NEEDED
//#define CMU_HS_P_MAXSAT_M00_L            0x1B4C //NOT NEEDED
//#define CMU_HS_P_MAXSAT_M00_H            0x1B4D //NOT NEEDED
//#define CMU_HS_P_MAXSAT_M01_L            0x1B4E //NOT NEEDED
//#define CMU_HS_P_MAXSAT_M01_H            0x1B4F //NOT NEEDED
//#define CMU_HS_P_MAXSAT_M02_L            0x1B50 //NOT NEEDED
//#define CMU_HS_P_MAXSAT_M02_H            0x1B51 //NOT NEEDED
//#define CMU_HS_P_MAXSAT_M10_L            0x1B52 //NOT NEEDED
//#define CMU_HS_P_MAXSAT_M10_H            0x1B53 //NOT NEEDED
//#define CMU_HS_P_MAXSAT_M11_L            0x1B54 //NOT NEEDED
//#define CMU_HS_P_MAXSAT_M11_H            0x1B55 //NOT NEEDED
//#define CMU_HS_P_MAXSAT_M12_L            0x1B56 //NOT NEEDED
//#define CMU_HS_P_MAXSAT_M12_H            0x1B57 //NOT NEEDED
//#define CMU_HS_P_MAXSAT_M20_L            0x1B58 //NOT NEEDED
//#define CMU_HS_P_MAXSAT_M20_H            0x1B59 //NOT NEEDED
//#define CMU_HS_P_MAXSAT_M21_L            0x1B5A //NOT NEEDED
//#define CMU_HS_P_MAXSAT_M21_H            0x1B5B //NOT NEEDED
//#define CMU_HS_P_MAXSAT_M22_L            0x1B5C //NOT NEEDED
//#define CMU_HS_P_MAXSAT_M22_H            0x1B5D //NOT NEEDED
//#define CMU_HS_P_MAXSAT_ALPHA            0x1B5E //NOT NEEDED
//#define CMU_HS_P_MAXSAT_CST              0x1B5F //NOT NEEDED
//
//#define CMU_ICR_DEGAMMA_CTRL             0x1B70 //NOT NEEDED
//#define CMU_ICR_DEGAMMA_HCTRL            0x1B71 //NOT NEEDED
//#define CMU_ICR_DEGAMMA_HADDR_L          0x1B72 //NOT NEEDED
//#define CMU_ICR_DEGAMMA_HADDR_H          0x1B73 //NOT NEEDED
//#define CMU_ICR_DEGAMMA_HDATA_H_B        0x1B74 //NOT NEEDED
//#define CMU_ICR_DEGAMMA_HDATA_H_G        0x1B75 //NOT NEEDED
//#define CMU_ICR_DEGAMMA_HDATA_H_R        0x1B76 //NOT NEEDED
//#define CMU_ICR_DEGAMMA_HDATA_L          0x1B77 //NOT NEEDED
//#define CMU_ICR_DEGAMMA_LDATA            0x1B78 //NOT NEEDED
//#define CMU_ICR_GAMUT_CTRL               0x1B79 //NOT NEEDED
//#define CMU_ICR_GAMUT_HCTRL              0x1B7A //NOT NEEDED
//#define CMU_ICR_GAMUT_HADDR_L            0x1B7C //NOT NEEDED
//#define CMU_ICR_GAMUT_HADDR_H            0x1B7D //NOT NEEDED
//#define CMU_ICR_GAMUTS_HDATA_H           0x1B7E //NOT NEEDED
//#define CMU_ICR_GAMUTD_HDATA_H           0x1B7F //NOT NEEDED
//#define CMU_ICR_GAMUTSD_HDATA_L          0x1B80 //NOT NEEDED
//#define CMU_ICR_CHROMA_TYPE_CTRL         0x1B81 //NOT NEEDED
//#define CMU_ICR_CHROMA_TYPE_HCTRL        0x1B82 //NOT NEEDED
//#define CMU_ICR_CHROMA_TYPE_HADDR_L      0x1B84 //NOT NEEDED
//#define CMU_ICR_CHROMA_TYPE_HADDR_H      0x1B85 //NOT NEEDED
//#define CMU_ICR_CHROMA_TYPE_HDATA_L      0x1B86 //NOT NEEDED
//#define CMU_ICR_CHROMA_TYPE_HDATA_H      0x1B87 //NOT NEEDED
// K2 CMU END

#define CMU_GCSC_M_C0_L                  0x1B96 //FF
#define CMU_GCSC_M_C0_H                  0x1B97 //3F
#define CMU_GCSC_M_C1_L                  0x1B98 //FF
#define CMU_GCSC_M_C1_H                  0x1B99 //3F
#define CMU_GCSC_M_C2_L                  0x1B9A //FF
#define CMU_GCSC_M_C2_H                  0x1B9B //3F
#define CMU_GCSC_M_C3_L                  0x1B9C //FF
#define CMU_GCSC_M_C3_H                  0x1B9D //3F
#define CMU_GCSC_M_C4_L                  0x1B9E //FF
#define CMU_GCSC_M_C4_H                  0x1B9F //3F
#define CMU_GCSC_M_C5_L                  0x1BA0 //FF
#define CMU_GCSC_M_C5_H                  0x1BA1 //3F
#define CMU_GCSC_M_C6_L                  0x1BA2 //FF
#define CMU_GCSC_M_C6_H                  0x1BA3 //3F
#define CMU_GCSC_M_C7_L                  0x1BA4 //FF
#define CMU_GCSC_M_C7_H                  0x1BA5 //3F
#define CMU_GCSC_M_C8_L                  0x1BA6 //FF
#define CMU_GCSC_M_C8_H                  0x1BA7 //3F

#define CMU_GCSC_P_C0_L                  0x1BA8 //FF
#define CMU_GCSC_P_C0_H                  0x1BA9 //3F
#define CMU_GCSC_P_C1_L                  0x1BAA //FF
#define CMU_GCSC_P_C1_H                  0x1BAB //3F
#define CMU_GCSC_P_C2_L                  0x1BAC //FF
#define CMU_GCSC_P_C2_H                  0x1BAD //3F
#define CMU_GCSC_P_C3_L                  0x1BAE //FF
#define CMU_GCSC_P_C3_H                  0x1BAF //3F
#define CMU_GCSC_P_C4_L                  0x1BB0 //FF
#define CMU_GCSC_P_C4_H                  0x1BB1 //3F
#define CMU_GCSC_P_C5_L                  0x1BB2 //FF
#define CMU_GCSC_P_C5_H                  0x1BB3 //3F
#define CMU_GCSC_P_C6_L                  0x1BB4 //FF
#define CMU_GCSC_P_C6_H                  0x1BB5 //3F
#define CMU_GCSC_P_C7_L                  0x1BB6 //FF
#define CMU_GCSC_P_C7_H                  0x1BB7 //3F
#define CMU_GCSC_P_C8_L                  0x1BB8 //FF
#define CMU_GCSC_P_C8_H                  0x1BB9 //3F

#define CMU_GCSC_M_O1_0                  0x1BBC //FF
#define CMU_GCSC_M_O1_1                  0x1BBD //FF
#define CMU_GCSC_M_O1_2                  0x1BBE //03

#define CMU_GCSC_M_O2_0                  0x1BC0 //FF
#define CMU_GCSC_M_O2_1                  0x1BC1 //FF
#define CMU_GCSC_M_O2_2                  0x1BC2 //03

#define CMU_GCSC_M_O3_0                  0x1BC4 //FF
#define CMU_GCSC_M_O3_1                  0x1BC5 //FF
#define CMU_GCSC_M_O3_2                  0x1BC6 //03

#define CMU_GCSC_P_O1_0                  0x1BC8 //FF
#define CMU_GCSC_P_O1_1                  0x1BC9 //FF
#define CMU_GCSC_P_O1_2                  0x1BCA //03

#define CMU_GCSC_P_O2_0                  0x1BCC //FF
#define CMU_GCSC_P_O2_1                  0x1BCD //FF
#define CMU_GCSC_P_O2_2                  0x1BCE //03

#define CMU_GCSC_P_O3_0                  0x1BD0 //FF
#define CMU_GCSC_P_O3_1                  0x1BD1 //FF
#define CMU_GCSC_P_O3_2                  0x1BD2 //03

#define CMU_GAMMA_CTRL                   0x1BD4 //FF
#define CMU_GAMMA_HCTRL                  0x1BD5 //01
#define CMU_GAMMA_HADDR_L                0x1BD6 //FF
#define CMU_GAMMA_HADDR_H                0x1BD7 //07
#define CMU_GAMMA_LDATA                  0x1BD8 //3F
#define CMU_GAMMA_HDATA_L                0x1BDA //3F
#define CMU_GAMMA_HDATA_H_R              0x1BDB //FF
#define CMU_GAMMA_HDATA_H_G              0x1BDC //FF
#define CMU_GAMMA_HDATA_H_B              0x1BDD //FF


#define CMU_BR_M_EN                      0x1BF0 //FF
#define CMU_BR_M_TH1_L                   0x1BF2 //FF
#define CMU_BR_M_TH1_H                   0x1BF3 //03
#define CMU_BR_M_TH2_L                   0x1BF4 //FF
#define CMU_BR_M_TH2_H                   0x1BF5 //03
#define CMU_BR_P_EN                      0x1BF6 //FF
#define CMU_BR_P_TH1_L                   0x1BF8 //FF
#define CMU_BR_P_TH1_H                   0x1BF9 //03
#define CMU_BR_P_TH2_L                   0x1BFA //FF
#define CMU_BR_P_TH2_H                   0x1BFB //03

#define CMU_ACE_M_EN                     0x1C00 //1F
#define CMU_ACE_M_WFG1                   0x1C04 //1F
#define CMU_ACE_M_WFG2                   0x1C05 //1F
#define CMU_ACE_M_WFG3                   0x1C06 //1F
#define CMU_ACE_M_MS1                    0x1C08 //FF
#define CMU_ACE_M_MS2                    0x1C09 //FF
#define CMU_ACE_M_MS3                    0x1C0A //FF
#define CMU_ACE_M_TH0                    0x1C0C //FF
#define CMU_ACE_M_TH1                    0x1C0D //FF
#define CMU_ACE_M_TH2                    0x1C0E //FF
#define CMU_ACE_M_TH3                    0x1C0F //FF
#define CMU_ACE_M_TH4                    0x1C10 //FF
#define CMU_ACE_M_TH5                    0x1C11 //FF
#define CMU_ACE_M_OP0_L                  0x1C12 //FF
#define CMU_ACE_M_OP0_H                  0x1C13 //03
#define CMU_ACE_M_OP5_L                  0x1C14 //FF
#define CMU_ACE_M_OP5_H                  0x1C15 //03
#define CMU_ACE_M_GB2                    0x1C16 //FF
#define CMU_ACE_M_GB3                    0x1C17 //FF
#define CMU_ACE_P_EN                     0x1C18 //1F
#define CMU_ACE_P_WFG1                   0x1C1C //1F
#define CMU_ACE_P_WFG2                   0x1C1D //1F
#define CMU_ACE_P_WFG3                   0x1C1E //1F
#define CMU_ACE_P_MS1                    0x1C20 //FF
#define CMU_ACE_P_MS2                    0x1C21 //FF
#define CMU_ACE_P_MS3                    0x1C22 //FF
#define CMU_ACE_P_TH0                    0x1C24 //FF
#define CMU_ACE_P_TH1                    0x1C25 //FF
#define CMU_ACE_P_TH2                    0x1C26 //FF
#define CMU_ACE_P_TH3                    0x1C27 //FF
#define CMU_ACE_P_TH4                    0x1C28 //FF
#define CMU_ACE_P_TH5                    0x1C29 //FF
#define CMU_ACE_P_OP0_L                  0x1C2A //FF
#define CMU_ACE_P_OP0_H                  0x1C2B //03
#define CMU_ACE_P_OP5_L                  0x1C2C //FF
#define CMU_ACE_P_OP5_H                  0x1C2D //03
#define CMU_ACE_P_GB2                    0x1C2E //FF
#define CMU_ACE_P_GB3                    0x1C2F //FF

//////////////////////////////////////////////////////////////////////////
// QTC
#define BDE0                             0x1C30 //FF
#define BDE1                             0x1C31 //FF
#define BDE2                             0x1C32 //FF
#define BDE3                             0x1C33 //FF
#define BDE4                             0x1C34 //FF
#define BDE5                             0x1C35 //FF
#define BDE6                             0x1C36 //FF
#define BDE7                             0x1C37 //FF

/////////////////////////////////////////////////////////////////////////////
#define CPCB0_PL3_AL_CTRL0      0x1E00 //07
#define CPCB0_PL3_AL_CTRL1      0x1E01 //FF
#define CPCB0_PL3A_AL_CTRL0     0x1E02 //07
#define CPCB0_PL3A_AL_CTRL1     0x1E03 //FF
#define CPCB0_PL3B_AL_CTRL0     0x1E04 //07
#define CPCB0_PL3B_AL_CTRL1     0x1E05 //FF
#define CPCB0_PL3C_AL_CTRL0     0x1E06 //07
#define CPCB0_PL3C_AL_CTRL1     0x1E07 //FF
#define CPCB0_OO_P3D_AL         0x1E08 //FF

#define CPCB0_OO_P5_AL          0x1E0A //FF
#define CPCB0_OO_P6_AL          0x1E0B //FF
#define CPCB0_OO_P7_AL          0x1E0C //FF
#define CPCB0_OO_P7A_AL         0x1E0D //FF
#define CPCB0_OO_P7B_AL         0x1E0E //FF
#define CPCB0_OO_P7C_AL         0x1E0F //FF
#define CPCB0_OO_P7D_AL         0x1E10 //FF

#define CPCB0_P3B_SX_L          0x1E18 //FF
#define CPCB0_P3B_SX_H          0x1E19 //0F
#define CPCB0_P3B_EX_L          0x1E1A //FF
#define CPCB0_P3B_EX_H          0x1E1B //0F
#define CPCB0_P3B_SY_L          0x1E1C //FF
#define CPCB0_P3B_SY_H          0x1E1D //0F
#define CPCB0_P3B_EY_L          0x1E1E //FF
#define CPCB0_P3B_EY_H          0x1E1F //0F

#define CPCB0_P3C_SX_L          0x1E20 //FF
#define CPCB0_P3C_SX_H          0x1E21 //0F
#define CPCB0_P3C_EX_L          0x1E22 //FF
#define CPCB0_P3C_EX_H          0x1E23 //0F
#define CPCB0_P3C_SY_L          0x1E24 //FF
#define CPCB0_P3C_SY_H          0x1E25 //0F
#define CPCB0_P3C_EY_L          0x1E26 //FF
#define CPCB0_P3C_EY_H          0x1E27 //0F

#define CPCB0_P3D_SX_L          0x1E28 //FF
#define CPCB0_P3D_SX_H          0x1E29 //0F
#define CPCB0_P3D_EX_L          0x1E2A //FF
#define CPCB0_P3D_EX_H          0x1E2B //0F
#define CPCB0_P3D_SY_L          0x1E2C //FF
#define CPCB0_P3D_SY_H          0x1E2D //0F
#define CPCB0_P3D_EY_L          0x1E2E //FF
#define CPCB0_P3D_EY_H          0x1E2F //0F

#define CPCB0_P7B_SX_L          0x1E30 //FF
#define CPCB0_P7B_SX_H          0x1E31 //0F
#define CPCB0_P7B_EX_L          0x1E32 //FF
#define CPCB0_P7B_EX_H          0x1E33 //0F
#define CPCB0_P7B_SY_L          0x1E34 //FF
#define CPCB0_P7B_SY_H          0x1E35 //0F
#define CPCB0_P7B_EY_L          0x1E36 //FF
#define CPCB0_P7B_EY_H          0x1E37 //0F

#define CPCB0_P7C_SX_L          0x1E38 //FF
#define CPCB0_P7C_SX_H          0x1E39 //0F
#define CPCB0_P7C_EX_L          0x1E3A //FF
#define CPCB0_P7C_EX_H          0x1E3B //0F
#define CPCB0_P7C_SY_L          0x1E3C //FF
#define CPCB0_P7C_SY_H          0x1E3D //0F
#define CPCB0_P7C_EY_L          0x1E3E //FF
#define CPCB0_P7C_EY_H          0x1E3F //0F

#define CPCB0_P7D_SX_L          0x1E40 //FF
#define CPCB0_P7D_SX_H          0x1E41 //0F
#define CPCB0_P7D_EX_L          0x1E42 //FF
#define CPCB0_P7D_EX_H          0x1E43 //0F
#define CPCB0_P7D_SY_L          0x1E44 //FF
#define CPCB0_P7D_SY_H          0x1E45 //0F
#define CPCB0_P7D_EY_L          0x1E46 //FF
#define CPCB0_P7D_EY_H          0x1E47 //0F

#define CPCB0_OO_FIX3B_0        0x1E48 //FF
#define CPCB0_OO_FIX3B_1        0x1E49 //FF
#define CPCB0_OO_FIX3B_2        0x1E4A //FF

#define CPCB0_OO_FIX3C_0        0x1E4C //FF
#define CPCB0_OO_FIX3C_1        0x1E4D //FF
#define CPCB0_OO_FIX3C_2        0x1E4E //FF

#define CPCB0_OO_FIX3D_0        0x1E50 //FF
#define CPCB0_OO_FIX3D_1        0x1E51 //FF
#define CPCB0_OO_FIX3D_2        0x1E52 //FF

#define CPCB0_OO_LAY56          0x1E54 //FF
#define CPCB0_OO_LAY7           0x1E55 //07

#define CPCB0_VO_KEY_MIN_0      0x1E56 //FF
#define CPCB0_VO_KEY_MIN_1      0x1E57 //FF
#define CPCB0_VO_KEY_MIN_2      0x1E58 //FF
#define CPCB0_VO_KEY_MAX_0      0x1E59 //FF
#define CPCB0_VO_KEY_MAX_1      0x1E5A //FF
#define CPCB0_VO_KEY_MAX_2      0x1E5B //FF
#define CPCB0_VO_KEY_EN         0x1E5C //07

#define CPCB1_PL3_AL_CTRL0      0x1E64 //07
#define CPCB1_PL3_AL_CTRL1      0x1E65 //FF
#define CPCB1_PL3A_AL_CTRL0     0x1E66 //07
#define CPCB1_PL3A_AL_CTRL1     0x1E67 //FF
#define CPCB1_PL3B_AL_CTRL0     0x1E68 //07
#define CPCB1_PL3B_AL_CTRL1     0x1E69 //FF
#define CPCB1_PL3C_AL_CTRL0     0x1E6A //07
#define CPCB1_PL3C_AL_CTRL1     0x1E6B //FF
#define CPCB1_OO_P3D_AL         0x1E6C //FF

#define CPCB1_OO_P5_AL          0x1E6E //FF
#define CPCB1_OO_P6_AL          0x1E6F //FF
#define CPCB1_OO_P7_AL          0x1E70 //FF
#define CPCB1_OO_P7A_AL         0x1E71 //FF
#define CPCB1_OO_P7B_AL         0x1E72 //FF
#define CPCB1_OO_P7C_AL         0x1E73 //FF
#define CPCB1_OO_P7D_AL         0x1E74 //FF

#define CPCB1_P3B_SX_L          0x1E80 //FF
#define CPCB1_P3B_SX_H          0x1E81 //0F
#define CPCB1_P3B_EX_L          0x1E82 //FF
#define CPCB1_P3B_EX_H          0x1E83 //0F
#define CPCB1_P3B_SY_L          0x1E84 //FF
#define CPCB1_P3B_SY_H          0x1E85 //0F
#define CPCB1_P3B_EY_L          0x1E86 //FF
#define CPCB1_P3B_EY_H          0x1E87 //0F

#define CPCB1_P3C_SX_L          0x1E88 //FF
#define CPCB1_P3C_SX_H          0x1E89 //0F
#define CPCB1_P3C_EX_L          0x1E8A //FF
#define CPCB1_P3C_EX_H          0x1E8B //0F
#define CPCB1_P3C_SY_L          0x1E8C //FF
#define CPCB1_P3C_SY_H          0x1E8D //0F
#define CPCB1_P3C_EY_L          0x1E8E //FF
#define CPCB1_P3C_EY_H          0x1E8F //0F

#define CPCB1_P3D_SX_L          0x1E90 //FF
#define CPCB1_P3D_SX_H          0x1E91 //0F
#define CPCB1_P3D_EX_L          0x1E92 //FF
#define CPCB1_P3D_EX_H          0x1E93 //0F
#define CPCB1_P3D_SY_L          0x1E94 //FF
#define CPCB1_P3D_SY_H          0x1E95 //0F
#define CPCB1_P3D_EY_L          0x1E96 //FF
#define CPCB1_P3D_EY_H          0x1E97 //0F

#define CPCB1_P7B_SX_L          0x1E98 //FF
#define CPCB1_P7B_SX_H          0x1E99 //0F
#define CPCB1_P7B_EX_L          0x1E9A //FF
#define CPCB1_P7B_EX_H          0x1E9B //0F
#define CPCB1_P7B_SY_L          0x1E9C //FF
#define CPCB1_P7B_SY_H          0x1E9D //0F
#define CPCB1_P7B_EY_L          0x1E9E //FF
#define CPCB1_P7B_EY_H          0x1E9F //0F

#define CPCB1_P7C_SX_L          0x1EA0 //FF
#define CPCB1_P7C_SX_H          0x1EA1 //0F
#define CPCB1_P7C_EX_L          0x1EA2 //FF
#define CPCB1_P7C_EX_H          0x1EA3 //0F
#define CPCB1_P7C_SY_L          0x1EA4 //FF
#define CPCB1_P7C_SY_H          0x1EA5 //0F
#define CPCB1_P7C_EY_L          0x1EA6 //FF
#define CPCB1_P7C_EY_H          0x1EA7 //0F

#define CPCB1_P7D_SX_L          0x1EA8 //FF
#define CPCB1_P7D_SX_H          0x1EA9 //0F
#define CPCB1_P7D_EX_L          0x1EAA //FF
#define CPCB1_P7D_EX_H          0x1EAB //0F
#define CPCB1_P7D_SY_L          0x1EAC //FF
#define CPCB1_P7D_SY_H          0x1EAD //0F
#define CPCB1_P7D_EY_L          0x1EAE //FF
#define CPCB1_P7D_EY_H          0x1EAF //0F

#define CPCB1_OO_FIX3B_0        0x1EB0 //FF
#define CPCB1_OO_FIX3B_1        0x1EB1 //FF
#define CPCB1_OO_FIX3B_2        0x1EB2 //FF

#define CPCB1_OO_FIX3C_0        0x1EB4 //FF
#define CPCB1_OO_FIX3C_1        0x1EB5 //FF
#define CPCB1_OO_FIX3C_2        0x1EB6 //FF

#define CPCB1_OO_FIX3D_0        0x1EB8 //FF
#define CPCB1_OO_FIX3D_1        0x1EB9 //FF
#define CPCB1_OO_FIX3D_2        0x1EBA //FF

#define CPCB1_OO_LAY56          0x1EBC //FF
#define CPCB1_OO_LAY7           0x1EBD //07

#define CPCB1_VO_KEY_MIN_0      0x1EBE //FF
#define CPCB1_VO_KEY_MIN_1      0x1EBF //FF
#define CPCB1_VO_KEY_MIN_2      0x1EC0 //FF
#define CPCB1_VO_KEY_MAX_0      0x1EC1 //FF
#define CPCB1_VO_KEY_MAX_1      0x1EC2 //FF
#define CPCB1_VO_KEY_MAX_2      0x1EC3 //FF
#define CPCB1_VO_KEY_EN         0x1EC4 //07

#define CPCB1_PL1_UPS_CTRL      0x1EC5 //01
#define CPCB1_PL2_UPS_CTRL      0x1EC6 //01

#define CPCB1_AL_SENSE_CTRL0    0x1EC7 //7F
#define CPCB1_AL_SENSE_CTRL1    0x1EC8 //7F
#define CPCB1_P3_CR_SX_L        0x1EC9 //FF
#define CPCB1_P3_CR_SX_H        0x1ECA //0F
#define CPCB1_P3_CR_EX_L        0x1ECB //FF
#define CPCB1_P3_CR_EX_H        0x1ECC //0F
#define CPCB1_P3_CR_SY_L        0x1ECD //FF
#define CPCB1_P3_CR_SY_H        0x1ECE //0F
#define CPCB1_P3_CR_EY_L        0x1ECF //FF
#define CPCB1_P3_CR_EY_H        0x1ED0 //0F
#define CPCB1_P3A_CR_SX_L       0x1ED1 //FF
#define CPCB1_P3A_CR_SX_H       0x1ED2 //0F
#define CPCB1_P3A_CR_EX_L       0x1ED3 //FF
#define CPCB1_P3A_CR_EX_H       0x1ED4 //0F
#define CPCB1_P3A_CR_SY_L       0x1ED5 //FF
#define CPCB1_P3A_CR_SY_H       0x1ED6 //0F
#define CPCB1_P3A_CR_EY_L       0x1ED7 //FF
#define CPCB1_P3A_CR_EY_H       0x1ED8 //0F
#define CPCB1_TG_FREEZE         0x1ED9 //03

#define CPCB0_AL_SENSE_CTRL0    0x1EE0 //7F
#define CPCB0_AL_SENSE_CTRL1    0x1EE1 //7F
#define CPCB0_P3_CR_SX_L        0x1EE2 //FF
#define CPCB0_P3_CR_SX_H        0x1EE3 //0F
#define CPCB0_P3_CR_EX_L        0x1EE4 //FF
#define CPCB0_P3_CR_EX_H        0x1EE5 //0F
#define CPCB0_P3_CR_SY_L        0x1EE6 //FF
#define CPCB0_P3_CR_SY_H        0x1EE7 //0F
#define CPCB0_P3_CR_EY_L        0x1EE8 //FF
#define CPCB0_P3_CR_EY_H        0x1EE9 //0F
#define CPCB0_P3A_CR_SX_L       0x1EEA //FF
#define CPCB0_P3A_CR_SX_H       0x1EEB //0F
#define CPCB0_P3A_CR_EX_L       0x1EEC //FF
#define CPCB0_P3A_CR_EX_H       0x1EED //0F
#define CPCB0_P3A_CR_SY_L       0x1EEE //FF
#define CPCB0_P3A_CR_SY_H       0x1EEF //0F
#define CPCB0_P3A_CR_EY_L       0x1EF0 //FF
#define CPCB0_P3A_CR_EY_H       0x1EF1 //0F
#define CPCB0_TG_FREEZE         0x1EF2 //0F

#define CPCB0_P3C_CR_SX_L       0x1EF3 //FF
#define CPCB0_P3C_CR_SX_H       0x1EF4 //0F
#define CPCB0_P3C_CR_EX_L       0x1EF5 //FF
#define CPCB0_P3C_CR_EX_H       0x1EF6 //0F
#define CPCB0_P3C_CR_SY_L       0x1EF7 //FF
#define CPCB0_P3C_CR_SY_H       0x1EF8 //0F
#define CPCB0_P3C_CR_EY_L       0x1EF9 //FF
#define CPCB0_P3C_CR_EY_H       0x1EFA //0F

#define CPCB1_P3C_CR_SX_L       0x1F00 //FF
#define CPCB1_P3C_CR_SX_H       0x1F01 //0F
#define CPCB1_P3C_CR_EX_L       0x1F02 //FF
#define CPCB1_P3C_CR_EX_H       0x1F03 //0F
#define CPCB1_P3C_CR_SY_L       0x1F04 //FF
#define CPCB1_P3C_CR_SY_H       0x1F05 //0F
#define CPCB1_P3C_CR_EY_L       0x1F06 //FF
#define CPCB1_P3C_CR_EY_H       0x1F07 //0F

// CAR registers
#define CAR0                    0x1D00 //FF
#define CAR1                    0x1D01 //FF
#define CAR2                    0x1D02 //FF
#define CAR3                    0x1D03 //FF
#define CAR4                    0x1D04 //FF
#define CAR5                    0x1D05 //FF
#define CAR6                    0x1D06 //FF
#define CAR7                    0x1D07 //FF
#define CAR8                    0x1D08 //FF
#define CAR9                    0x1D09 //FF
#define CAR10                   0x1D0A //FF
#define CAR11                   0x1D0B //FF
#define CAR12                   0x1D0C //FF
#define CAR13                   0x1D0D //FF
#define CAR14                   0x1D0E //FF
#define CAR15                   0x1D0F //FF
#define CAR16                   0x1D10 //FF
#define CAR17                   0x1D11 //FF
#define CAR18                   0x1D12 //FF
#define CAR19                   0x1D13 //FF
#define CAR20                   0x1D14 //FF
#define CAR21                   0x1D15 //FF
#define CAR22                   0x1D16 //FF
#define CAR23                   0x1D17 //FF
#define CAR24                   0x1D18 //FF
#define CAR25                   0x1D19 //FF
#define CAR26                   0x1D1A //FF
#define CAR27                   0x1D1B //FF
#define CAR28                   0x1D1C //FF
#define CAR29                   0x1D1D //FF
#define CAR30                   0x1D1E //FF
#define CAR31                   0x1D1F //FF
#define CAR32                   0x1D20 //FF
#define CAR33                   0x1D21 //FF
#define CAR34                   0x1D22 //FF
#define CAR35                   0x1D23 //FF
#define CAR36                   0x1D24 //FF
#define CAR37                   0x1D25 //FF
#define CAR38                   0x1D26 //FF
#define CAR39                   0x1D27 //FF
#define CAR40                   0x1D28 //FF
#define CAR41                   0x1D29 //FF
#define CAR42                   0x1D2A //FF
#define CAR43                   0x1D2B //FF
#define CAR44                   0x1D2C //FF
#define CAR45                   0x1D2D //FF
#define CAR46                   0x1D2E //FF
#define CAR47                   0x1D2F //FF
#define CAR48                   0x1D30 //FF
#define CAR49                   0x1D31 //FF
#define CAR50                   0x1D32 //FF
#define CAR51                   0x1D33 //FF
#define CAR52                   0x1D34 //FF
#define CAR53                   0x1D35 //FF
#define CAR54                   0x1D36 //FF
#define CAR55                   0x1D37 //FF
#define CAR56                   0x1D38 //FF
#define CAR57                   0x1D39 //FF
#define CAR58                   0x1D3A //FF
#define CAR59                   0x1D3B //FF
#define CAR60                   0x1D3C //FF
#define CAR61                   0x1D3D //FF
#define CAR62                   0x1D3E //FF
#define CAR63                   0x1D3F //FF
#define CAR64                   0x1D40 //FF
#define CAR65                   0x1D41 //FF
#define CAR66                   0x1D42 //FF
#define CAR67                   0x1D43 //FF
#define CAR68                   0x1D44 //FF
#define CAR69                   0x1D45 //FF
#define CAR70                   0x1D46 //FF
#define CAR71                   0x1D47 //FF
#define CAR72                   0x1D48 //FF

#define CAR100                  0x1D60
#define CAR101                  0x1D61
#define CAR102                  0x1D62
#define CAR103                  0x1D63
#define CAR104                  0x1D64
#define CAR105                  0x1D65
#define CAR106                  0x1D66
#define CAR107                  0x1D67
#define CAR108                  0x1D68
#define CAR109                  0x1D69
#define CAR110                  0x1D6A
#define CAR111                  0x1D6B
#define CAR112                  0x1D6C
#define CAR113                  0x1D6D
#define CAR114                  0x1D6E
#define CAR115                  0x1D6F
#define CAR116                  0x1D70
#define CAR117                  0x1D71
#define CAR118                  0x1D72
#define CAR119                  0x1D73
#define CAR120                  0x1D74
#define CAR121                  0x1D75
#define CAR122                  0x1D76

// NEST registers
#define NEST_NEW_CTRL0          0x600 //FF
#define NEST_NEW_CTRL1          0x601 //FF
#define NEST_NEW_CTRL2          0x602 //FF
#define NEST_NEW_CTRL3          0x603 //FF
#define NEST_NEW_CTRL4          0x604 //FF
#define NEST_NEW_CTRL5          0x605 //FF
#define NEST_NEW_CTRL6          0x606 //FF
#define NEST_NEW_CTRL7          0x607 //FF
#define NEST_NEW_CTRL8          0x608 //FF
#define NEST_NEW_CTRL9          0x609 //FF
#define NEST_NEW_CTRL10         0x60A //FF
#define NEST_NEW_CTRL11         0x60B //FF
#define NEST_NEW_CTRL12         0x60C //FF
#define NEST_NEW_CTRL13         0x60D //FF
#define NEST_NEW_CTRL14         0x60E //FF
#define NEST_NEW_CTRL15         0x60F //FF
#define NEST_NEW_CTRL16         0x610 //FF
#define NEST_NEW_CTRL17         0x611 //FF
#define NEST_NEW_CTRL18         0x612 //FF
#define NEST_NEW_CTRL19         0x613 //FF
#define NEST_NEW_CTRL20         0x614 //FF
#define NEST_NEW_CTRL21         0x615 //FF
#define NEST_NEW_CTRL22         0x616 //FF
#define NEST_NEW_CTRL23         0x617 //FF
#define NEST_NEW_CTRL24         0x618 //FF
#define NEST_NEW_CTRL25         0x619 //FF
#define NEST_NEW_CTRL26         0x61A //FF
#define NEST_NEW_CTRL27         0x61B //FF
#define NEST_NEW_CTRL28         0x61C //FF
#define NEST_NEW_CTRL29         0x61D //FF
#define NEST_NEW_CTRL30         0x61E //FF
#define NEST_NEW_CTRL31         0x61F //FF
#define NEST_NEW_CTRL32         0x620 //FF
#define NEST_NEW_CTRL33         0x621 //FF
#define NEST_NEW_CTRL34         0x622 //FF
#define NEST_NEW_CTRL35         0x623 //FF
#define NEST_NEW_CTRL36         0x624 //FF
#define NEST_NEW_CTRL37         0x625 //FF
#define NEST_NEW_CTRL38         0x626 //FF
#define NEST_NEW_CTRL39         0x627 //FF
#define NEST_NEW_CTRL40         0x628 //FF
#define NEST_NEW_CTRL41         0x629 //FF
#define NEST_NEW_CTRL42         0x62A //FF
#define NEST_NEW_CTRL43         0x62B //FF
#define NEST_NEW_CTRL44         0x62C //FF
#define NEST_NEW_CTRL45         0x62D //FF
#define NEST_NEW_CTRL46         0x62E //FF
#define NEST_NEW_CTRL47         0x62F //FF
#define NEST_NEW_CTRL48         0x630 //FF
#define NEST_NEW_CTRL49         0x631 //FF
#define NEST_NEW_CTRL50         0x632 //FF
#define NEST_NEW_CTRL51         0x633 //FF
#define NEST_NEW_CTRL52         0x634 //FF
#define NEST_NEW_CTRL53         0x635 //FF
#define NEST_NEW_CTRL54         0x636 //FF
#define NEST_NEW_CTRL55         0x637 //FF
#define NEST_NEW_CTRL56         0x638 //FF
#define NEST_NEW_CTRL57         0x639 //FF
#define NEST_NEW_CTRL58         0x63A //FF
#define NEST_NEW_CTRL59         0x63B //FF
#define NEST_NEW_CTRL60         0x63C //FF
#define NEST_NEW_CTRL61         0x63D //FF
#define NEST_NEW_CTRL62         0x63E //FF
#define NEST_NEW_CTRL63         0x63F //FF
#define NEST_NEW_CTRL64         0x640 //FF
#define NEST_NEW_CTRL65         0x641 //FF
#define NEST_NEW_CTRL66         0x642 //FF
#define NEST_NEW_CTRL67         0x643 //FF
#define NEST_NEW_CTRL68         0x644 //FF
#define NEST_NEW_CTRL69         0x645 //FF
#define NEST_NEW_CTRL70         0x646 //FF
#define NEST_NEW_CTRL71         0x647 //FF
#define NEST_NEW_CTRL72         0x648 //FF
#define NEST_NEW_CTRL73         0x649 //FF
#define NEST_NEW_CTRL74         0x64A //FF
#define NEST_NEW_CTRL75         0x64B //FF
#define NEST_NEW_CTRL76         0x64C //FF
#define NEST_NEW_CTRL77         0x64D //FF
#define NEST_NEW_CTRL78         0x64E //FF
#define NEST_NEW_CTRL79         0x64F //FF
#define NEST_NEW_CTRL80         0x650 //FF
#define NEST_NEW_CTRL81         0x651 //FF
#define NEST_NEW_CTRL82         0x652 //FF
#define NEST_NEW_CTRL83         0x653 //FF
#define NEST_NEW_CTRL84         0x654 //FF
#define NEST_NEW_CTRL85         0x655 //FF
#define NEST_NEW_CTRL86         0x656 //FF
#define NEST_NEW_CTRL87         0x657 //FF
#define NEST_NEW_CTRL88         0x658 //FF
#define NEST_NEW_CTRL89         0x659 //FF
#define NEST_NEW_CTRL90         0x65A //FF
#define NEST_NEW_CTRL91         0x65B //FF
#define NEST_NEW_CTRL92         0x65C //FF
#define NEST_NEW_CTRL93         0x65D //FF
#define NEST_NEW_CTRL94         0x65E //FF
#define NEST_NEW_CTRL95         0x65F //FF
#define NEST_NEW_CTRL96         0x660 //FF
#define NEST_NEW_CTRL97         0x661 //FF
#define NEST_NEW_CTRL98         0x662 //FF
#define NEST_NEW_CTRL99         0x663 //FF
#define NEST_NEW_CTRL100        0x664 //FF
#define NEST_NEW_CTRL101        0x665 //FF
#define NEST_NEW_CTRL102        0x666 //FF
#define NEST_NEW_CTRL103        0x667 //FF
#define NEST_NEW_CTRL104        0x668 //FF
#define NEST_NEW_CTRL105        0x669 //FF
#define NEST_NEW_CTRL106        0x66A //FF
#define NEST_NEW_CTRL107        0x66B //FF
#define NEST_NEW_CTRL108        0x66C //FF
#define NEST_NEW_CTRL109        0x66D //FF
#define NEST_NEW_CTRL110        0x66E //FF
#define NEST_NEW_CTRL111        0x66F //FF
#define NEST_NEW_CTRL112        0x670 //FF
#define NEST_NEW_CTRL113        0x671 //FF
#define NEST_NEW_CTRL114        0x672 //FF
#define NEST_NEW_CTRL115        0x673 //FF
#define NEST_NEW_CTRL116        0x674 //FF
#define NEST_NEW_CTRL117        0x675 //FF
#define NEST_NEW_CTRL118        0x676 //FF
#define NEST_NEW_CTRL119        0x677 //FF

#define NEST_NEW_CTRL200        0x690
#define NEST_NEW_CTRL201        0x691
#define NEST_NEW_CTRL202        0x692
#define NEST_NEW_CTRL203        0x693
#define NEST_NEW_CTRL204        0x694
#define NEST_NEW_CTRL205        0x695
#define NEST_NEW_CTRL206        0x696
#define NEST_NEW_CTRL207        0x697
#define NEST_NEW_CTRL208        0x698
#define NEST_NEW_CTRL209        0x699
#define NEST_NEW_CTRL210        0x69A
#define NEST_NEW_CTRL211        0x69B
#define NEST_NEW_CTRL212        0x69C
#define NEST_NEW_CTRL213        0x69D
#define NEST_NEW_CTRL214        0x69E
#define NEST_NEW_CTRL215        0x69F
#define NEST_NEW_CTRL216        0x6A0
//-----------------------------------------------------------------------------
// VDOP - 0x110 regs
//-----------------------------------------------------------------------------

#define VOP_TTL_BASE                   VDOP_BASE+0x0000
#define VOP_LVDS_BASE                  VDOP_BASE+0x0030
#define VOP_LVDS1_BASE                 VDOP_BASE+0x0040
#define LVDS_DFIFO_BASE                VDOP_BASE+0x0090

// VDOP-TTL
#define VDOP_CTRL0_0                    0x0000
#define VDOP_CTRL1_0                    0x0001
#define VDOP_TTL_FRMT_0                 0x0002
#define VDOP_SIGACC_NOF_0               0x0003
#define VDOP_B_HITHR_0                  0x0004
#define VDOP_B_HITHR_1                  0x0005
#define VDOP_G_HITHR_0                  0x0006
#define VDOP_G_HITHR_1                  0x0007
#define VDOP_R_HITHR_0                  0x0008
#define VDOP_R_HITHR_1                  0x0009
#define VDOP_B_LOTHR_0                  0x000a
#define VDOP_B_LOTHR_1                  0x000b
#define VDOP_G_LOTHR_0                  0x000c
#define VDOP_G_LOTHR_1                  0x000d
#define VDOP_R_LOTHR_0                  0x000e
#define VDOP_R_LOTHR_1                  0x000f
#define VDOP_B_BLNK_DATA_0              0x0010
#define VDOP_B_BLNK_DATA_1              0x0011
#define VDOP_G_BLNK_DATA_0              0x0012
#define VDOP_G_BLNK_DATA_1              0x0013
#define VDOP_R_BLNK_DATA_0              0x0014
#define VDOP_R_BLNK_DATA_1              0x0015
#define VDOP_TTL_SIG_FLAGS_0            0x0016
#define VDOP_TTL_SIG_OP_0               0x0018
#define VDOP_TTL_SIG_OP_1               0x0019
#define VDOP_TTL_SIG_OP_2               0x001A
#define VDOP_TTL_SIG_OP_3               0x001B
#define VOP_LVDS_SEL                    0x001C
#define VOP_DIT_INP_RES                 0x001D
#define VOP_DIT_OUT_RES                 0x001E
#define VDOP_TTL_BIT_POL_INV_0          0x0020
#define VDOP_TTL_BIT_POL_INV_1          0x0021
#define VDOP_TTL_BIT_POL_INV_2          0x0022
#define VDOP_TTL_BIT_POL_INV_3          0x0023
#define VDOP_TTL_BIT_POL_INV_4          0x0024
#define VDOP_TD_CTRL0_0                 0x0026
#define VDOP_TD_CTRL1_0                 0x0027
#define VDOP_TD_UDATA_0                 0x0028
#define VDOP_TD_UDATA_1                 0x0029
#define VDOP_TD_MDATA_0                 0x002A
#define VDOP_TD_MDATA_1                 0x002B
#define VDOP_TD_LDATA_0                 0x002C
#define VDOP_TD_LDATA_1                 0x002D
#define VDOP_TD_LR_ADJ_CNT_0            0x002E
#define VDOP_TD_LR_ADJ_CNT_1            0x002F

// VDOP-LVDS
#define LVDS_CTRL_0                     0x0000
#define LVDS_TEST_DATA_CH0_0            0x0001
#define LVDS_TEST_DATA_CH1_0            0x0002
#define LVDS_TEST_DATA_CH2_0            0x0003
#define LVDS_TEST_DATA_CH3_0            0x0004
#define LVDS_TEST_DATA_CH4_0            0x0005
#define LVDS_TEST_DATA_CH5_0            0x0006
#define LVDS_TEST_DATA_CH6_0            0x0007
#define LVDS_TEST_DATA_CH7_0            0x0008
#define LVDS_TEST_DATA_CH8_0            0x0009
#define LVDS_TEST_DATA_CH9_0            0x000a
#define LVDS_TEST_DATA_CH10_0           0x000b
#define LVDS_TEST_DATA_CH11_0           0x000c

// VDOP-LVDS1
#define LVDS_D0SEL_CH0_0                0x0000
#define LVDS_D1SEL_CH0_0                0x0001
#define LVDS_D2SEL_CH0_0                0x0002
#define LVDS_D3SEL_CH0_0                0x0003
#define LVDS_D4SEL_CH0_0                0x0004
#define LVDS_D5SEL_CH0_0                0x0005
#define LVDS_D6SEL_CH0_0                0x0006
#define LVDS_D0SEL_CH1_0                0x0007
#define LVDS_D1SEL_CH1_0                0x0008
#define LVDS_D2SEL_CH1_0                0x0009
#define LVDS_D3SEL_CH1_0                0x000a
#define LVDS_D4SEL_CH1_0                0x000b
#define LVDS_D5SEL_CH1_0                0x000c
#define LVDS_D6SEL_CH1_0                0x000d
#define LVDS_D0SEL_CH2_0                0x000e
#define LVDS_D1SEL_CH2_0                0x000f
#define LVDS_D2SEL_CH2_0                0x0010
#define LVDS_D3SEL_CH2_0                0x0011
#define LVDS_D4SEL_CH2_0                0x0012
#define LVDS_D5SEL_CH2_0                0x0013
#define LVDS_D6SEL_CH2_0                0x0014
#define LVDS_D0SEL_CH3_0                0x0015
#define LVDS_D1SEL_CH3_0                0x0016
#define LVDS_D2SEL_CH3_0                0x0017
#define LVDS_D3SEL_CH3_0                0x0018
#define LVDS_D4SEL_CH3_0                0x0019
#define LVDS_D5SEL_CH3_0                0x001a
#define LVDS_D6SEL_CH3_0                0x001b
#define LVDS_D0SEL_CH4_0                0x001c
#define LVDS_D1SEL_CH4_0                0x001d
#define LVDS_D2SEL_CH4_0                0x001e
#define LVDS_D3SEL_CH4_0                0x001f
#define LVDS_D4SEL_CH4_0                0x0020
#define LVDS_D5SEL_CH4_0                0x0021
#define LVDS_D6SEL_CH4_0                0x0022
#define LVDS_D0SEL_CH5_0                0x0023
#define LVDS_D1SEL_CH5_0                0x0024
#define LVDS_D2SEL_CH5_0                0x0025
#define LVDS_D3SEL_CH5_0                0x0026
#define LVDS_D4SEL_CH5_0                0x0027
#define LVDS_D5SEL_CH5_0                0x0028
#define LVDS_D6SEL_CH5_0                0x0029
#define LVDS_D0SEL_CH6_0                0x002a
#define LVDS_D1SEL_CH6_0                0x002b
#define LVDS_D2SEL_CH6_0                0x002c
#define LVDS_D3SEL_CH6_0                0x002d
#define LVDS_D4SEL_CH6_0                0x002e
#define LVDS_D5SEL_CH6_0                0x002f
#define LVDS_D6SEL_CH6_0                0x0030
#define LVDS_D0SEL_CH7_0                0x0031
#define LVDS_D1SEL_CH7_0                0x0032
#define LVDS_D2SEL_CH7_0                0x0033
#define LVDS_D3SEL_CH7_0                0x0034
#define LVDS_D4SEL_CH7_0                0x0035
#define LVDS_D5SEL_CH7_0                0x0036
#define LVDS_D6SEL_CH7_0                0x0037
#define LVDS_D0SEL_CH8_0                0x0038
#define LVDS_D1SEL_CH8_0                0x0039
#define LVDS_D2SEL_CH8_0                0x003a
#define LVDS_D3SEL_CH8_0                0x003b
#define LVDS_D4SEL_CH8_0                0x003c
#define LVDS_D5SEL_CH8_0                0x003d
#define LVDS_D6SEL_CH8_0                0x003e
#define LVDS_D0SEL_CH9_0                0x003f
#define LVDS_D1SEL_CH9_0                0x0040
#define LVDS_D2SEL_CH9_0                0x0041
#define LVDS_D3SEL_CH9_0                0x0042
#define LVDS_D4SEL_CH9_0                0x0043
#define LVDS_D5SEL_CH9_0                0x0044
#define LVDS_D6SEL_CH9_0                0x0045

#define FIFO_CTRL_0                     0x0000
#define FIFO_MAX_OCC_0                  0x0001
#define FIFO_MAX_OCC_1                  0x0002
#define FIFO_MIN_OCC_0                  0x0003
#define FIFO_MIN_OCC_1                  0x0004
#define FIFO_OCC_STAT_0                 0x0005
#define FIFO_OCC_STAT_1                 0x0006

// SPDIF RX has been given with address 0x200 Please dont use the space!!!

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
#define VNR_NEST_CTRL            0x42e //3F
#define VNR_NOI_SIG              0x42f //FF
#define VNR_NEST_REG             0x430 //FF
#define VNR_NE_SAT               0x431 //7F
#define VNR_SNE_STATUS           0x432 //FF
#define VNR_TNE_STATUS           0x433 //FF
#define VNR_DEBUG                0x434 //1F

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

// Deinterlacer
#define DIN0                     0x500 //FF
#define DIN1                     0x501 //FF
#define DIN2                     0x502 //7F
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
#define DIN14                    0x50e //0F
#define DIN15                    0x50f //FF
#define DIN16                    0x510 //1F
#define DIN17                    0x511 //FF
#define DIN18                    0x512 //FF
#define DIN19                    0x513 //FF
#define DIN20                    0x514 //FF
#define DIN21                    0x515 //FF
#define DIN22                    0x516 //FF
#define DIN23                    0x517 //FF
#define DIN24                    0x518 //FF
#define DIN25                    0x519 //FF
#define DIN26                    0x51a //1F
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
#define DIN39                    0x527 //3F
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
#define DIN65                    0x543 //7F

#define DIN66                    0x544 //FF
#define DIN67                    0x545 //03
#define DIN68                    0x546 //0F
#define DIN69                    0x547 //FF
#define DIN70                    0x548 //FF
#define DIN71                    0x549 //FF
#define DIN72                    0x54a //FF
#define DIN73                    0x54b //FF
#define DIN74                    0x54c //FF
#define DIN75                    0x54d //FF
#define DIN76                    0x54e //1F
#define DIN77                    0x54f //FF
#define DIN78                    0x550 //FF
#define DIN79                    0x551 //FF
#define DIN80                    0x552 //FF
#define DIN81                    0x553 //3F
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
#define DIN106                   0x56e //03
#define DIN107                   0x56f //1F
#define DIN108                   0x570 //1F
#define DIN109                   0x571 //1F
#define DIN110                   0x572 //1F
#define DIN111                   0x573 //1F
#define DIN112                   0x574 //1F
#define DIN113                   0x575 //1F
#define DIN114                   0x576 //1F
#define DIN115                   0x577 //1F
#define DIN116                   0x578 //1F
#define DIN117                   0x579 //1F
#define DIN118                   0x57a //1F
#define DIN119                   0x57b //1F
#define DIN120                   0x57c //1F
#define DIN121                   0x57d //1F
#define DIN122                   0x57e //1F
#define DIN123                   0x57f //1F
#define DIN124                   0x580 //1F
#define DIN125                   0x581 //1F
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

#define DIN200                   0x5B0 //FF
#define DIN201                   0x5B1 //FF

#define DIN202                   0x5C0 //FF
#define DIN203                   0x5C1 //FF
#define DIN204                   0x5C2 //FF

#define DIN205                   0x5C4 //FF
#define DIN206                   0x5C5 //FF
#define DIN207                   0x5C6 //FF

#define DIN208                   0x5C8 //FF
#define DIN209                   0x5C9 //FF
#define DIN210                   0x5Ca //FF

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

#define FRC_CLNT_CTRL            0x00 //03
#define FRC_CTRL                 0x01 //FF
#define FRC_CTRL0                0x02 //FF
#define FRC_CTRL1                0x03 //FF
#define FRC_CTRL2                0x04 //FF
#define FRC_DLY_FRST_DE          0x05 //FF
#define FRC_DLY_DE_LRST          0x06 //FF
#define FRC_DLY_LRST_DE          0x07 //FF
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
#define FRC_BEHRES_L             0x24 //FF
#define FRC_BEHRES_H             0x25 //0F
#define FRC_BEVRES_L             0x26 //FF
#define FRC_BEVRES_H             0x27 //0F
#define FRC_FEVRES_L             0x28 //FF
#define FRC_FEVRES_H             0x29 //0F
#define FRC_SCLCLK_CTRL          0x2a //FF
#define FRC_SCLCLK_CTRL1         0x2b //FF

#define SCL_IVRES_0              0x60 //FF
#define SCL_IVRES_1              0x61 //0F
#define SCL_IHRES_0              0x62 //FF
#define SCL_IHRES_1              0x63 //0F
#define SCL_OVRES_0              0x64 //FF
#define SCL_OVRES_1              0x65 //0F
#define SCL_OHRES_0              0x66 //FF
#define SCL_OHRES_1              0x67 //0F
#define SCL_MEMSIZE_0            0x68 //FF
#define SCL_MEMSIZE_1            0x69 //07
#define SCL_VRATIO_0             0x6c //FF
#define SCL_VRATIO_1             0x6d //FF
#define SCL_VRATIO_2             0x6e //FF
#define SCL_HRATIO_0             0x70 //FF
#define SCL_HRATIO_1             0x71 //FF
#define SCL_HRATIO_2             0x72 //FF
#define SCL_VWRAP_0              0x74 //FF
#define SCL_VWRAP_1              0x75 //FF
#define SCL_VINITPH_EVEN         0x76 //FF
#define SCL_VINITPH_ODD          0x77 //FF
#define SCL_HINITPH              0x78 //FF
#define SCL_VCTRL_0              0x79 //1F
#define SCL_HCTRL_0              0x7a //1F
#define SCL_CTRL_0               0x7b //3F
#define SCL_NUMTAPS              0x7c //FF
#define SCL_HTAP_OFFSET          0x7d //07
#define SCL_VTAP_OFFSET          0x7e //FF
#define SCL_STALL                0x7f //FF
#define SCL_INIT_RATIO_0         0x80 //FF
#define SCL_INIT_RATIO_1         0x81 //FF
#define SCL_INIT_RATIO_2         0x82 //FF
#define SCL_INC_RATIO_0          0x84 //FF
#define SCL_INC_RATIO_1          0x85 //FF
#define SCL_INC_RATIO_2          0x86 //FF
#define SCL_NLCRES_0             0x88 //FF
#define SCL_NLCRES_1             0x89 //0F

#define SCL_COEFF0_L             0x8a //FF
#define SCL_COEFF0_H             0x8b //0F
#define SCL_COEFF1_L             0x8c //FF
#define SCL_COEFF1_H             0x8d //0F
#define SCL_COEFF2_L             0x8e //FF
#define SCL_COEFF2_H             0x8f //0F
#define SCL_COEFF3_L             0x90 //FF
#define SCL_COEFF3_H             0x91 //0F
#define SCL_COEFF4_L             0x92 //FF
#define SCL_COEFF4_H             0x93 //0F
#define SCL_COEFF5_L             0x94 //FF
#define SCL_COEFF5_H             0x95 //0F
#define SCL_COEFF6_L             0x96 //FF
#define SCL_COEFF6_H             0x97 //0F
#define SCL_COEFF7_L             0x98 //FF
#define SCL_COEFF7_H             0x99 //0F
#define SCL_COEFF8_L             0x9a //FF
#define SCL_COEFF8_H             0x9b //0F
#define SCL_COEFF9_L             0x9c //FF
#define SCL_COEFF9_H             0x9d //0F
#define SCL_COEFF10_L            0x9e //FF
#define SCL_COEFF10_H            0x9f //0F
#define SCL_COEFF11_L            0xA0 //FF
#define SCL_COEFF11_H            0xA1 //0F
#define SCL_COEFF_CTRL           0xA2 //7F
#define SCL_COEFF_LOAD           0xA3 //FF
#define SCL_MEMCTRL              0xA5 //03

#define SCL_C0_L                 0xA6 //FF
#define SCL_C0_H                 0xA7 //3F
#define SCL_C1_L                 0xA8 //FF
#define SCL_C1_H                 0xA9 //3F
#define SCL_C2_L                 0xAA //FF
#define SCL_C2_H                 0xAB //3F
#define SCL_C3_L                 0xAC //FF
#define SCL_C3_H                 0xAD //3F
#define SCL_C4_L                 0xAE //FF
#define SCL_C4_H                 0xAF //3F
#define SCL_C5_L                 0xB0 //FF
#define SCL_C5_H                 0xB1 //3F
#define SCL_C6_L                 0xB2 //FF
#define SCL_C6_H                 0xB3 //3F
#define SCL_C7_L                 0xB4 //FF
#define SCL_C7_H                 0xB5 //3F
#define SCL_C8_L                 0xB6 //FF
#define SCL_C8_H                 0xB7 //3F
#define SCL_A0_L                 0xB8 //FF
#define SCL_A0_H                 0xB9 //FF
#define SCL_A1_L                 0xBA //FF
#define SCL_A1_H                 0xBB //FF
#define SCL_A2_L                 0xBC //FF
#define SCL_A2_H                 0xBD //FF

#define SCL_ALPH_CTRL            0xBE //FF
#define SCL_ALPH0                0xBF //FF
#define SCL_ALPH1                0xC0 //FF
#define SCL_ALPH2                0xC1 //FF
#define SCL_ALPH3                0xC2 //FF
#define SCL_ALPH4                0xC3 //FF

// Video Encoder maddr.def
#define E_CTRL_0                 0x1900 //0F
#define E_CTRL_1                 0x1901 //0F
#define E_CTRL_2                 0x1902 //0F
#define E_CTRL_3                 0x1903 //0F
#define E_CTRL_4                 0x1904 //FF
#define E_CTRL_5                 0x1905 //FF
#define E_CTRL_6                 0x1906 //FF
#define E_CTRL_7                 0x1907 //FF
#define E_CTRL_8                 0x1908 //FF
#define E_CTRL_9                 0x1909 //FF
#define E_CTRL_10                0x190a //FF
#define E_CTRL_11                0x190b //03
#define E_CTRL_12                0x190c //FF
#define E_CTRL_13                0x190d //03
#define E_CTRL_14                0x190e //FF
#define E_CTRL_15                0x190f //FF
#define E_CTRL_16                0x1910 //FF
#define E_CTRL_17                0x1911 //FF
#define E_CTRL_18                0x1912 //FF
#define E_CTRL_19                0x1913 //03
#define E_CTRL_20                0x1914 //FF
#define E_CTRL_21                0x1915 //03
#define E_CTRL_22                0x1916 //FF
#define E_CTRL_23                0x1917 //FF
#define E_CTRL_24                0x1918 //FF
#define E_CTRL_25                0x1919 //03
#define E_CTRL_26                0x191a //01
#define E_CTRL_27                0x191b //FF
#define E_CTRL_28                0x191c //FF
#define E_CTRL_29                0x191d //03
#define E_CTRL_30                0x191e //FF
#define E_CTRL_31                0x191f //07
#define E_CTRL_32                0x1920 //01
#define E_CTRL_33                0x1921 //FF
#define E_CTRL_34                0x1922 //FF
#define E_CTRL_35                0x1923 //03
#define E_CTRL_36                0x1924 //FF
#define E_CTRL_37                0x1925 //03
#define E_CTRL_38                0x1926 //FF
#define E_CTRL_39                0x1927 //03
#define E_CTRL_40                0x1928 //FF
#define E_CTRL_41                0x1929 //01
#define E_CTRL_42                0x192a //FF
#define E_CTRL_43                0x192b //0F
#define E_CTRL_44                0x192c //FF
#define E_CTRL_45                0x192d //0F
#define E_CTRL_46                0x192e //NOT NEEDED
#define E_CTRL_47                0x192f //NOT NEEDED
#define E_CTRL_48                0x1930 //0F
#define E_CTRL_49                0x1931 //NOT NEEDED
#define E_CTRL_50                0x1932 //NOT NEEDED
#define E_CTRL_51                0x1933 //NOT NEEDED
#define E_CTRL_52                0x1934 //FF
#define E_CTRL_53                0x1935 //FF
#define E_CTRL_54                0x1936 //FF
#define E_CTRL_55                0x1937 //FF
#define E_CTRL_56                0x1938 //FF
#define E_CTRL_57                0x1939 //FF
#define E_CTRL_58                0x193a //03
#define E_CTRL_59                0x193b //NOT NEEDED
#define E_CTRL_60                0x193c //FF
#define E_CTRL_61                0x193d //FF
#define E_CTRL_62                0x193e //FF
#define E_CTRL_63                0x193f //03
#define E_CTRL_64                0x1940 //FF
#define E_CTRL_65                0x1941 //07
#define E_CTRL_66                0x1942 //FF
#define E_CTRL_67                0x1943 //07
#define E_CTRL_68                0x1944 //FF
#define E_CTRL_69                0x1945 //07
#define E_CTRL_70                0x1946 //FF
#define E_CTRL_71                0x1947 //07
#define E_CTRL_72                0x1948 //FF
#define E_CTRL_73                0x1949 //01
#define E_CTRL_74                0x194a //FF
#define E_CTRL_75                0x194b //01
#define E_CTRL_76                0x194c //FF
#define E_CTRL_77                0x194d //01
#define E_CTRL_78                0x194e //07
#define E_CTRL_79                0x194f //FF
#define E_CTRL_80                0x1950 //07
#define E_CTRL_81                0x1951 //07
#define E_CTRL_82                0x1952 //07
#define E_CTRL_83                0x1953 //07
#define E_CTRL_84                0x1954 //FF
#define E_CTRL_85                0x1955 //0F
#define E_CTRL_86                0x1956 //FF
#define E_CTRL_87                0x1957 //FF
#define E_CTRL_88                0x1958 //FF
#define E_CTRL_89                0x1959 //FF
#define E_CTRL_90                0x195a //01
#define E_CTRL_91                0x195b //1F
#define E_CTRL_92                0x195c //FF
#define E_CTRL_93                0x195d //NOT NEEDED
#define E_CTRL_94                0x195e //FF
#define E_CTRL_95                0x195f //FF
#define E_CTRL_96                0x1960 //0F
#define E_CTRL_97                0x1961 //NOT NEEDED
#define E_CTRL_98                0x1962 //FF
#define E_CTRL_99                0x1963 //0F
#define E_CTRL_100               0x1964 //FF
#define E_CTRL_101               0x1965 //FF
#define E_CTRL_102               0x1966 //FF
#define E_CTRL_103               0x1967 //FF
#define E_CTRL_104               0x1968 //01
#define E_CTRL_105               0x1969 //NOT NEEDED
#define E_CTRL_106               0x196a //NOT NEEDED
#define E_CTRL_107               0x196b //FF
#define E_CTRL_108               0x196c //FF
#define E_CTRL_109               0x196d //FF
#define E_CTRL_110               0x196e //FF
#define E_CTRL_111               0x196f //NOT NEEDED
#define E_CTRL_112               0x1970 //FF
#define E_CTRL_113               0x1971 //FF
#define E_CTRL_114               0x1972 //FF
#define E_CTRL_115               0x1973 //NOT NEEDED
#define E_CTRL_116               0x1974 //FF
#define E_CTRL_117               0x1975 //0F
#define E_CTRL_118               0x1976 //FF
#define E_CTRL_119               0x1977 //0F
#define E_CTRL_120               0x1978 //NOT NEEDED
#define E_CTRL_121               0x1979 //NOT NEEDED
#define E_CTRL_122               0x197a //NOT NEEDED
#define E_CTRL_123               0x197b //NOT NEEDED
#define E_CTRL_124               0x197c //NOT NEEDED
#define E_CTRL_125               0x197d //NOT NEEDED
#define E_CTRL_126               0x197e //NOT NEEDED
#define E_CTRL_127               0x197f //NOT NEEDED
#define E_CTRL_128               0x1980 //FF
#define E_CTRL_129               0x1981 //1F
#define E_CTRL_130               0x1982 //FF
#define E_CTRL_131               0x1983 //07
#define E_CTRL_132               0x1984 //FF
#define E_CTRL_133               0x1985 //FF
#define E_CTRL_134               0x1986 //FF
#define E_CTRL_135               0x1987 //1F
#define E_CTRL_136               0x1988 //FF
#define E_CTRL_137               0x1989 //07
#define E_CTRL_138               0x198a //FF
#define E_CTRL_139               0x198b //1F
#define E_CTRL_140               0x198c //FF
#define E_CTRL_141               0x198d //07
#define E_CTRL_142               0x198e //7F
#define E_CTRL_143               0x198f //0F
#define E_CTRL_144               0x1990 //FF
#define E_CTRL_145               0x1991 //FF
#define E_CTRL_146               0x1992 //FF
#define E_CTRL_147               0x1993 //03
#define E_CTRL_148               0x1994 //07
#define E_CTRL_149               0x1995 //NOT NEEDED
#define E_CTRL_150               0x1996 //FF
#define E_CTRL_151               0x1997 //07
#define E_CTRL_152               0x1998 //FF
#define E_CTRL_153               0x1999 //07
#define E_CTRL_154               0x199a //FF
#define E_CTRL_155               0x199b //07
#define E_CTRL_156               0x199c //FF
#define E_CTRL_157               0x199d //01
#define E_CTRL_158               0x199e //FF
#define E_CTRL_159               0x199f //01
#define E_CTRL_160               0x19a0 //FF
#define E_CTRL_161               0x19a1 //01
#define E_CTRL_162               0x19a2 //0F
#define E_CTRL_163               0x19a3 //1F
#define E_CTRL_164               0x19a4 //FF
#define E_CTRL_165               0x19a5 //NOT NEEDED
#define E_CTRL_166               0x19a6 //FF
#define E_CTRL_167               0x19a7 //FF
#define E_CTRL_168               0x19a8 //0F
#define E_CTRL_169               0x19a9 //NOT NEEDED
#define E_CTRL_170               0x19aa //FF
#define E_CTRL_171               0x19ab //0F
#define E_CTRL_172               0x19ac //FF
#define E_CTRL_173               0x19ad //FF
#define E_CTRL_174               0x19ae //FF
#define E_CTRL_175               0x19af //FF
#define E_CTRL_176               0x19b0 //FF
#define E_CTRL_177               0x19b1 //0F
#define E_CTRL_178               0x19b2 //01
#define E_CTRL_179               0x19b3 //NOT NEEDED
#define E_CTRL_180               0x19b4 //NOT NEEDED
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

// HDMI-TX registers
//-----------AUDIO---------------
#define HDTX_ACR_N0            0x1000 //FF
#define HDTX_ACR_N1            0x1001 //FF
#define HDTX_ACR_N2            0x1002 //0F
#define HDTX_ACR_CTS0          0x1004 //FF
#define HDTX_ACR_CTS1          0x1005 //FF
#define HDTX_ACR_CTS2          0x1006 //0F
#define HDTX_ACR_CTRL          0x1007 //0F
#define HDTX_ACR_STS0          0x1008 //FF
#define HDTX_ACR_STS1          0x1009 //FF
#define HDTX_ACR_STS2          0x100A //0F
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
#define HDTX_FIFO_CTRL         0x101D //FF
#define HDTX_MEMSIZE_L         0x101E //FF
#define HDTX_MEMSIZE_H         0x101F //01
#define HDTX_GCP_CFG0          0x1020 //0F
#define HDTX_GCP_CFG1          0x1021 //FF
#define HDTX_AUD_STS           0x1022 //FF
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
#define HDTX_HTOT_STS_L        0x102E //FF
#define HDTX_HTOT_STS_H        0x102F //FF
#define HDTX_HBLANK_STS_L      0x1030 //FF
#define HDTX_HBLANK_STS_H      0x1031 //FF
#define HDTX_VTOT_STS_L        0x1032 //FF
#define HDTX_VTOT_STS_H        0x1033 //FF
#define HDTX_VRES_STS_L        0x1034 //FF
#define HDTX_VRES_STS_H        0x1035 //FF
#define HDTX_VSTART_STS_L      0x1036 //FF
#define HDTX_VSTART_STS_H      0x1037 //FF
#define HDTX_VIDEO_STS         0x1038 //01
#define HDTX_VIDEO_CTRL        0x1039 //03
#define HDTX_HDMI_CTRL         0x103A //3F
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

#define HDTX_PP_HW             0x1046 //0F

//-----------------------------
// DEEP COLOR
//------_-----------------------
#define HDTX_DC_FIFO_SFT_RST   0x1047 //01
#define HDTX_DC_FIFO_WR_PTR    0x1048 //3F
#define HDTX_DC_FIFO_RD_PTR    0x1049 //3F

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
#define HDTX_TDATA_SEL         0x105B //07
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
#define HDTX_HBR_PKT           0x112E //03

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
#define TX_HDCP_HPD_SEL        0x1140
#define TX_HDCP_SW_HPD         0x1141

// --  CEC
#define CEC_NOACK_REG_ADDR               0x1190
#define CEC_TX_PRESENT_STATE_REG_ADDR    0x1191
#define CEC_RX_PRESENT_STATE_REG_ADDR    0x1192
#define CEC_COLL_WINDOW_TIME_REG_0_ADDR  0x1193
#define CEC_COLL_WINDOW_TIME_REG_1_ADDR  0x1194
#define CEC_COLL_WINDOW_TIME_REG_2_ADDR  0x1195
#define CEC_COLL_WINDOW_TIME_REG_3_ADDR  0x1196
#define CEC_TR_TIMER_EN_REG_ADDR         0x1197
#define CEC_TOGGLE_FOR_WRITE_REG_ADDR    0x1198
#define CEC_TOGGLE_FOR_READ_REG_ADDR     0x119c
#define CEC_RDY_ADDR                     0x11a0
#define CEC_RX_RDY_ADDR                  0x11a4
#define CEC_TX_FIFO_RESET_ADDR           0x11a5
#define CEC_RX_FIFO_RESET_ADDR           0x11a6
#define CEC_PMODE_ADDR                   0x11a7
#define CEC_TX_RDY_ADDR                  0x11a8
#define CEC_TX_TYPE_ADDR                 0x11ac
#define CEC_TX_RESP_TIME_0_ADDR          0x11b0
#define CEC_TX_RESP_TIME_1_ADDR          0x11b1
#define CEC_TX_RESP_TIME_2_ADDR          0x11b2
#define CEC_TX_RESP_TIME_3_ADDR          0x11b3
#define CEC_SIGNAL_FREE_TIME_0_ADDR      0x11b4
#define CEC_SIGNAL_FREE_TIME_1_ADDR      0x11b5
#define CEC_SIGNAL_FREE_TIME_2_ADDR      0x11b6
#define CEC_SIGNAL_FREE_TIME_3_ADDR      0x11b7
#define CEC_START_BIT_LO_THRESH_0_ADDR   0x11b8
#define CEC_START_BIT_LO_THRESH_1_ADDR   0x11b9
#define CEC_START_BIT_LO_THRESH_2_ADDR   0x11ba
#define CEC_START_BIT_LO_THRESH_3_ADDR   0x11bb
#define CEC_START_BIT_HI_THRESH_0_ADDR   0x11bc
#define CEC_START_BIT_HI_THRESH_1_ADDR   0x11bd
#define CEC_START_BIT_HI_THRESH_2_ADDR   0x11be
#define CEC_START_BIT_HI_THRESH_3_ADDR   0x11bf
#define CEC_DATA_BIT_0_LO_THRESH_0_ADDR  0x11c0
#define CEC_DATA_BIT_0_LO_THRESH_1_ADDR  0x11c1
#define CEC_DATA_BIT_0_LO_THRESH_2_ADDR  0x11c2
#define CEC_DATA_BIT_0_LO_THRESH_3_ADDR  0x11c3
#define CEC_DATA_BIT_1_LO_THRESH_0_ADDR  0x11c4
#define CEC_DATA_BIT_1_LO_THRESH_1_ADDR  0x11c5
#define CEC_DATA_BIT_1_LO_THRESH_2_ADDR  0x11c6
#define CEC_DATA_BIT_1_LO_THRESH_3_ADDR  0x11c7
#define CEC_DATA_BIT_0_HI_THRESH_0_ADDR  0x11c8
#define CEC_DATA_BIT_0_HI_THRESH_1_ADDR  0x11c9
#define CEC_DATA_BIT_0_HI_THRESH_2_ADDR  0x11ca
#define CEC_DATA_BIT_0_HI_THRESH_3_ADDR  0x11cb
#define CEC_DATA_BIT_1_HI_THRESH_0_ADDR  0x11cc
#define CEC_DATA_BIT_1_HI_THRESH_1_ADDR  0x11cd
#define CEC_DATA_BIT_1_HI_THRESH_2_ADDR  0x11ce
#define CEC_DATA_BIT_1_HI_THRESH_3_ADDR  0x11cf
#define CEC_SSP_ACK_TIME_0_ADDR          0x11d0
#define CEC_SSP_ACK_TIME_1_ADDR          0x11d1
#define CEC_SSP_ACK_TIME_2_ADDR          0x11d2
#define CEC_SSP_ACK_TIME_3_ADDR          0x11d3
#define CEC_INTR_ENABLE_REG_ADDR         0x11d4
#define CEC_DATA_REG_ADDR                0x11d8
#define CEC_EOM_REG_ADDR                 0x11dc
#define CEC_FIFO_STATUS_REG_ADDR         0x11e0
#define CEC_INTR_STATUS_REG_ADDR         0x11e4
#define CEC_NOMINAL_SAMPLE_TIME_0_ADDR   0x11e8
#define CEC_NOMINAL_SAMPLE_TIME_1_ADDR   0x11e9
#define CEC_NOMINAL_SAMPLE_TIME_2_ADDR   0x11ea
#define CEC_NOMINAL_SAMPLE_TIME_3_ADDR   0x11eb
#define CEC_HYST_TIME_0_ADDR             0x11ec
#define CEC_HYST_TIME_1_ADDR             0x11ed
#define CEC_HYST_TIME_2_ADDR             0x11ee
#define CEC_HYST_TIME_3_ADDR             0x11ef
#define CEC_FOLLOWER_ACK_TIME_0_ADDR     0x11f0
#define CEC_FOLLOWER_ACK_TIME_1_ADDR     0x11f1
#define CEC_FOLLOWER_ACK_TIME_2_ADDR     0x11f2
#define CEC_FOLLOWER_ACK_TIME_3_ADDR     0x11f3
#define CEC_RX_BUF_READ_REG_ADDR         0x11f4
#define CEC_LOGICAL_ADDR_REG_ADDR        0x11f8
#define CEC_JITTER_CNT_0_ADDR            0x11fc
#define CEC_JITTER_CNT_1_ADDR            0x11fd
#define CEC_JITTER_CNT_2_ADDR            0x11fe
#define CEC_JITTER_CNT_3_ADDR            0x11ff

//--------------------------------
//HDCP
//--------------------------------

//ADDRESS OF AKEYS
#define TX_HDCP_AKEY0_BYTE_0     0x1200 //FF
#define TX_HDCP_AKEY0_BYTE_1     0x1201 //FF
#define TX_HDCP_AKEY0_BYTE_2     0x1202 //FF
#define TX_HDCP_AKEY0_BYTE_3     0x1203 //FF
#define TX_HDCP_AKEY0_BYTE_4     0x1204 //FF
#define TX_HDCP_AKEY0_BYTE_5     0x1205 //FF
#define TX_HDCP_AKEY0_BYTE_6     0x1206 //FF

#define TX_HDCP_AKEY1_BYTE_0     0x1208 //FF
#define TX_HDCP_AKEY1_BYTE_1     0x1209 //FF
#define TX_HDCP_AKEY1_BYTE_2     0x120A //FF
#define TX_HDCP_AKEY1_BYTE_3     0x120B //FF
#define TX_HDCP_AKEY1_BYTE_4     0x120C //FF
#define TX_HDCP_AKEY1_BYTE_5     0x120D //FF
#define TX_HDCP_AKEY1_BYTE_6     0x120E //FF

#define TX_HDCP_AKEY2_BYTE_0     0x1210 //FF
#define TX_HDCP_AKEY2_BYTE_1     0x1211 //FF
#define TX_HDCP_AKEY2_BYTE_2     0x1212 //FF
#define TX_HDCP_AKEY2_BYTE_3     0x1213 //FF
#define TX_HDCP_AKEY2_BYTE_4     0x1214 //FF
#define TX_HDCP_AKEY2_BYTE_5     0x1215 //FF
#define TX_HDCP_AKEY2_BYTE_6     0x1216 //FF

#define TX_HDCP_AKEY3_BYTE_0     0x1218 //FF
#define TX_HDCP_AKEY3_BYTE_1     0x1219 //FF
#define TX_HDCP_AKEY3_BYTE_2     0x121A //FF
#define TX_HDCP_AKEY3_BYTE_3     0x121B //FF
#define TX_HDCP_AKEY3_BYTE_4     0x121C //FF
#define TX_HDCP_AKEY3_BYTE_5     0x121D //FF
#define TX_HDCP_AKEY3_BYTE_6     0x121E //FF

#define TX_HDCP_AKEY4_BYTE_0     0x1220 //FF
#define TX_HDCP_AKEY4_BYTE_1     0x1221 //FF
#define TX_HDCP_AKEY4_BYTE_2     0x1222 //FF
#define TX_HDCP_AKEY4_BYTE_3     0x1223 //FF
#define TX_HDCP_AKEY4_BYTE_4     0x1224 //FF
#define TX_HDCP_AKEY4_BYTE_5     0x1225 //FF
#define TX_HDCP_AKEY4_BYTE_6     0x1226 //FF

#define TX_HDCP_AKEY5_BYTE_0     0x1228 //FF
#define TX_HDCP_AKEY5_BYTE_1     0x1229 //FF
#define TX_HDCP_AKEY5_BYTE_2     0x122A //FF
#define TX_HDCP_AKEY5_BYTE_3     0x122B //FF
#define TX_HDCP_AKEY5_BYTE_4     0x122C //FF
#define TX_HDCP_AKEY5_BYTE_5     0x122D //FF
#define TX_HDCP_AKEY5_BYTE_6     0x122E //FF

#define TX_HDCP_AKEY6_BYTE_0     0x1230 //FF
#define TX_HDCP_AKEY6_BYTE_1     0x1231 //FF
#define TX_HDCP_AKEY6_BYTE_2     0x1232 //FF
#define TX_HDCP_AKEY6_BYTE_3     0x1233 //FF
#define TX_HDCP_AKEY6_BYTE_4     0x1234 //FF
#define TX_HDCP_AKEY6_BYTE_5     0x1235 //FF
#define TX_HDCP_AKEY6_BYTE_6     0x1236 //FF

#define TX_HDCP_AKEY7_BYTE_0     0x1238 //FF
#define TX_HDCP_AKEY7_BYTE_1     0x1239 //FF
#define TX_HDCP_AKEY7_BYTE_2     0x123A //FF
#define TX_HDCP_AKEY7_BYTE_3     0x123B //FF
#define TX_HDCP_AKEY7_BYTE_4     0x123C //FF
#define TX_HDCP_AKEY7_BYTE_5     0x123D //FF
#define TX_HDCP_AKEY7_BYTE_6     0x123E //FF

#define TX_HDCP_AKEY8_BYTE_0     0x1240 //FF
#define TX_HDCP_AKEY8_BYTE_1     0x1241 //FF
#define TX_HDCP_AKEY8_BYTE_2     0x1242 //FF
#define TX_HDCP_AKEY8_BYTE_3     0x1243 //FF
#define TX_HDCP_AKEY8_BYTE_4     0x1244 //FF
#define TX_HDCP_AKEY8_BYTE_5     0x1245 //FF
#define TX_HDCP_AKEY8_BYTE_6     0x1246 //FF

#define TX_HDCP_AKEY9_BYTE_0     0x1248 //FF
#define TX_HDCP_AKEY9_BYTE_1     0x1249 //FF
#define TX_HDCP_AKEY9_BYTE_2     0x124A //FF
#define TX_HDCP_AKEY9_BYTE_3     0x124B //FF
#define TX_HDCP_AKEY9_BYTE_4     0x124C //FF
#define TX_HDCP_AKEY9_BYTE_5     0x124D //FF
#define TX_HDCP_AKEY9_BYTE_6     0x124E //FF

#define TX_HDCP_AKEY10_BYTE_0    0x1250 //FF
#define TX_HDCP_AKEY10_BYTE_1    0x1251 //FF
#define TX_HDCP_AKEY10_BYTE_2    0x1252 //FF
#define TX_HDCP_AKEY10_BYTE_3    0x1253 //FF
#define TX_HDCP_AKEY10_BYTE_4    0x1254 //FF
#define TX_HDCP_AKEY10_BYTE_5    0x1255 //FF
#define TX_HDCP_AKEY10_BYTE_6    0x1256 //FF

#define TX_HDCP_AKEY11_BYTE_0    0x1258 //FF
#define TX_HDCP_AKEY11_BYTE_1    0x1259 //FF
#define TX_HDCP_AKEY11_BYTE_2    0x125A //FF
#define TX_HDCP_AKEY11_BYTE_3    0x125B //FF
#define TX_HDCP_AKEY11_BYTE_4    0x125C //FF
#define TX_HDCP_AKEY11_BYTE_5    0x125D //FF
#define TX_HDCP_AKEY11_BYTE_6    0x125E //FF

#define TX_HDCP_AKEY12_BYTE_0    0x1260 //FF
#define TX_HDCP_AKEY12_BYTE_1    0x1261 //FF
#define TX_HDCP_AKEY12_BYTE_2    0x1262 //FF
#define TX_HDCP_AKEY12_BYTE_3    0x1263 //FF
#define TX_HDCP_AKEY12_BYTE_4    0x1264 //FF
#define TX_HDCP_AKEY12_BYTE_5    0x1265 //FF
#define TX_HDCP_AKEY12_BYTE_6    0x1266 //FF

#define TX_HDCP_AKEY13_BYTE_0    0x1268 //FF
#define TX_HDCP_AKEY13_BYTE_1    0x1269 //FF
#define TX_HDCP_AKEY13_BYTE_2    0x126A //FF
#define TX_HDCP_AKEY13_BYTE_3    0x126B //FF
#define TX_HDCP_AKEY13_BYTE_4    0x126C //FF
#define TX_HDCP_AKEY13_BYTE_5    0x126D //FF
#define TX_HDCP_AKEY13_BYTE_6    0x126E //FF

#define TX_HDCP_AKEY14_BYTE_0    0x1270 //FF
#define TX_HDCP_AKEY14_BYTE_1    0x1271 //FF
#define TX_HDCP_AKEY14_BYTE_2    0x1272 //FF
#define TX_HDCP_AKEY14_BYTE_3    0x1273 //FF
#define TX_HDCP_AKEY14_BYTE_4    0x1274 //FF
#define TX_HDCP_AKEY14_BYTE_5    0x1275 //FF
#define TX_HDCP_AKEY14_BYTE_6    0x1276 //FF

#define TX_HDCP_AKEY15_BYTE_0    0x1278 //FF
#define TX_HDCP_AKEY15_BYTE_1    0x1279 //FF
#define TX_HDCP_AKEY15_BYTE_2    0x127A //FF
#define TX_HDCP_AKEY15_BYTE_3    0x127B //FF
#define TX_HDCP_AKEY15_BYTE_4    0x127C //FF
#define TX_HDCP_AKEY15_BYTE_5    0x127D //FF
#define TX_HDCP_AKEY15_BYTE_6    0x127E //FF

#define TX_HDCP_AKEY16_BYTE_0    0x1280 //FF
#define TX_HDCP_AKEY16_BYTE_1    0x1281 //FF
#define TX_HDCP_AKEY16_BYTE_2    0x1282 //FF
#define TX_HDCP_AKEY16_BYTE_3    0x1283 //FF
#define TX_HDCP_AKEY16_BYTE_4    0x1284 //FF
#define TX_HDCP_AKEY16_BYTE_5    0x1285 //FF
#define TX_HDCP_AKEY16_BYTE_6    0x1286 //FF

#define TX_HDCP_AKEY17_BYTE_0    0x1288 //FF
#define TX_HDCP_AKEY17_BYTE_1    0x1289 //FF
#define TX_HDCP_AKEY17_BYTE_2    0x128A //FF
#define TX_HDCP_AKEY17_BYTE_3    0x128B //FF
#define TX_HDCP_AKEY17_BYTE_4    0x128C //FF
#define TX_HDCP_AKEY17_BYTE_5    0x128D //FF
#define TX_HDCP_AKEY17_BYTE_6    0x128E //FF

#define TX_HDCP_AKEY18_BYTE_0    0x1290 //FF
#define TX_HDCP_AKEY18_BYTE_1    0x1291 //FF
#define TX_HDCP_AKEY18_BYTE_2    0x1292 //FF
#define TX_HDCP_AKEY18_BYTE_3    0x1293 //FF
#define TX_HDCP_AKEY18_BYTE_4    0x1294 //FF
#define TX_HDCP_AKEY18_BYTE_5    0x1295 //FF
#define TX_HDCP_AKEY18_BYTE_6    0x1296 //FF

#define TX_HDCP_AKEY19_BYTE_0    0x1298 //FF
#define TX_HDCP_AKEY19_BYTE_1    0x1299 //FF
#define TX_HDCP_AKEY19_BYTE_2    0x129A //FF
#define TX_HDCP_AKEY19_BYTE_3    0x129B //FF
#define TX_HDCP_AKEY19_BYTE_4    0x129C //FF
#define TX_HDCP_AKEY19_BYTE_5    0x129D //FF
#define TX_HDCP_AKEY19_BYTE_6    0x129E //FF

#define TX_HDCP_AKEY20_BYTE_0    0x12A0 //FF
#define TX_HDCP_AKEY20_BYTE_1    0x12A1 //FF
#define TX_HDCP_AKEY20_BYTE_2    0x12A2 //FF
#define TX_HDCP_AKEY20_BYTE_3    0x12A3 //FF
#define TX_HDCP_AKEY20_BYTE_4    0x12A4 //FF
#define TX_HDCP_AKEY20_BYTE_5    0x12A5 //FF
#define TX_HDCP_AKEY20_BYTE_6    0x12A6 //FF

#define TX_HDCP_AKEY21_BYTE_0    0x12A8 //FF
#define TX_HDCP_AKEY21_BYTE_1    0x12A9 //FF
#define TX_HDCP_AKEY21_BYTE_2    0x12AA //FF
#define TX_HDCP_AKEY21_BYTE_3    0x12AB //FF
#define TX_HDCP_AKEY21_BYTE_4    0x12AC //FF
#define TX_HDCP_AKEY21_BYTE_5    0x12AD //FF
#define TX_HDCP_AKEY21_BYTE_6    0x12AE //FF

#define TX_HDCP_AKEY22_BYTE_0    0x12B0 //FF
#define TX_HDCP_AKEY22_BYTE_1    0x12B1 //FF
#define TX_HDCP_AKEY22_BYTE_2    0x12B2 //FF
#define TX_HDCP_AKEY22_BYTE_3    0x12B3 //FF
#define TX_HDCP_AKEY22_BYTE_4    0x12B4 //FF
#define TX_HDCP_AKEY22_BYTE_5    0x12B5 //FF
#define TX_HDCP_AKEY22_BYTE_6    0x12B6 //FF

#define TX_HDCP_AKEY23_BYTE_0    0x12B8 //FF
#define TX_HDCP_AKEY23_BYTE_1    0x12B9 //FF
#define TX_HDCP_AKEY23_BYTE_2    0x12BA //FF
#define TX_HDCP_AKEY23_BYTE_3    0x12BB //FF
#define TX_HDCP_AKEY23_BYTE_4    0x12BC //FF
#define TX_HDCP_AKEY23_BYTE_5    0x12BD //FF
#define TX_HDCP_AKEY23_BYTE_6    0x12BE //FF

#define TX_HDCP_AKEY24_BYTE_0    0x12C0 //FF
#define TX_HDCP_AKEY24_BYTE_1    0x12C1 //FF
#define TX_HDCP_AKEY24_BYTE_2    0x12C2 //FF
#define TX_HDCP_AKEY24_BYTE_3    0x12C3 //FF
#define TX_HDCP_AKEY24_BYTE_4    0x12C4 //FF
#define TX_HDCP_AKEY24_BYTE_5    0x12C5 //FF
#define TX_HDCP_AKEY24_BYTE_6    0x12C6 //FF

#define TX_HDCP_AKEY25_BYTE_0    0x12C8 //FF
#define TX_HDCP_AKEY25_BYTE_1    0x12C9 //FF
#define TX_HDCP_AKEY25_BYTE_2    0x12CA //FF
#define TX_HDCP_AKEY25_BYTE_3    0x12CB //FF
#define TX_HDCP_AKEY25_BYTE_4    0x12CC //FF
#define TX_HDCP_AKEY25_BYTE_5    0x12CD //FF
#define TX_HDCP_AKEY25_BYTE_6    0x12CE //FF

#define TX_HDCP_AKEY26_BYTE_0    0x12D0 //FF
#define TX_HDCP_AKEY26_BYTE_1    0x12D1 //FF
#define TX_HDCP_AKEY26_BYTE_2    0x12D2 //FF
#define TX_HDCP_AKEY26_BYTE_3    0x12D3 //FF
#define TX_HDCP_AKEY26_BYTE_4    0x12D4 //FF
#define TX_HDCP_AKEY26_BYTE_5    0x12D5 //FF
#define TX_HDCP_AKEY26_BYTE_6    0x12D6 //FF

#define TX_HDCP_AKEY27_BYTE_0    0x12D8 //FF
#define TX_HDCP_AKEY27_BYTE_1    0x12D9 //FF
#define TX_HDCP_AKEY27_BYTE_2    0x12DA //FF
#define TX_HDCP_AKEY27_BYTE_3    0x12DB //FF
#define TX_HDCP_AKEY27_BYTE_4    0x12DC //FF
#define TX_HDCP_AKEY27_BYTE_5    0x12DD //FF
#define TX_HDCP_AKEY27_BYTE_6    0x12DE //FF

#define TX_HDCP_AKEY28_BYTE_0    0x12E0 //FF
#define TX_HDCP_AKEY28_BYTE_1    0x12E1 //FF
#define TX_HDCP_AKEY28_BYTE_2    0x12E2 //FF
#define TX_HDCP_AKEY28_BYTE_3    0x12E3 //FF
#define TX_HDCP_AKEY28_BYTE_4    0x12E4 //FF
#define TX_HDCP_AKEY28_BYTE_5    0x12E5 //FF
#define TX_HDCP_AKEY28_BYTE_6    0x12E6 //FF

#define TX_HDCP_AKEY29_BYTE_0    0x12E8 //FF
#define TX_HDCP_AKEY29_BYTE_1    0x12E9 //FF
#define TX_HDCP_AKEY29_BYTE_2    0x12EA //FF
#define TX_HDCP_AKEY29_BYTE_3    0x12EB //FF
#define TX_HDCP_AKEY29_BYTE_4    0x12EC //FF
#define TX_HDCP_AKEY29_BYTE_5    0x12ED //FF
#define TX_HDCP_AKEY29_BYTE_6    0x12EE //FF

#define TX_HDCP_AKEY30_BYTE_0    0x12F0 //FF
#define TX_HDCP_AKEY30_BYTE_1    0x12F1 //FF
#define TX_HDCP_AKEY30_BYTE_2    0x12F2 //FF
#define TX_HDCP_AKEY30_BYTE_3    0x12F3 //FF
#define TX_HDCP_AKEY30_BYTE_4    0x12F4 //FF
#define TX_HDCP_AKEY30_BYTE_5    0x12F5 //FF
#define TX_HDCP_AKEY30_BYTE_6    0x12F6 //FF

#define TX_HDCP_AKEY31_BYTE_0    0x12F8 //FF
#define TX_HDCP_AKEY31_BYTE_1    0x12F9 //FF
#define TX_HDCP_AKEY31_BYTE_2    0x12FA //FF
#define TX_HDCP_AKEY31_BYTE_3    0x12FB //FF
#define TX_HDCP_AKEY31_BYTE_4    0x12FC //FF
#define TX_HDCP_AKEY31_BYTE_5    0x12FD //FF
#define TX_HDCP_AKEY31_BYTE_6    0x12FE //FF

#define TX_HDCP_AKEY32_BYTE_0    0x1300 //FF
#define TX_HDCP_AKEY32_BYTE_1    0x1301 //FF
#define TX_HDCP_AKEY32_BYTE_2    0x1302 //FF
#define TX_HDCP_AKEY32_BYTE_3    0x1303 //FF
#define TX_HDCP_AKEY32_BYTE_4    0x1304 //FF
#define TX_HDCP_AKEY32_BYTE_5    0x1305 //FF
#define TX_HDCP_AKEY32_BYTE_6    0x1306 //FF

#define TX_HDCP_AKEY33_BYTE_0    0x1308 //FF
#define TX_HDCP_AKEY33_BYTE_1    0x1309 //FF
#define TX_HDCP_AKEY33_BYTE_2    0x130A //FF
#define TX_HDCP_AKEY33_BYTE_3    0x130B //FF
#define TX_HDCP_AKEY33_BYTE_4    0x130C //FF
#define TX_HDCP_AKEY33_BYTE_5    0x130D //FF
#define TX_HDCP_AKEY33_BYTE_6    0x130E //FF

#define TX_HDCP_AKEY34_BYTE_0    0x1310 //FF
#define TX_HDCP_AKEY34_BYTE_1    0x1311 //FF
#define TX_HDCP_AKEY34_BYTE_2    0x1312 //FF
#define TX_HDCP_AKEY34_BYTE_3    0x1313 //FF
#define TX_HDCP_AKEY34_BYTE_4    0x1314 //FF
#define TX_HDCP_AKEY34_BYTE_5    0x1315 //FF
#define TX_HDCP_AKEY34_BYTE_6    0x1316 //FF

#define TX_HDCP_AKEY35_BYTE_0    0x1318 //FF
#define TX_HDCP_AKEY35_BYTE_1    0x1319 //FF
#define TX_HDCP_AKEY35_BYTE_2    0x131A //FF
#define TX_HDCP_AKEY35_BYTE_3    0x131B //FF
#define TX_HDCP_AKEY35_BYTE_4    0x131C //FF
#define TX_HDCP_AKEY35_BYTE_5    0x131D //FF
#define TX_HDCP_AKEY35_BYTE_6    0x131E //FF

#define TX_HDCP_AKEY36_BYTE_0    0x1320 //FF
#define TX_HDCP_AKEY36_BYTE_1    0x1321 //FF
#define TX_HDCP_AKEY36_BYTE_2    0x1322 //FF
#define TX_HDCP_AKEY36_BYTE_3    0x1323 //FF
#define TX_HDCP_AKEY36_BYTE_4    0x1324 //FF
#define TX_HDCP_AKEY36_BYTE_5    0x1325 //FF
#define TX_HDCP_AKEY36_BYTE_6    0x1326 //FF

#define TX_HDCP_AKEY37_BYTE_0    0x1328 //FF
#define TX_HDCP_AKEY37_BYTE_1    0x1329 //FF
#define TX_HDCP_AKEY37_BYTE_2    0x132A //FF
#define TX_HDCP_AKEY37_BYTE_3    0x132B //FF
#define TX_HDCP_AKEY37_BYTE_4    0x132C //FF
#define TX_HDCP_AKEY37_BYTE_5    0x132D //FF
#define TX_HDCP_AKEY37_BYTE_6    0x132E //FF

#define TX_HDCP_AKEY38_BYTE_0    0x1330 //FF
#define TX_HDCP_AKEY38_BYTE_1    0x1331 //FF
#define TX_HDCP_AKEY38_BYTE_2    0x1332 //FF
#define TX_HDCP_AKEY38_BYTE_3    0x1333 //FF
#define TX_HDCP_AKEY38_BYTE_4    0x1334 //FF
#define TX_HDCP_AKEY38_BYTE_5    0x1335 //FF
#define TX_HDCP_AKEY38_BYTE_6    0x1336 //FF

#define TX_HDCP_AKEY39_BYTE_0    0x1338 //FF
#define TX_HDCP_AKEY39_BYTE_1    0x1339 //FF
#define TX_HDCP_AKEY39_BYTE_2    0x133A //FF
#define TX_HDCP_AKEY39_BYTE_3    0x133B //FF
#define TX_HDCP_AKEY39_BYTE_4    0x133C //FF
#define TX_HDCP_AKEY39_BYTE_5    0x133D //FF
#define TX_HDCP_AKEY39_BYTE_6    0x133E //FF


#define TX_HDCP_AKSV_BYTE_0      0x1340 //FF
#define TX_HDCP_AKSV_BYTE_1      0x1341 //FF
#define TX_HDCP_AKSV_BYTE_2      0x1342 //FF
#define TX_HDCP_AKSV_BYTE_3      0x1343 //FF
#define TX_HDCP_AKSV_BYTE_4      0x1344 //FF

// HDCP REGS
#define TX_HDCP_CTRL             0x1350 //7F
#define TX_HDCP_STATUS_1         0x1351 //7F
#define TX_HDCP_STATUS_2         0x1352 //03
#define TX_HDCP_INTR_0           0x1353
#define TX_HDCP_TX_AKSV_0        0x1354 //FF
#define TX_HDCP_TX_AKSV_1        0x1355 //FF
#define TX_HDCP_TX_AKSV_2        0x1356 //FF
#define TX_HDCP_TX_AKSV_3        0x1357 //FF
#define TX_HDCP_TX_AKSV_4        0x1358 //FF
#define TX_HDCP_RX_BKSV_0        0x135C //FF
#define TX_HDCP_RX_BKSV_1        0x135D //FF
#define TX_HDCP_RX_BKSV_2        0x135E //FF
#define TX_HDCP_RX_BKSV_3        0x135F //FF
#define TX_HDCP_RX_BKSV_4        0x1360 //FF
#define TX_HDCP_TX_AINFO         0x1361 //FF
#define TX_HDCP_RX_BCAPS         0x1362 //FF
#define TX_HDCP_RX_BSTATUS_0     0x1364 //FF
#define TX_HDCP_RX_BSTATUS_1     0x1365 //FF
#define TX_HDCP_TX_AN_0          0x1368 //FF
#define TX_HDCP_TX_AN_1          0x1369 //FF
#define TX_HDCP_TX_AN_2          0x136A //FF
#define TX_HDCP_TX_AN_3          0x136B //FF
#define TX_HDCP_TX_AN_4          0x136C //FF
#define TX_HDCP_TX_AN_5          0x136D //FF
#define TX_HDCP_TX_AN_6          0x136E //FF
#define TX_HDCP_TX_AN_7          0x136F //FF
#define TX_HDCP_TX_M0_0          0x1370 //M0 shouldn't be accessable by FW
#define TX_HDCP_TX_M0_1          0x1371
#define TX_HDCP_TX_M0_2          0x1372
#define TX_HDCP_TX_M0_3          0x1373
#define TX_HDCP_TX_M0_4          0x1374
#define TX_HDCP_TX_M0_5          0x1375
#define TX_HDCP_TX_M0_6          0x1376
#define TX_HDCP_TX_M0_7          0x1377
#define TX_HDCP_TX_R0_0          0x1378 //FF
#define TX_HDCP_TX_R0_1          0x1379 //FF
#define TX_HDCP_RX_R0_0          0x137A //FF
#define TX_HDCP_RX_R0_1          0x137B //FF
#define TX_HDCP_TX_RI_0          0x137C //FF
#define TX_HDCP_TX_RI_1          0x137D //FF
#define TX_HDCP_RX_RI_0          0x137E //FF
#define TX_HDCP_RX_RI_1          0x137F //FF
#define TX_HDCP_TX_PJ            0x1380 //FF
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
#define TX_HDCP_INTR_CLR         0x1398 //1F
#define TX_HDCP_INTR_MASK        0x1399 //1F

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

#define VOP_AHD_C0_L             0x1818 //FF
#define VOP_AHD_C0_H             0x1819 //3F
#define VOP_AHD_C1_L             0x181a //FF
#define VOP_AHD_C1_H             0x181b //3F
#define VOP_AHD_C2_L             0x181c //FF
#define VOP_AHD_C2_H             0x181d //3F
#define VOP_AHD_C3_L             0x181e //FF
#define VOP_AHD_C3_H             0x181f //3F
#define VOP_AHD_C4_L             0x1820 //FF
#define VOP_AHD_C4_H             0x1821 //3F
#define VOP_AHD_C5_L             0x1822 //FF
#define VOP_AHD_C5_H             0x1823 //3F
#define VOP_AHD_C6_L             0x1824 //FF
#define VOP_AHD_C6_H             0x1825 //3F
#define VOP_AHD_C7_L             0x1826 //FF
#define VOP_AHD_C7_H             0x1827 //3F
#define VOP_AHD_C8_L             0x1828 //FF
#define VOP_AHD_C8_H             0x1829 //3F
#define VOP_AHD_A0_L             0x182a //FF
#define VOP_AHD_A0_H             0x182b //FF
#define VOP_AHD_A1_L             0x182c //FF
#define VOP_AHD_A1_H             0x182d //FF
#define VOP_AHD_A2_L             0x182e //FF
#define VOP_AHD_A2_H             0x182f //FF
#define VOP_ASD_C0_L             0x1830 //FF
#define VOP_ASD_C0_H             0x1831 //3F
#define VOP_ASD_C1_L             0x1832 //FF
#define VOP_ASD_C1_H             0x1833 //3F
#define VOP_ASD_C2_L             0x1834 //FF
#define VOP_ASD_C2_H             0x1835 //3F
#define VOP_ASD_C3_L             0x1836 //FF
#define VOP_ASD_C3_H             0x1837 //3F
#define VOP_ASD_C4_L             0x1838 //FF
#define VOP_ASD_C4_H             0x1839 //3F
#define VOP_ASD_C5_L             0x183a //FF
#define VOP_ASD_C5_H             0x183b //3F
#define VOP_ASD_C6_L             0x183c //FF
#define VOP_ASD_C6_H             0x183d //3F
#define VOP_ASD_C7_L             0x183e //FF
#define VOP_ASD_C7_H             0x183f //3F
#define VOP_ASD_C8_L             0x1840 //FF
#define VOP_ASD_C8_H             0x1841 //3F
#define VOP_ASD_A0_L             0x1842 //FF
#define VOP_ASD_A0_H             0x1843 //FF
#define VOP_ASD_A1_L             0x1844 //FF
#define VOP_ASD_A1_H             0x1845 //FF
#define VOP_ASD_A2_L             0x1846 //FF
#define VOP_ASD_A2_H             0x1847 //FF
#define VOP_HDMI_C0_L            0x1848 //FF
#define VOP_HDMI_C0_H            0x1849 //3F
#define VOP_HDMI_C1_L            0x184a //FF
#define VOP_HDMI_C1_H            0x184b //3F
#define VOP_HDMI_C2_L            0x184c //FF
#define VOP_HDMI_C2_H            0x184d //3F
#define VOP_HDMI_C3_L            0x184e //FF
#define VOP_HDMI_C3_H            0x184f //3F
#define VOP_HDMI_C4_L            0x1850 //FF
#define VOP_HDMI_C4_H            0x1851 //3F
#define VOP_HDMI_C5_L            0x1852 //FF
#define VOP_HDMI_C5_H            0x1853 //3F
#define VOP_HDMI_C6_L            0x1854 //FF
#define VOP_HDMI_C6_H            0x1855 //3F
#define VOP_HDMI_C7_L            0x1856 //FF
#define VOP_HDMI_C7_H            0x1857 //3F
#define VOP_HDMI_C8_L            0x1858 //FF
#define VOP_HDMI_C8_H            0x1859 //3F
#define VOP_HDMI_A0_L            0x185a //FF
#define VOP_HDMI_A0_H            0x185b //FF
#define VOP_HDMI_A1_L            0x185c //FF
#define VOP_HDMI_A1_H            0x185d //FF
#define VOP_HDMI_A2_L            0x185e //FF
#define VOP_HDMI_A2_H            0x185f //FF
#define VOP_TTL24_C0_L           0x1860 //FF
#define VOP_TTL24_C0_H           0x1861 //3F
#define VOP_TTL24_C1_L           0x1862 //FF
#define VOP_TTL24_C1_H           0x1863 //3F
#define VOP_TTL24_C2_L           0x1864 //FF
#define VOP_TTL24_C2_H           0x1865 //3F
#define VOP_TTL24_C3_L           0x1866 //FF
#define VOP_TTL24_C3_H           0x1867 //3F
#define VOP_TTL24_C4_L           0x1868 //FF
#define VOP_TTL24_C4_H           0x1869 //3F
#define VOP_TTL24_C5_L           0x186a //FF
#define VOP_TTL24_C5_H           0x186b //3F
#define VOP_TTL24_C6_L           0x186c //FF
#define VOP_TTL24_C6_H           0x186d //3F
#define VOP_TTL24_C7_L           0x186e //FF
#define VOP_TTL24_C7_H           0x186f //3F
#define VOP_TTL24_C8_L           0x1870 //FF
#define VOP_TTL24_C8_H           0x1871 //3F
#define VOP_TTL24_A0_L           0x1872 //FF
#define VOP_TTL24_A0_H           0x1873 //FF
#define VOP_TTL24_A1_L           0x1874 //FF
#define VOP_TTL24_A1_H           0x1875 //FF
#define VOP_TTL24_A2_L           0x1876 //FF
#define VOP_TTL24_A2_H           0x1877 //FF
#define VOP_TTL30_C0_L           0x1878 //FF
#define VOP_TTL30_C0_H           0x1879 //3F
#define VOP_TTL30_C1_L           0x187a //FF
#define VOP_TTL30_C1_H           0x187b //3F
#define VOP_TTL30_C2_L           0x187c //FF
#define VOP_TTL30_C2_H           0x187d //3F
#define VOP_TTL30_C3_L           0x187e //FF
#define VOP_TTL30_C3_H           0x187f //3F
#define VOP_TTL30_C4_L           0x1880 //FF
#define VOP_TTL30_C4_H           0x1881 //3F
#define VOP_TTL30_C5_L           0x1882 //FF
#define VOP_TTL30_C5_H           0x1883 //3F
#define VOP_TTL30_C6_L           0x1884 //FF
#define VOP_TTL30_C6_H           0x1885 //3F
#define VOP_TTL30_C7_L           0x1886 //FF
#define VOP_TTL30_C7_H           0x1887 //3F
#define VOP_TTL30_C8_L           0x1888 //FF
#define VOP_TTL30_C8_H           0x1889 //3F
#define VOP_TTL30_A0_L           0x188a //FF
#define VOP_TTL30_A0_H           0x188b //FF
#define VOP_TTL30_A1_L           0x188c //FF
#define VOP_TTL30_A1_H           0x188d //FF
#define VOP_TTL30_A2_L           0x188e //FF
#define VOP_TTL30_A2_H           0x188f //FF

#define VOP_TTL30_TRIG           0x18C0 //01
#define VOP_TTL30_NOF            0x18C1 //FF
#define VOP_TTL30_SIG0           0x18C2 //FF
#define VOP_TTL30_SIG1           0x18C3 //FF
#define VOP_TTL30_SIG2           0x18C4 //FF
#define VOP_TTL30_SIG3           0x18C5 //FF
#define VOP_TTL30_SIG_ST         0x18C6 //07

#define VOP_TTL24_TRIG           0x18D0 //01
#define VOP_TTL24_NOF            0x18D1 //FF
#define VOP_TTL24_SIG0           0x18D2 //FF
#define VOP_TTL24_SIG1           0x18D3 //FF
#define VOP_TTL24_SIG2           0x18D4 //FF
#define VOP_TTL24_SIG3           0x18D5 //FF
#define VOP_TTL24_SIG_ST         0x18D6 //07
#define VOP_C656_SATURATE        0x18D7 //03
#define VOP_TTL24_CBLANK_L       0x18DC //FF
#define VOP_TTL24_CBLANK_H       0x18DD //0F
#define VOP_TTL24_MODE           0x18DE //01
#define VOP_CLKPOL               0x18DF //03

#define VOP_HDMI_SIG0            0x18E0 //FF
#define VOP_HDMI_SIG1            0x18E1 //FF
#define VOP_HDMI_SIG2            0x18E2 //FF
#define VOP_HDMI_SIG3            0x18E3 //FF
#define VOP_HDMI_SIG_ST          0x18E4 //07
#define VOP_HDMI_TRIG            0x18E5 //01
#define VOP_HDMI_NOF             0x18E6 //FF

#define VOP_ASD_SIG_ST           0x18E7 //07
#define VOP_ASD_SIG0             0x18E8 //FF
#define VOP_ASD_SIG1             0x18E9 //FF
#define VOP_ASD_SIG2             0x18EA //FF
#define VOP_ASD_SIG3             0x18EB //FF
#define VOP_ASD_TRIG             0x18EC //01
#define VOP_ASD_NOF              0x18ED //FF

#define VOP_AHD_SIG0             0x18F0 //07
#define VOP_AHD_SIG1             0x18F1 //FF
#define VOP_AHD_SIG2             0x18F2 //FF
#define VOP_AHD_SIG3             0x18F3 //FF
#define VOP_AHD_SIG_ST           0x18F4 //FF
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
#define CPCB0_EE0_CTRL          0x1490 //FF
#define CPCB0_EE0_LTIGAIN       0x1491 //FF
#define CPCB0_EE0_CTIGAIN       0x1492 //FF
#define CPCB0_EE1_CTRL          0x1493 //FF
#define CPCB0_EE1_LTIGAIN       0x1494 //FF
#define CPCB0_EE1_CTIGAIN       0x1495 //FF
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
#define  CPCB0_INT_CTRL         0x1510 //0F
#define  CPCB0_INT_MEMSIZE_L    0x1512 //FF
#define  CPCB0_INT_MEMSIZE_H    0x1513 //0F
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
#define  CPCB0_MISC_CTRL        0x152b //03

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
#define  CPCB2_INT_CTRL         0x1710 //0F
#define  CPCB2_INT_MEMSIZE_L    0x1712 //FF
#define  CPCB2_INT_MEMSIZE_H    0x1713 //0F
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

#define CMU_HS_M_EN                      0x1A81 //07
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

#define CMU_HS_P_EN                      0x1AF1 //07
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
#define CPCB0_P3B_SX_H          0x1E19 //FF
#define CPCB0_P3B_EX_L          0x1E1A //FF
#define CPCB0_P3B_EX_H          0x1E1B //FF
#define CPCB0_P3B_SY_L          0x1E1C //FF
#define CPCB0_P3B_SY_H          0x1E1D //FF
#define CPCB0_P3B_EY_L          0x1E1E //FF
#define CPCB0_P3B_EY_H          0x1E1F //FF

#define CPCB0_P3C_SX_L          0x1E20 //FF
#define CPCB0_P3C_SX_H          0x1E21 //FF
#define CPCB0_P3C_EX_L          0x1E22 //FF
#define CPCB0_P3C_EX_H          0x1E23 //FF
#define CPCB0_P3C_SY_L          0x1E24 //FF
#define CPCB0_P3C_SY_H          0x1E25 //FF
#define CPCB0_P3C_EY_L          0x1E26 //FF
#define CPCB0_P3C_EY_H          0x1E27 //FF

#define CPCB0_P3D_SX_L          0x1E28 //FF
#define CPCB0_P3D_SX_H          0x1E29 //FF
#define CPCB0_P3D_EX_L          0x1E2A //FF
#define CPCB0_P3D_EX_H          0x1E2B //FF
#define CPCB0_P3D_SY_L          0x1E2C //FF
#define CPCB0_P3D_SY_H          0x1E2D //FF
#define CPCB0_P3D_EY_L          0x1E2E //FF
#define CPCB0_P3D_EY_H          0x1E2F //FF

#define CPCB0_P7B_SX_L          0x1E30 //FF
#define CPCB0_P7B_SX_H          0x1E31 //FF
#define CPCB0_P7B_EX_L          0x1E32 //FF
#define CPCB0_P7B_EX_H          0x1E33 //FF
#define CPCB0_P7B_SY_L          0x1E34 //FF
#define CPCB0_P7B_SY_H          0x1E35 //FF
#define CPCB0_P7B_EY_L          0x1E36 //FF
#define CPCB0_P7B_EY_H          0x1E37 //FF

#define CPCB0_P7C_SX_L          0x1E38 //FF
#define CPCB0_P7C_SX_H          0x1E39 //FF
#define CPCB0_P7C_EX_L          0x1E3A //FF
#define CPCB0_P7C_EX_H          0x1E3B //FF
#define CPCB0_P7C_SY_L          0x1E3C //FF
#define CPCB0_P7C_SY_H          0x1E3D //FF
#define CPCB0_P7C_EY_L          0x1E3E //FF
#define CPCB0_P7C_EY_H          0x1E3F //FF

#define CPCB0_P7D_SX_L          0x1E40 //FF
#define CPCB0_P7D_SX_H          0x1E41 //FF
#define CPCB0_P7D_EX_L          0x1E42 //FF
#define CPCB0_P7D_EX_H          0x1E43 //FF
#define CPCB0_P7D_SY_L          0x1E44 //FF
#define CPCB0_P7D_SY_H          0x1E45 //FF
#define CPCB0_P7D_EY_L          0x1E46 //FF
#define CPCB0_P7D_EY_H          0x1E47 //FF

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
#define CPCB0_OO_LAY7           0x1E55 //0F

#define CPCB0_VO_KEY_MIN_0      0x1E56 //FF
#define CPCB0_VO_KEY_MIN_1      0x1E57 //FF
#define CPCB0_VO_KEY_MIN_2      0x1E58 //FF
#define CPCB0_VO_KEY_MAX_0      0x1E59 //FF
#define CPCB0_VO_KEY_MAX_1      0x1E5A //FF
#define CPCB0_VO_KEY_MAX_2      0x1E5B //FF
#define CPCB0_VO_KEY_EN         0x1E5C //FF

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
#define CPCB1_P3B_SX_H          0x1E81 //FF
#define CPCB1_P3B_EX_L          0x1E82 //FF
#define CPCB1_P3B_EX_H          0x1E83 //FF
#define CPCB1_P3B_SY_L          0x1E84 //FF
#define CPCB1_P3B_SY_H          0x1E85 //FF
#define CPCB1_P3B_EY_L          0x1E86 //FF
#define CPCB1_P3B_EY_H          0x1E87 //FF

#define CPCB1_P3C_SX_L          0x1E88 //FF
#define CPCB1_P3C_SX_H          0x1E89 //FF
#define CPCB1_P3C_EX_L          0x1E8A //FF
#define CPCB1_P3C_EX_H          0x1E8B //FF
#define CPCB1_P3C_SY_L          0x1E8C //FF
#define CPCB1_P3C_SY_H          0x1E8D //FF
#define CPCB1_P3C_EY_L          0x1E8E //FF
#define CPCB1_P3C_EY_H          0x1E8F //FF

#define CPCB1_P3D_SX_L          0x1E90 //FF
#define CPCB1_P3D_SX_H          0x1E91 //FF
#define CPCB1_P3D_EX_L          0x1E92 //FF
#define CPCB1_P3D_EX_H          0x1E93 //FF
#define CPCB1_P3D_SY_L          0x1E94 //FF
#define CPCB1_P3D_SY_H          0x1E95 //FF
#define CPCB1_P3D_EY_L          0x1E96 //FF
#define CPCB1_P3D_EY_H          0x1E97 //FF

#define CPCB1_P7B_SX_L          0x1E98 //FF
#define CPCB1_P7B_SX_H          0x1E99 //FF
#define CPCB1_P7B_EX_L          0x1E9A //FF
#define CPCB1_P7B_EX_H          0x1E9B //FF
#define CPCB1_P7B_SY_L          0x1E9C //FF
#define CPCB1_P7B_SY_H          0x1E9D //FF
#define CPCB1_P7B_EY_L          0x1E9E //FF
#define CPCB1_P7B_EY_H          0x1E9F //FF

#define CPCB1_P7C_SX_L          0x1EA0 //FF
#define CPCB1_P7C_SX_H          0x1EA1 //FF
#define CPCB1_P7C_EX_L          0x1EA2 //FF
#define CPCB1_P7C_EX_H          0x1EA3 //FF
#define CPCB1_P7C_SY_L          0x1EA4 //FF
#define CPCB1_P7C_SY_H          0x1EA5 //FF
#define CPCB1_P7C_EY_L          0x1EA6 //FF
#define CPCB1_P7C_EY_H          0x1EA7 //FF

#define CPCB1_P7D_SX_L          0x1EA8 //FF
#define CPCB1_P7D_SX_H          0x1EA9 //FF
#define CPCB1_P7D_EX_L          0x1EAA //FF
#define CPCB1_P7D_EX_H          0x1EAB //FF
#define CPCB1_P7D_SY_L          0x1EAC //FF
#define CPCB1_P7D_SY_H          0x1EAD //FF
#define CPCB1_P7D_EY_L          0x1EAE //FF
#define CPCB1_P7D_EY_H          0x1EAF //FF

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
#define CPCB1_OO_LAY7           0x1EBD //0F

#define CPCB1_VO_KEY_MIN_0      0x1EBE //FF
#define CPCB1_VO_KEY_MIN_1      0x1EBF //FF
#define CPCB1_VO_KEY_MIN_2      0x1EC0 //FF
#define CPCB1_VO_KEY_MAX_0      0x1EC1 //FF
#define CPCB1_VO_KEY_MAX_1      0x1EC2 //FF
#define CPCB1_VO_KEY_MAX_2      0x1EC3 //FF
#define CPCB1_VO_KEY_EN         0x1EC4 //FF

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

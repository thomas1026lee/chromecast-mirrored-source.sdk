//////
/// don't edit! auto-generated by docc: pex.h
////////////////////////////////////////////////////////////
#ifndef pex_h
#define pex_h (){}


#include "ctypes.h"

#pragma pack(1)
#ifdef __cplusplus
  extern "C" {
#endif

#ifndef _DOCC_H_BITOPS_
#define _DOCC_H_BITOPS_ (){}

    #define _bSETMASK_(b)                                      ((b)<32 ? (1<<((b)&31)) : 0)
    #define _NSETMASK_(msb,lsb)                                (_bSETMASK_((msb)+1)-_bSETMASK_(lsb))
    #define _bCLRMASK_(b)                                      (~_bSETMASK_(b))
    #define _NCLRMASK_(msb,lsb)                                (~_NSETMASK_(msb,lsb))
    #define _BFGET_(r,msb,lsb)                                 (_NSETMASK_((msb)-(lsb),0)&((r)>>(lsb)))
    #define _BFSET_(r,msb,lsb,v)                               do{ (r)&=_NCLRMASK_(msb,lsb); (r)|=_NSETMASK_(msb,lsb)&((v)<<(lsb)); }while(0)

#endif



//////
/// 
/// $INTERFACE oneReg                                   (4,4)
///     # # ----------------------------------------------------------
///     @ 0x00000                      (R-)
///               %unsigned 32 0x00000000                
///                                    ###
///                                    * One Register in an external IP block (i.e., PEX)
///                                    ###
///     # # ----------------------------------------------------------
/// $ENDOFINTERFACE  # size:       4B, bits:      32b, padding:     0B
////////////////////////////////////////////////////////////
#ifndef h_oneReg
#define h_oneReg (){}

    #define     BA_oneReg_0x00000000                           0x0000
    #define     B16oneReg_0x00000000                           0x0000
    #define   LSb32oneReg_0x00000000                              0
    #define   LSb16oneReg_0x00000000                              0
    #define       boneReg_0x00000000                           32
    #define   MSK32oneReg_0x00000000                              0xFFFFFFFF
    ///////////////////////////////////////////////////////////

    typedef struct SIE_oneReg {
    ///////////////////////////////////////////////////////////
    #define   GET32oneReg_0x00000000(r32)                      _BFGET_(r32,31, 0)
    #define   SET32oneReg_0x00000000(r32,v)                    _BFSET_(r32,31, 0,v)

            UNSG32 u_0x00000000                                : 32;
    ///////////////////////////////////////////////////////////
    } SIE_oneReg;

    ///////////////////////////////////////////////////////////
     SIGN32 oneReg_drvrd(SIE_oneReg *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 oneReg_drvwr(SIE_oneReg *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void oneReg_reset(SIE_oneReg *p);
     SIGN32 oneReg_cmp  (SIE_oneReg *p, SIE_oneReg *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define oneReg_check(p,pie,pfx,hLOG) oneReg_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define oneReg_print(p,    pfx,hLOG) oneReg_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif
//////
/// ENDOFINTERFACE: oneReg
////////////////////////////////////////////////////////////

//////
/// 
/// $INTERFACE PEX_REG                                  (4,4)
///     # # ----------------------------------------------------------
///     @ 0x00000                      (P)
///     # 0x00000 dummy                
///               $oneReg              dummy             REG   [16384]
///     # # ----------------------------------------------------------
/// $ENDOFINTERFACE  # size:   65536B, bits:  524288b, padding:     0B
////////////////////////////////////////////////////////////
#ifndef h_PEX_REG
#define h_PEX_REG (){}

    #define     RA_PEX_REG_dummy                               0x0000
    ///////////////////////////////////////////////////////////

    typedef struct SIE_PEX_REG {
    ///////////////////////////////////////////////////////////
              SIE_oneReg                                       ie_dummy[16384];
    ///////////////////////////////////////////////////////////
    } SIE_PEX_REG;

    ///////////////////////////////////////////////////////////
     SIGN32 PEX_REG_drvrd(SIE_PEX_REG *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 PEX_REG_drvwr(SIE_PEX_REG *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void PEX_REG_reset(SIE_PEX_REG *p);
     SIGN32 PEX_REG_cmp  (SIE_PEX_REG *p, SIE_PEX_REG *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define PEX_REG_check(p,pie,pfx,hLOG) PEX_REG_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define PEX_REG_print(p,    pfx,hLOG) PEX_REG_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif
//////
/// ENDOFINTERFACE: PEX_REG
////////////////////////////////////////////////////////////

//////
/// 
/// $INTERFACE Pex                     biu              (4,4)
///     # # ----------------------------------------------------------
///     @ 0x00000                      (R-)
///     # 0x00000 cfgReg               
///               $PEX_REG             cfgReg            MEM          
///                                    ###
///                                    * PEX Register Space, 64KB. (Hier:3)
///                                    ###
///     @ 0x10000 dummy1               (R-)
///               %unsigned 32 STS                       0x0
///                                    ###
///                                    * Dummy register for ISC to generate the BIU.
///                                    ###
///     @ 0x10004                      (W-)
///     #         # Stuffing bytes...
///               %% 524256
///     # # ----------------------------------------------------------
/// $ENDOFINTERFACE  # size:  131072B, bits:      64b, padding:     0B
////////////////////////////////////////////////////////////
#ifndef h_Pex
#define h_Pex (){}

    #define     RA_Pex_cfgReg                                  0x0000
    ///////////////////////////////////////////////////////////
    #define     RA_Pex_dummy1                                  0x10000

    #define     BA_Pex_dummy1_STS                              0x10000
    #define     B16Pex_dummy1_STS                              0x10000
    #define   LSb32Pex_dummy1_STS                                 0
    #define   LSb16Pex_dummy1_STS                                 0
    #define       bPex_dummy1_STS                              32
    #define   MSK32Pex_dummy1_STS                                 0xFFFFFFFF
    ///////////////////////////////////////////////////////////

    typedef struct SIE_Pex {
    ///////////////////////////////////////////////////////////
              SIE_PEX_REG                                      ie_cfgReg;
    ///////////////////////////////////////////////////////////
    #define   GET32Pex_dummy1_STS(r32)                         _BFGET_(r32,31, 0)
    #define   SET32Pex_dummy1_STS(r32,v)                       _BFSET_(r32,31, 0,v)

    #define     w32Pex_dummy1                                  {\
            UNSG32 udummy1_STS                                 : 32;\
          }
    union { UNSG32 u32Pex_dummy1;
            struct w32Pex_dummy1;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx10004                                  [65532];
    ///////////////////////////////////////////////////////////
    } SIE_Pex;

    typedef union  T32Pex_dummy1
          { UNSG32 u32;
            struct w32Pex_dummy1;
                 } T32Pex_dummy1;
    ///////////////////////////////////////////////////////////

    typedef union  TPex_dummy1
          { UNSG32 u32[1];
            struct {
            struct w32Pex_dummy1;
                   };
                 } TPex_dummy1;

    ///////////////////////////////////////////////////////////
     SIGN32 Pex_drvrd(SIE_Pex *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 Pex_drvwr(SIE_Pex *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void Pex_reset(SIE_Pex *p);
     SIGN32 Pex_cmp  (SIE_Pex *p, SIE_Pex *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define Pex_check(p,pie,pfx,hLOG) Pex_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define Pex_print(p,    pfx,hLOG) Pex_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif
//////
/// ENDOFINTERFACE: Pex
////////////////////////////////////////////////////////////



#ifdef __cplusplus
  }
#endif
#pragma  pack()

#endif
//////
/// ENDOFFILE: pex.h
////////////////////////////////////////////////////////////


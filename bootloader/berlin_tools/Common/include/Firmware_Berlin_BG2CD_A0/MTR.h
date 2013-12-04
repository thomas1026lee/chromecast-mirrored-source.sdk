//////
/// don't edit! auto-generated by docc: MTR.h
////////////////////////////////////////////////////////////
#ifndef MTR_h
#define MTR_h (){}


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
/// $INTERFACE streamCtl               biu              (4,4)
///     # # ----------------------------------------------------------
///     @ 0x00000                      (P)
///               ###
///               * The base address of the frame
///               ###
///               %unsigned 32 baseAddr                  0x0
///     @ 0x00004                      (P)
///               ###
///               * The width of the frame
///               ###
///               %unsigned 16 width                     0x0
///               %%        16         # Stuffing bits...
///     @ 0x00008                      (P)
///               ###
///               * The stride of the DDR image. That is the distance between on pixel and the pixel directly below it.
///               ###
///               %unsigned 16 stride                    0x0
///               %%        16         # Stuffing bits...
///     @ 0x0000C                      (P)
///               ###
///               * The height of the frame
///               ###
///               %unsigned 16 height                    0x0
///               %%        16         # Stuffing bits...
///     @ 0x00010                      (P)
///               ###
///               * The intended value quantization parameters
///               ###
///               %unsigned 8  qp                        0x0
///               %%        24         # Stuffing bits...
///     @ 0x00014                      (P)
///               ###
///               * The target compression ratio
///               * Note that the MTR only supports bandwidth compression, but not memory footprint compression. We may support an optional hard compression ratio cap so we can reduce memory footprint as well.
///               ###
///               %unsigned 8  compressRatio             0x0
///               %%        24         # Stuffing bits...
///     @ 0x00018                      (P)
///               ###
///               * Compression scheme
///               * Note: Scheme C not supported in our current application
///               ###
///               %unsigned 3  scheme                    0x0
///                                    : A                         0x0
///                                    : B                         0x1
///                                    : C                         0x2
///                                    : D                         0x3
///                                    : E                         0x4
///               %%        29         # Stuffing bits...
///     # # ----------------------------------------------------------
/// $ENDOFINTERFACE  # size:      28B, bits:      99b, padding:     0B
////////////////////////////////////////////////////////////
#ifndef h_streamCtl
#define h_streamCtl (){}

    #define     BA_streamCtl_baseAddr                          0x0000
    #define     B16streamCtl_baseAddr                          0x0000
    #define   LSb32streamCtl_baseAddr                             0
    #define   LSb16streamCtl_baseAddr                             0
    #define       bstreamCtl_baseAddr                          32
    #define   MSK32streamCtl_baseAddr                             0xFFFFFFFF
    ///////////////////////////////////////////////////////////

    #define     BA_streamCtl_width                             0x0004
    #define     B16streamCtl_width                             0x0004
    #define   LSb32streamCtl_width                                0
    #define   LSb16streamCtl_width                                0
    #define       bstreamCtl_width                             16
    #define   MSK32streamCtl_width                                0x0000FFFF
    ///////////////////////////////////////////////////////////

    #define     BA_streamCtl_stride                            0x0008
    #define     B16streamCtl_stride                            0x0008
    #define   LSb32streamCtl_stride                               0
    #define   LSb16streamCtl_stride                               0
    #define       bstreamCtl_stride                            16
    #define   MSK32streamCtl_stride                               0x0000FFFF
    ///////////////////////////////////////////////////////////

    #define     BA_streamCtl_height                            0x000C
    #define     B16streamCtl_height                            0x000C
    #define   LSb32streamCtl_height                               0
    #define   LSb16streamCtl_height                               0
    #define       bstreamCtl_height                            16
    #define   MSK32streamCtl_height                               0x0000FFFF
    ///////////////////////////////////////////////////////////

    #define     BA_streamCtl_qp                                0x0010
    #define     B16streamCtl_qp                                0x0010
    #define   LSb32streamCtl_qp                                   0
    #define   LSb16streamCtl_qp                                   0
    #define       bstreamCtl_qp                                8
    #define   MSK32streamCtl_qp                                   0x000000FF
    ///////////////////////////////////////////////////////////

    #define     BA_streamCtl_compressRatio                     0x0014
    #define     B16streamCtl_compressRatio                     0x0014
    #define   LSb32streamCtl_compressRatio                        0
    #define   LSb16streamCtl_compressRatio                        0
    #define       bstreamCtl_compressRatio                     8
    #define   MSK32streamCtl_compressRatio                        0x000000FF
    ///////////////////////////////////////////////////////////

    #define     BA_streamCtl_scheme                            0x0018
    #define     B16streamCtl_scheme                            0x0018
    #define   LSb32streamCtl_scheme                               0
    #define   LSb16streamCtl_scheme                               0
    #define       bstreamCtl_scheme                            3
    #define   MSK32streamCtl_scheme                               0x00000007
    #define        streamCtl_scheme_A                                       0x0
    #define        streamCtl_scheme_B                                       0x1
    #define        streamCtl_scheme_C                                       0x2
    #define        streamCtl_scheme_D                                       0x3
    #define        streamCtl_scheme_E                                       0x4
    ///////////////////////////////////////////////////////////

    typedef struct SIE_streamCtl {
    ///////////////////////////////////////////////////////////
    #define   GET32streamCtl_baseAddr(r32)                     _BFGET_(r32,31, 0)
    #define   SET32streamCtl_baseAddr(r32,v)                   _BFSET_(r32,31, 0,v)

            UNSG32 u_baseAddr                                  : 32;
    ///////////////////////////////////////////////////////////
    #define   GET32streamCtl_width(r32)                        _BFGET_(r32,15, 0)
    #define   SET32streamCtl_width(r32,v)                      _BFSET_(r32,15, 0,v)
    #define   GET16streamCtl_width(r16)                        _BFGET_(r16,15, 0)
    #define   SET16streamCtl_width(r16,v)                      _BFSET_(r16,15, 0,v)

            UNSG32 u_width                                     : 16;
            UNSG32 RSVDx4_b16                                  : 16;
    ///////////////////////////////////////////////////////////
    #define   GET32streamCtl_stride(r32)                       _BFGET_(r32,15, 0)
    #define   SET32streamCtl_stride(r32,v)                     _BFSET_(r32,15, 0,v)
    #define   GET16streamCtl_stride(r16)                       _BFGET_(r16,15, 0)
    #define   SET16streamCtl_stride(r16,v)                     _BFSET_(r16,15, 0,v)

            UNSG32 u_stride                                    : 16;
            UNSG32 RSVDx8_b16                                  : 16;
    ///////////////////////////////////////////////////////////
    #define   GET32streamCtl_height(r32)                       _BFGET_(r32,15, 0)
    #define   SET32streamCtl_height(r32,v)                     _BFSET_(r32,15, 0,v)
    #define   GET16streamCtl_height(r16)                       _BFGET_(r16,15, 0)
    #define   SET16streamCtl_height(r16,v)                     _BFSET_(r16,15, 0,v)

            UNSG32 u_height                                    : 16;
            UNSG32 RSVDxC_b16                                  : 16;
    ///////////////////////////////////////////////////////////
    #define   GET32streamCtl_qp(r32)                           _BFGET_(r32, 7, 0)
    #define   SET32streamCtl_qp(r32,v)                         _BFSET_(r32, 7, 0,v)
    #define   GET16streamCtl_qp(r16)                           _BFGET_(r16, 7, 0)
    #define   SET16streamCtl_qp(r16,v)                         _BFSET_(r16, 7, 0,v)

            UNSG32 u_qp                                        :  8;
            UNSG32 RSVDx10_b8                                  : 24;
    ///////////////////////////////////////////////////////////
    #define   GET32streamCtl_compressRatio(r32)                _BFGET_(r32, 7, 0)
    #define   SET32streamCtl_compressRatio(r32,v)              _BFSET_(r32, 7, 0,v)
    #define   GET16streamCtl_compressRatio(r16)                _BFGET_(r16, 7, 0)
    #define   SET16streamCtl_compressRatio(r16,v)              _BFSET_(r16, 7, 0,v)

            UNSG32 u_compressRatio                             :  8;
            UNSG32 RSVDx14_b8                                  : 24;
    ///////////////////////////////////////////////////////////
    #define   GET32streamCtl_scheme(r32)                       _BFGET_(r32, 2, 0)
    #define   SET32streamCtl_scheme(r32,v)                     _BFSET_(r32, 2, 0,v)
    #define   GET16streamCtl_scheme(r16)                       _BFGET_(r16, 2, 0)
    #define   SET16streamCtl_scheme(r16,v)                     _BFSET_(r16, 2, 0,v)

            UNSG32 u_scheme                                    :  3;
            UNSG32 RSVDx18_b3                                  : 29;
    ///////////////////////////////////////////////////////////
    } SIE_streamCtl;

    ///////////////////////////////////////////////////////////
     SIGN32 streamCtl_drvrd(SIE_streamCtl *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 streamCtl_drvwr(SIE_streamCtl *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void streamCtl_reset(SIE_streamCtl *p);
     SIGN32 streamCtl_cmp  (SIE_streamCtl *p, SIE_streamCtl *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define streamCtl_check(p,pie,pfx,hLOG) streamCtl_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define streamCtl_print(p,    pfx,hLOG) streamCtl_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif
//////
/// ENDOFINTERFACE: streamCtl
////////////////////////////////////////////////////////////

//////
/// 
/// $INTERFACE zOrderCtl               biu              (4,4)
///     # # ----------------------------------------------------------
///     @ 0x00000 baseAddr             (P)
///               ###
///               * The base address of the Z-order rectangle
///               ###
///               %unsigned 32 val                       0x0
///                                    ###
///                                    * Must be 128-byte (AXI burst x16) aligned. Lower 7-bits will be ignored.
///                                    ###
///     @ 0x00004 width                (P)
///               ###
///               * The width of the Z-order rectangle
///               ###
///               %unsigned 13 val                       0x0
///                                    ###
///                                    * Max supported width is 2048 pixels for 32-bit pixels, 4096 for 16-bit pixels, and 8192 for 8-bit pixels.
///                                    ###
///               %%        19         # Stuffing bits...
///     @ 0x00008 stride               (P)
///               ###
///               * The stride of the DDR image. That is the distance between one pixel and the pixel directly below it.
///               ###
///               %unsigned 16 val                       0x0
///                                    ###
///                                    * Must be 128-byte (AXI burst x16) aligned. Lower 7-bits will be ignored. Max supported stride is 16k pixels for 32-bit pixels, 32k pixels for 16-bit pixels, and 64k for 8-bit pixels.
///                                    ###
///               %%        16         # Stuffing bits...
///     @ 0x0000C height               (P)
///               ###
///               * The height of the Z-order rectangle
///               ###
///               %unsigned 11 val                       0x0
///                                    ###
///                                    * Max supported height is 2048 pixels.
///                                    ###
///               %%        21         # Stuffing bits...
///     @ 0x00010 active               (P)
///               ###
///               * Bit indicating whether the current rectangle filtering is active
///               ###
///               %unsigned 1  val                       0x0
///               %%        31         # Stuffing bits...
///     @ 0x00014 visible              (P)
///               ###
///               * Bit indicating whether the current region is visible
///               ###
///               %unsigned 1  val                       0x0
///               %%        31         # Stuffing bits...
///     # # ----------------------------------------------------------
/// $ENDOFINTERFACE  # size:      24B, bits:      74b, padding:     0B
////////////////////////////////////////////////////////////
#ifndef h_zOrderCtl
#define h_zOrderCtl (){}

    #define     RA_zOrderCtl_baseAddr                          0x0000

    #define     BA_zOrderCtl_baseAddr_val                      0x0000
    #define     B16zOrderCtl_baseAddr_val                      0x0000
    #define   LSb32zOrderCtl_baseAddr_val                         0
    #define   LSb16zOrderCtl_baseAddr_val                         0
    #define       bzOrderCtl_baseAddr_val                      32
    #define   MSK32zOrderCtl_baseAddr_val                         0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_zOrderCtl_width                             0x0004

    #define     BA_zOrderCtl_width_val                         0x0004
    #define     B16zOrderCtl_width_val                         0x0004
    #define   LSb32zOrderCtl_width_val                            0
    #define   LSb16zOrderCtl_width_val                            0
    #define       bzOrderCtl_width_val                         13
    #define   MSK32zOrderCtl_width_val                            0x00001FFF
    ///////////////////////////////////////////////////////////
    #define     RA_zOrderCtl_stride                            0x0008

    #define     BA_zOrderCtl_stride_val                        0x0008
    #define     B16zOrderCtl_stride_val                        0x0008
    #define   LSb32zOrderCtl_stride_val                           0
    #define   LSb16zOrderCtl_stride_val                           0
    #define       bzOrderCtl_stride_val                        16
    #define   MSK32zOrderCtl_stride_val                           0x0000FFFF
    ///////////////////////////////////////////////////////////
    #define     RA_zOrderCtl_height                            0x000C

    #define     BA_zOrderCtl_height_val                        0x000C
    #define     B16zOrderCtl_height_val                        0x000C
    #define   LSb32zOrderCtl_height_val                           0
    #define   LSb16zOrderCtl_height_val                           0
    #define       bzOrderCtl_height_val                        11
    #define   MSK32zOrderCtl_height_val                           0x000007FF
    ///////////////////////////////////////////////////////////
    #define     RA_zOrderCtl_active                            0x0010

    #define     BA_zOrderCtl_active_val                        0x0010
    #define     B16zOrderCtl_active_val                        0x0010
    #define   LSb32zOrderCtl_active_val                           0
    #define   LSb16zOrderCtl_active_val                           0
    #define       bzOrderCtl_active_val                        1
    #define   MSK32zOrderCtl_active_val                           0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_zOrderCtl_visible                           0x0014

    #define     BA_zOrderCtl_visible_val                       0x0014
    #define     B16zOrderCtl_visible_val                       0x0014
    #define   LSb32zOrderCtl_visible_val                          0
    #define   LSb16zOrderCtl_visible_val                          0
    #define       bzOrderCtl_visible_val                       1
    #define   MSK32zOrderCtl_visible_val                          0x00000001
    ///////////////////////////////////////////////////////////

    typedef struct SIE_zOrderCtl {
    ///////////////////////////////////////////////////////////
    #define   GET32zOrderCtl_baseAddr_val(r32)                 _BFGET_(r32,31, 0)
    #define   SET32zOrderCtl_baseAddr_val(r32,v)               _BFSET_(r32,31, 0,v)

    #define     w32zOrderCtl_baseAddr                          {\
            UNSG32 ubaseAddr_val                               : 32;\
          }
    union { UNSG32 u32zOrderCtl_baseAddr;
            struct w32zOrderCtl_baseAddr;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32zOrderCtl_width_val(r32)                    _BFGET_(r32,12, 0)
    #define   SET32zOrderCtl_width_val(r32,v)                  _BFSET_(r32,12, 0,v)
    #define   GET16zOrderCtl_width_val(r16)                    _BFGET_(r16,12, 0)
    #define   SET16zOrderCtl_width_val(r16,v)                  _BFSET_(r16,12, 0,v)

    #define     w32zOrderCtl_width                             {\
            UNSG32 uwidth_val                                  : 13;\
            UNSG32 RSVDx4_b13                                  : 19;\
          }
    union { UNSG32 u32zOrderCtl_width;
            struct w32zOrderCtl_width;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32zOrderCtl_stride_val(r32)                   _BFGET_(r32,15, 0)
    #define   SET32zOrderCtl_stride_val(r32,v)                 _BFSET_(r32,15, 0,v)
    #define   GET16zOrderCtl_stride_val(r16)                   _BFGET_(r16,15, 0)
    #define   SET16zOrderCtl_stride_val(r16,v)                 _BFSET_(r16,15, 0,v)

    #define     w32zOrderCtl_stride                            {\
            UNSG32 ustride_val                                 : 16;\
            UNSG32 RSVDx8_b16                                  : 16;\
          }
    union { UNSG32 u32zOrderCtl_stride;
            struct w32zOrderCtl_stride;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32zOrderCtl_height_val(r32)                   _BFGET_(r32,10, 0)
    #define   SET32zOrderCtl_height_val(r32,v)                 _BFSET_(r32,10, 0,v)
    #define   GET16zOrderCtl_height_val(r16)                   _BFGET_(r16,10, 0)
    #define   SET16zOrderCtl_height_val(r16,v)                 _BFSET_(r16,10, 0,v)

    #define     w32zOrderCtl_height                            {\
            UNSG32 uheight_val                                 : 11;\
            UNSG32 RSVDxC_b11                                  : 21;\
          }
    union { UNSG32 u32zOrderCtl_height;
            struct w32zOrderCtl_height;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32zOrderCtl_active_val(r32)                   _BFGET_(r32, 0, 0)
    #define   SET32zOrderCtl_active_val(r32,v)                 _BFSET_(r32, 0, 0,v)
    #define   GET16zOrderCtl_active_val(r16)                   _BFGET_(r16, 0, 0)
    #define   SET16zOrderCtl_active_val(r16,v)                 _BFSET_(r16, 0, 0,v)

    #define     w32zOrderCtl_active                            {\
            UNSG32 uactive_val                                 :  1;\
            UNSG32 RSVDx10_b1                                  : 31;\
          }
    union { UNSG32 u32zOrderCtl_active;
            struct w32zOrderCtl_active;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32zOrderCtl_visible_val(r32)                  _BFGET_(r32, 0, 0)
    #define   SET32zOrderCtl_visible_val(r32,v)                _BFSET_(r32, 0, 0,v)
    #define   GET16zOrderCtl_visible_val(r16)                  _BFGET_(r16, 0, 0)
    #define   SET16zOrderCtl_visible_val(r16,v)                _BFSET_(r16, 0, 0,v)

    #define     w32zOrderCtl_visible                           {\
            UNSG32 uvisible_val                                :  1;\
            UNSG32 RSVDx14_b1                                  : 31;\
          }
    union { UNSG32 u32zOrderCtl_visible;
            struct w32zOrderCtl_visible;
          };
    ///////////////////////////////////////////////////////////
    } SIE_zOrderCtl;

    typedef union  T32zOrderCtl_baseAddr
          { UNSG32 u32;
            struct w32zOrderCtl_baseAddr;
                 } T32zOrderCtl_baseAddr;
    typedef union  T32zOrderCtl_width
          { UNSG32 u32;
            struct w32zOrderCtl_width;
                 } T32zOrderCtl_width;
    typedef union  T32zOrderCtl_stride
          { UNSG32 u32;
            struct w32zOrderCtl_stride;
                 } T32zOrderCtl_stride;
    typedef union  T32zOrderCtl_height
          { UNSG32 u32;
            struct w32zOrderCtl_height;
                 } T32zOrderCtl_height;
    typedef union  T32zOrderCtl_active
          { UNSG32 u32;
            struct w32zOrderCtl_active;
                 } T32zOrderCtl_active;
    typedef union  T32zOrderCtl_visible
          { UNSG32 u32;
            struct w32zOrderCtl_visible;
                 } T32zOrderCtl_visible;
    ///////////////////////////////////////////////////////////

    typedef union  TzOrderCtl_baseAddr
          { UNSG32 u32[1];
            struct {
            struct w32zOrderCtl_baseAddr;
                   };
                 } TzOrderCtl_baseAddr;
    typedef union  TzOrderCtl_width
          { UNSG32 u32[1];
            struct {
            struct w32zOrderCtl_width;
                   };
                 } TzOrderCtl_width;
    typedef union  TzOrderCtl_stride
          { UNSG32 u32[1];
            struct {
            struct w32zOrderCtl_stride;
                   };
                 } TzOrderCtl_stride;
    typedef union  TzOrderCtl_height
          { UNSG32 u32[1];
            struct {
            struct w32zOrderCtl_height;
                   };
                 } TzOrderCtl_height;
    typedef union  TzOrderCtl_active
          { UNSG32 u32[1];
            struct {
            struct w32zOrderCtl_active;
                   };
                 } TzOrderCtl_active;
    typedef union  TzOrderCtl_visible
          { UNSG32 u32[1];
            struct {
            struct w32zOrderCtl_visible;
                   };
                 } TzOrderCtl_visible;

    ///////////////////////////////////////////////////////////
     SIGN32 zOrderCtl_drvrd(SIE_zOrderCtl *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 zOrderCtl_drvwr(SIE_zOrderCtl *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void zOrderCtl_reset(SIE_zOrderCtl *p);
     SIGN32 zOrderCtl_cmp  (SIE_zOrderCtl *p, SIE_zOrderCtl *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define zOrderCtl_check(p,pie,pfx,hLOG) zOrderCtl_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define zOrderCtl_print(p,    pfx,hLOG) zOrderCtl_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif
//////
/// ENDOFINTERFACE: zOrderCtl
////////////////////////////////////////////////////////////

//////
/// 
/// $INTERFACE mtr                     biu              (4,4)
///     # # ----------------------------------------------------------
///     @ 0x00000 zOrderCtl            (P)
///     # 0x00000 zOAry                
///               $zOrderCtl           zOAry             REG       [4]
///                                    ###
///                                    * 4 rectangles are defined
///                                    ###
///     @ 0x00060 zOActive             (P)
///               ###
///               * Bit indicating whether Z-order filtering is active
///               ###
///               %unsigned 1  val                       0x0
///               %%        31         # Stuffing bits...
///     @ 0x00064 zOBypLength          (P)
///               ###
///               * Specifies the upper limit of the transaction burst length that will always be bypassed
///               ###
///               %unsigned 4  val                       0x3
///                                    ###
///                                    * Transactions of burst length < bypLength will be bypassed.
///                                    ###
///               %%        28         # Stuffing bits...
///     # # ----------------------------------------------------------
/// $ENDOFINTERFACE  # size:     104B, bits:     301b, padding:     0B
////////////////////////////////////////////////////////////
#ifndef h_mtr
#define h_mtr (){}

    #define     RA_mtr_zOrderCtl                               0x0000
    #define     RA_mtr_zOAry                                   0x0000
    ///////////////////////////////////////////////////////////
    #define     RA_mtr_zOActive                                0x0060

    #define     BA_mtr_zOActive_val                            0x0060
    #define     B16mtr_zOActive_val                            0x0060
    #define   LSb32mtr_zOActive_val                               0
    #define   LSb16mtr_zOActive_val                               0
    #define       bmtr_zOActive_val                            1
    #define   MSK32mtr_zOActive_val                               0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_mtr_zOBypLength                             0x0064

    #define     BA_mtr_zOBypLength_val                         0x0064
    #define     B16mtr_zOBypLength_val                         0x0064
    #define   LSb32mtr_zOBypLength_val                            0
    #define   LSb16mtr_zOBypLength_val                            0
    #define       bmtr_zOBypLength_val                         4
    #define   MSK32mtr_zOBypLength_val                            0x0000000F
    ///////////////////////////////////////////////////////////

    typedef struct SIE_mtr {
    ///////////////////////////////////////////////////////////
              SIE_zOrderCtl                                    ie_zOAry[4];
    ///////////////////////////////////////////////////////////
    #define   GET32mtr_zOActive_val(r32)                       _BFGET_(r32, 0, 0)
    #define   SET32mtr_zOActive_val(r32,v)                     _BFSET_(r32, 0, 0,v)
    #define   GET16mtr_zOActive_val(r16)                       _BFGET_(r16, 0, 0)
    #define   SET16mtr_zOActive_val(r16,v)                     _BFSET_(r16, 0, 0,v)

    #define     w32mtr_zOActive                                {\
            UNSG32 uzOActive_val                               :  1;\
            UNSG32 RSVDx60_b1                                  : 31;\
          }
    union { UNSG32 u32mtr_zOActive;
            struct w32mtr_zOActive;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32mtr_zOBypLength_val(r32)                    _BFGET_(r32, 3, 0)
    #define   SET32mtr_zOBypLength_val(r32,v)                  _BFSET_(r32, 3, 0,v)
    #define   GET16mtr_zOBypLength_val(r16)                    _BFGET_(r16, 3, 0)
    #define   SET16mtr_zOBypLength_val(r16,v)                  _BFSET_(r16, 3, 0,v)

    #define     w32mtr_zOBypLength                             {\
            UNSG32 uzOBypLength_val                            :  4;\
            UNSG32 RSVDx64_b4                                  : 28;\
          }
    union { UNSG32 u32mtr_zOBypLength;
            struct w32mtr_zOBypLength;
          };
    ///////////////////////////////////////////////////////////
    } SIE_mtr;

    typedef union  T32mtr_zOActive
          { UNSG32 u32;
            struct w32mtr_zOActive;
                 } T32mtr_zOActive;
    typedef union  T32mtr_zOBypLength
          { UNSG32 u32;
            struct w32mtr_zOBypLength;
                 } T32mtr_zOBypLength;
    ///////////////////////////////////////////////////////////

    typedef union  Tmtr_zOActive
          { UNSG32 u32[1];
            struct {
            struct w32mtr_zOActive;
                   };
                 } Tmtr_zOActive;
    typedef union  Tmtr_zOBypLength
          { UNSG32 u32[1];
            struct {
            struct w32mtr_zOBypLength;
                   };
                 } Tmtr_zOBypLength;

    ///////////////////////////////////////////////////////////
     SIGN32 mtr_drvrd(SIE_mtr *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 mtr_drvwr(SIE_mtr *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void mtr_reset(SIE_mtr *p);
     SIGN32 mtr_cmp  (SIE_mtr *p, SIE_mtr *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define mtr_check(p,pie,pfx,hLOG) mtr_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define mtr_print(p,    pfx,hLOG) mtr_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif
//////
/// ENDOFINTERFACE: mtr
////////////////////////////////////////////////////////////



#ifdef __cplusplus
  }
#endif
#pragma  pack()

#endif
//////
/// ENDOFFILE: MTR.h
////////////////////////////////////////////////////////////

#ifndef _THINVPP_H_
#define _THINVPP_H_

#include "thinvpp_module.h"

#ifdef __cplusplus
extern "C" {
#endif


void THINVPP_CPCB_ISR_service(THINVPP_OBJ *vpp_obj, int cpcbID);
void THINVPP_Enable_ISR_Interrupt(THINVPP_OBJ *vpp_obj, int cpcbID, int flag);

#ifdef __cplusplus
}
#endif

#endif

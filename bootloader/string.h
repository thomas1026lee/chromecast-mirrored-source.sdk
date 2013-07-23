#ifndef _LINUX_STRING_H_
#define _LINUX_STRING_H_

#include "com_type.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __HAVE_ARCH_STRCPY
extern char * strcpy(char * dest,const char *src);
#endif
#ifndef __HAVE_ARCH_STRNCPY
extern char * strncpy(char * dest,const char *src, size_t count);
#endif
#ifndef __HAVE_ARCH_STRCAT
extern char * strcat(char * dest, const char *src);
#endif
#ifndef __HAVE_ARCH_STRNCAT
extern char * strncat(char * dest, const char * src, size_t count);
#endif
#ifndef __HAVE_ARCH_STRCMP
extern int strcmp(const char * cs ,const char * ct);
#endif
#ifndef __HAVE_ARCH_STRNCMP
extern int strncmp(const char * cs,const char * ct,size_t count);
#endif
#ifndef __HAVE_ARCH_STRCHR
extern char * strchr(const char * s ,int c);
#endif
#ifndef __HAVE_ARCH_STRRCHR
extern char * strrchr(const char * s,int c);
#endif
#ifndef __HAVE_ARCH_STRSTR
extern char * strstr(const char * s1,const char * s2);
#endif
#ifndef __HAVE_ARCH_STRLEN
extern size_t strlen(const char * s);
#endif
#ifndef __HAVE_ARCH_STRNLEN
extern size_t strnlen(const char * s,size_t count);
#endif
#ifndef __HAVE_ARCH_STRDUP
extern char * strdup(const char * s);
#endif
#ifndef __HAVE_ARCH_STRSWAB
extern char * strswab(const char * s);
#endif

#ifndef __HAVE_ARCH_MEMSET
extern void * memset(void * s,int c,size_t count);
#endif
#ifndef __HAVE_ARCH_MEMCPY
extern void * memcpy(void * dest,const void * src ,size_t count);
#endif
#ifndef __HAVE_ARCH_MEMMOVE
extern void * memmove(void * dest,const void * src,size_t count);
#endif
#ifndef __HAVE_ARCH_MEMSCAN
extern void * memscan(void * addr,int c ,size_t size);
#endif
#ifndef __HAVE_ARCH_MEMCMP
extern int memcmp(const void * cs,const void * ct,size_t count);
#endif
#ifndef __HAVE_ARCH_MEMCHR
extern void * memchr(const void * s,int c,size_t n);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _LINUX_STRING_H_ */

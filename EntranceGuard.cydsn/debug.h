//********************************************************************************************************
// File name		: debug.h
// Description 		: debug.c的头文件
// Author			: Keno
// Date				: 2016.8.8
// Version			: V02
//********************************************************************************************************
#if !defined(__DEBUG_H__)
#define __DEBUG_H__

//******************************************** 头文件声明 **********************************************//
#include <project.h>
#include "stdio.h"



//********************************************* 函数声明 ***********************************************//
#if defined(__ARMCC_VERSION)
    
/* For MDK/RVDS compiler revise fputc function for printf functionality */
int fputc(int ch, FILE *file);

#elif defined (__ICCARM__)      /* IAR */

/* For IAR compiler revise __write() function for printf functionality */
size_t __write(int handle, const unsigned char * buffer, size_t size);

#else  /* (__GNUC__)  GCC */

/* For GCC compiler revise _write() function for printf functionality */
int _write(int file, char *ptr, int len);

#endif  /* (__ARMCC_VERSION) */


void Set32ByPtr(uint8 ptr[], uint32 value);
void DBG_SOFT_PutStr(const char8 string[]);
void DBG_SOFT_PutArrHex(const uint8 array[], uint32 byteCount, uint8 alin);
void DBG_SOFT_PrintfHex(const uint8 array[], uint32 byteCount, uint8 alin);


#endif

//******************************************** END OF FILE *********************************************//

//********************************************************************************************************
// File name		: debug.c
// Description		: 打印相关的函数
// Author			: Keno
// Date				: 2016.8.8
// Version			: V02
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
#include "project.h"
#include "common.h"



#if defined(__ARMCC_VERSION)
    
/* For MDK/RVDS compiler revise fputc function for printf functionality */
struct __FILE 
{
    int handle;  
};

enum 
{
    STDIN_HANDLE,
    STDOUT_HANDLE,
    STDERR_HANDLE
};

FILE __stdin = {STDIN_HANDLE};
FILE __stdout = {STDOUT_HANDLE};
FILE __stderr = {STDERR_HANDLE};

int fputc(int ch, FILE *file) 
{
    int ret = EOF;

    switch( file->handle )
    {
        case STDOUT_HANDLE:
            FP_UART_UartPutChar(ch);
            ret = ch ;
            break ;

        case STDERR_HANDLE:
            ret = ch ;
            break ;

        default:
            file = file;
            break ;
    }
    return ret ;
}

#elif defined (__ICCARM__)      /* IAR */

/* For IAR compiler revise __write() function for printf functionality */
size_t __write(int handle, const unsigned char * buffer, size_t size)
{
    size_t nChars = 0;

    if (buffer == 0)
    {
        /*
         * This means that we should flush internal buffers.  Since we
         * don't we just return.  (Remember, "handle" == -1 means that all
         * handles should be flushed.)
         */
        return (0);
    }

    for (/* Empty */; size != 0; --size)
    {
        UART_DEB_PutChar(*buffer++);
        ++nChars;
    }

    return (nChars);
}

#else  /* (__GNUC__)  GCC */

/* For GCC compiler revise _write() function for printf functionality */
int _write(int file, char *ptr, int len)
{
    int i;
    file = file;
    for (i = 0; i < len; i++)
    {	
	    UART_433_UartPutChar(*ptr++);
	}
    return len;
}


#endif  /* (__ARMCC_VERSION) */   

#if 0
//********************************************************************************************************
// Function name	: Set32ByPtr
// Description		: 将32bit的数据转化为4个8bit的数据输出
// Parameter		: ptr
//					: value
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void Set32ByPtr(uint8 ptr[], uint32 value)
{
    ptr[0u] = (uint8) value;
    ptr[1u] = (uint8) (value >> 8u);    
    ptr[2u] = (uint8) (value >> 16u);    
    ptr[3u] = (uint8) (value >> 24u);
}
#endif

#if DBG_SOFT_ENNABLE
//********************************************************************************************************
// Function name	: DBG_SOFT_PutStr
// Description		: 打印字符串
// Parameter		: string
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void DBG_SOFT_PutStr(const char8 string[])
{
	#if  DBG_SOFT_ENNABLE
	DBG_SOFT_PutString(string);
	#endif
}


//********************************************************************************************************
// Function name	: DBG_SOFT_PutArrHex
// Description		: 打印指定长度的16进制数据
// Parameter		: array
//					: byteCount
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void DBG_SOFT_PutArrHex(const uint8 array[], uint32 byteCount, uint8 alin)
{
	uint32 arrayIndex = 0;
volatile uint8 tmp = alin;
	
	while (arrayIndex < byteCount)
	{
		DBG_SOFT_PutHexByte(array[arrayIndex]);
		DBG_SOFT_PutStr(" ");
		arrayIndex++;
		if(arrayIndex % tmp == 0)
			DBG_SOFT_PutStr("\n");
	}
	DBG_SOFT_PutStr("\n");
}


//********************************************************************************************************
// Function name	: DBG_SOFT_PrintfHex
// Description		: 打印指定长度的16进制数据
// Parameter		: array
//					: byteCount
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void DBG_SOFT_PrintfHex(const uint8 array[], uint32 byteCount, uint8 alin)
{
	#if  DBG_SOFT_ENNABLE
	DBG_SOFT_PutArrHex(array, byteCount, alin);
	#endif
}

#endif

//******************************************** END OF FILE *********************************************//

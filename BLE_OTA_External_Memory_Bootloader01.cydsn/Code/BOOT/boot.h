//********************************************************************************************************
// File name		: boot.h
// Description		: boot.c的头文件
// Author			: Keno
// Date				: 2016.8.8
// Version			: V02
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
#if !defined(__boot_H__)
#define __boot_H__

//********************************************** 头文件 ************************************************//
#include "cytypes.h"
#include "w25qxx.h"
#include "debug.h"
	

//********************************************** 宏定义 ************************************************//
#define MAX_BUFFER_SIZE 		768

/* File is not accessable */
#define CYRET_ERR_FILE          0x01
/* Reached the end of the file */
#define CYRET_ERR_EOF           0x02
/* The amount of data available is outside the expected range */
#define CYRET_ERR_LENGTH        0x03
/* The data is not of the proper form */
#define CYRET_ERR_DATA          0x04
/* The command is not recognized */
#define CYRET_ERR_CMD           0x05
/* The expected device does not match the detected device */
#define CYRET_ERR_DEVICE        0x06
/* The bootloader version detected is not supported */
#define CYRET_ERR_VERSION       0x07
/* The checksum does not match the expected value */
#define CYRET_ERR_CHECKSUM      0x08
/* The flash array is not valid */
#define CYRET_ERR_ARRAY         0x09
/* The flash row is not valid */
#define CYRET_ERR_ROW           0x0A
/* The bootloader is not ready to process data */
#define CYRET_ERR_BTLDR         0x0B
/* The application is currently marked as active */
#define CYRET_ERR_ACTIVE        0x0C
/* An unknown error occured */
#define CYRET_ERR_UNK           0x0F
/* The operation was aborted */
#define CYRET_ABORT             0xFF

/* The communications object reported an error */
#define CYRET_ERR_COMM_MASK     0x2000
/* The bootloader reported an error */
#define CYRET_ERR_BTLDR_MASK    0x4000



//******************************************* 数据位置定义 *********************************************//
#define W25X_START_ADDR					(0x0)	

#define W25X_IMAGE_CURVER_ADDR			(0x32000)						// 200K的位置					//
#define W25X_IMAGE_NEWVER_ADDR			(0x32000 + W25X_PAGE_SIZE)
#define W25X_IMAGE_SIZE_ADDR			(W25X_IMAGE_NEWVER_ADDR + 0x01)	
#define SIZE_OF_W25X_IMAGE_SIZE_ADDR	(4)								//高位在前的放置方式			//

//主要用到的地址
#define W25X_IMAGE_METDATA_ADDR			(0x32000)						// OTA的起始地址，200K位置		//
#define W25X_IMAGESTART_ADDR			(0x32000 + W25X_PAGE_SIZE)
#define W25X_APP_ABS_ADDR(row)          (W25X_IMAGESTART_ADDR + ((row) * CY_FLASH_SIZEOF_ROW))
	
#define	ImageHeadLen					(0x000c)						// 文件头长度					//
#define	RowdataLen						(0x020d)						// 一行的长度525				//

#define	RowDataOffsetAddr				(0x000c)						// 数据区的便宜地址				//

	

//********************************************* 函数声明 ***********************************************//
int  UpdateImage(void);
void LaunchApp(void);


#endif

//******************************************** END OF FILE *********************************************//

//********************************************************************************************************
// File name		: boot.c
// Description		: 启动引导相关函数
// Author			: Keno
// Date				: 2016.8.8
// Version			: V02
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
#include "boot.h "



const uint8 SILICONID[4] = {0x13, 0x27, 0x11, 0xA3};
//********************************************************************************************************
// Function name	: LRC_CheckSum
// Description		: LRC校验算法
// Parameter		: data	: 数据
//					: len	: 长度
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 传入的data数据为未经过拼接的字符串数据.
//********************************************************************************************************
uint8 LRC_CheckSum(uint8 * data, uint16 len)
{
	uint16 i, sum = 0;
	uint8 LrcData[530];
	uint8 ret;
	
	for (i = 0; i < len; i++)
	{
		if (data[i] > 0x40)
		{
			LrcData[i] = data[i] - 0x41 + 10;
		}
		else
		{
			LrcData[i] = data[i] - 0x30;
		}
	}
	
	for (i = 0; i < len / 2; i++)
	{
		sum += (uint16)((LrcData[2 * i] << 4) + LrcData[2 * i + 1]);
	}
	ret = sum & 0xFF;
	ret = 256 - ret;
	return ret;
}

//********************************************************************************************************
// Function name	: CyBtldr_FromAscii
// Description		: 字符串转回16进制
// Parameter		: value
// Return			: CYRET_SUCCESS
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
int CyBtldr_CharToHex(char value)
{
    if (('0' <= value) && (value <= '9'))
        return (uint8)(value - '0');
    if (('a' <= value) && (value <= 'f'))
        return (uint8)(10 + value - 'a');
    if (('A' <= value) && (value <= 'F'))
        return (uint8)(10 + value - 'A');
	
    return CYRET_SUCCESS;
}

//********************************************************************************************************
// Function name	: CyBtldr_AsciiToHex
// Description		: 把2个字符合并成一个hex数据输出
// Parameter		: input_len		: 输入的字符串长度
//					: input_buf		: 输入的字符内容
//					: output_len	: 输出的数据长度
//					: output_buf	: 输出的数据内容
// Return			: void
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
int CyBtldr_AsciiToHex(uint32 input_len, uint8* input_buf, uint32* output_len, uint8* output_buf)
{
    uint16 i;
    int err = CYRET_SUCCESS;

    if (input_len & 1)												// 确保输入的数据个数为偶数			//
	{
        err = CYRET_ERR_LENGTH;
		return err;
    }
	
    for (i = 0; i < input_len / 2; i++)
    {
		output_buf[i] = (CyBtldr_CharToHex(input_buf[i * 2]) << 4) | CyBtldr_CharToHex(input_buf[i * 2 + 1]);
    }
    *output_len = i;
	
    return err;
}

//********************************************************************************************************
// Function name	: CyBtldr_ParseHeader
// Description		: 解析cyacd文件中的头信息
// Parameter		: bufSize	: 输入的字符串大小
//					  buf		: 传入字符串数据
//					  siliconId	: 器件号
//					  siliconRev: 器件版本
//					  chksum	: 器件检验方式
// Return			: 结果
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
int CyBtldr_ParseHeader(uint32 bufSize, uint8* buf, uint32* siliconId, uint8* siliconRev, uint8* chksum)
{
	uint32 out_len;
	uint8  out_data[MAX_BUFFER_SIZE];
	
	int err = CyBtldr_AsciiToHex(bufSize, buf, &out_len, out_data);

	if (CYRET_SUCCESS == err)
	{
		*chksum = out_data[5];
		*siliconId = (out_data[0] << 24) | (out_data[1] << 16) | (out_data[2] << 8) | (out_data[3]);
	    *siliconRev = out_data[4];
	}
	
	return err;
}

//********************************************************************************************************
// Function name	: CyBtldr_ParseRowData
// Description		: 解析cyacd文件中的一行ROW数据
// Parameter		: bufSize	: 输入的字符串大小
//					  buf		: 传入字符串数据
//					  ArrayId	: 输出的组号 
//					  RowNum	: 输出的行编号
//					  DataLen	: 输出的hex数据长度
//					  Data		: 输出的hex数据
//					  chksum	: 输出的校验和
// Return			: 结果
//--------------------------------------------------------------------------------------------------------
// Info				: 1-array, 2-rowNum, 2-size, N-data, 1-checksum
//--------------------------------------------------------------------------------------------------------
// 一个Array（阵列）总计有200行，故算RowNum时，需将Array值*0x200.
//********************************************************************************************************
int CyBtldr_ParseRowData(uint32 bufSize, uint8* buf, uint8* ArrayId, uint16* RowNum, uint16* DataLen, uint8* Data, uint8* chksum)
{
	int err = CYRET_SUCCESS;
    uint32 hexSize;
    uint8  hexData[MAX_BUFFER_SIZE];
    
    if (bufSize <= 6)
	{
        err = CYRET_ERR_LENGTH;
    }
	else if (buf[0] == ':')
    {
		err = CyBtldr_AsciiToHex(bufSize - 1, &buf[1], &hexSize, hexData);
		*ArrayId =   hexData[0];
		*RowNum  = ((hexData[1] << 8) | (hexData[2])) + 0x200 * hexData[0];
		*DataLen =  (hexData[3] << 8) | (hexData[4]);
		*chksum  =  (hexData[hexSize - 1]);
		
		if ((*DataLen + 6) == hexSize)
		{
			memcpy(Data, hexData + 5, *DataLen);
		}
		else
		{
			err = CYRET_ERR_DATA;
		}
	}
    else
	{
        err = CYRET_ERR_CMD;
	}
	
    return err;
}

//********************************************************************************************************
// Function name	: UpdateImage
// Description		: 拷贝启动镜像到BLE芯片FLASH中，并启动
// Parameter		: none
// Return			: 结果
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
int UpdateImage(void)
{
	uint8  err = CYRET_SUCCESS;
	uint32 siliconID;
	uint8  siliconID_Buf[4];
	uint8  siliconRev;
	uint8  ChkSumType;
	
	uint8  RowData[530] = {0};
	uint8  arrayId; 
	uint16 rowNum;
	uint16 dataLen;
	uint8  data[256] = {0};
	uint8  checksum;
	
	uint32 imageSize;
	uint8  imageTmp[4];
	uint32 imageRowQty;
	uint16 i;
	uint8  CountChkSum;
	
	//==== 验证Image的头是否正确 =======================================================================//
	W25QXX_Read(RowData, W25X_IMAGESTART_ADDR, ImageHeadLen);		// 读取Image的头					//
	err = CyBtldr_ParseHeader(ImageHeadLen, RowData, &siliconID, &siliconRev, &ChkSumType);
	if (err != CYRET_SUCCESS)
	{
		DBG_PRINTF("\nUpdateImage-->CyBtldr_ParseHeader Error = 0x%02x\n", err);
		return FALSE;
	}
	else
	{
		siliconID_Buf[0] = (uint8)(siliconID >> 24);
		siliconID_Buf[1] = (uint8)(siliconID >> 16);
		siliconID_Buf[2] = (uint8)(siliconID >> 8);
		siliconID_Buf[3] = (uint8)(siliconID >> 0);
		if (memcmp(siliconID_Buf, SILICONID, 4) != 0)
		{
			DBG_PRINTF("\nUpdateImage-->SiliconID Error.\n");
			return FALSE;
		}
		if (ChkSumType != 0)
		{
			DBG_PRINTF("\nUpdateImage-->CheckSum Type Error.\n");
			return FALSE;
		}
	}

	//==== 验证Image的数据区内容是否正确 ===============================================================//
	W25QXX_Read(imageTmp, W25X_IMAGE_METDATA_ADDR, 4);				// 读取Image的总大小				//
	//imageSize = (imageTmp[0] << 8) + imageTmp[1];
	imageSize = (imageTmp[0] << 8) + imageTmp[1] + (imageTmp[2] << 24) + (imageTmp[3] << 16);
	imageRowQty = (imageSize - ImageHeadLen) / RowdataLen;
	for (i = 0; i < imageRowQty; i++)
	{
		DBG_PRINTF("UpdateImage-->i=0x%x\n", i);
		
		W25QXX_Read(RowData, (W25X_IMAGESTART_ADDR + RowDataOffsetAddr + i * RowdataLen), RowdataLen);
		
		CountChkSum = LRC_CheckSum(RowData + 1, RowdataLen - 3);	// 计算校验和						//
		DBG_PRINTF("UpdateImage-->ChkSum  = 0x%02x\n", CountChkSum);
		
		err = CyBtldr_ParseRowData(RowdataLen, RowData, &arrayId, &rowNum, &dataLen, data, &checksum);
		//DBG_PRINT_TEXT("UpdateImage-->RowData:\n");
		//DBG_SOFT_PrintfHex(RowData, RowdataLen);
		DBG_PRINTF("UpdateImage-->ArrayId = 0x%02x\n", arrayId);
		DBG_PRINTF("UpdateImage-->RowNum  = 0x%04x\n", rowNum);
		DBG_PRINTF("UpdateImage-->DataLen = 0x%04x\n", dataLen);
		DBG_PRINTF("UpdateImage-->CheckSum= 0x%02x\n", checksum);
		//DBG_PRINTF("UpdateImage-->Data:\n");
		//DBG_SOFT_PrintfHex(data, 256);
		
		if (err != CYRET_SUCCESS)
		{
			DBG_PRINTF("\nUpdateImage-->CyBtldr_ParseRowData Error1 = 0x%02x\n", err);
			return FALSE;
		}
		else
		{
			if (CountChkSum != checksum)
			{
				DBG_PRINTF("\nUpdateImage-->Check Sum Error.\n");
				return FALSE;
			}
		}
	}
	
	//==== 将Image的数据写入片内Flash中 ================================================================//
	DBG_PRINTF("\nUpdateImage-->Update Flash Data Start!\n");
	for (i = 0; i < imageRowQty; i++)
	{
		W25QXX_Read(RowData, (W25X_IMAGESTART_ADDR + RowDataOffsetAddr + i * RowdataLen), RowdataLen);
		err = CyBtldr_ParseRowData(RowdataLen, RowData, &arrayId, &rowNum, &dataLen, data, &checksum);
		if (err != CYRET_SUCCESS)
		{
			DBG_PRINTF("\nUpdateImage-->CyBtldr_ParseRowData Error2 = 0x%02x\n", err);
			return FALSE;
		}
		else
		{
			err = (CYRET_SUCCESS != CySysFlashWriteRow((uint32)rowNum, data)) ? Bootloader_ERR_ROW : CYRET_SUCCESS;
			if (err != CYRET_SUCCESS)
			{
				DBG_PRINTF("\nCySysFlashWriteRow Err = 0x%02x\n", err);
				return FALSE;
			}
		}
	}
 	DBG_PRINTF("\nUpdateImage-->Update Flash Data Finish!\n");
	
	//==== Image升级成功，置位开始重启 ================================================================//
	Bootloader_SET_RUN_TYPE(Bootloader_SCHEDULE_BTLDB);
    CySoftwareReset();
	
	return TRUE;
}

//********************************************************************************************************
// Function name	: LaunchApp
// Description		: 启动Bootloadable程序，即APP
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void LaunchApp(void)
{
	Bootloader_GetMetadata(Bootloader_GET_BTLDB_ADDR, 0);
    __asm volatile("    BX  R0\n");
}

//******************************************** END OF FILE *********************************************//

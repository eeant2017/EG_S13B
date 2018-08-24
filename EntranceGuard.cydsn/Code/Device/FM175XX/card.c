//********************************************************************************************************
// File name		: card.c
// Description		: 刷卡的相关函数
// Author			: Keno
// Date				: 2016.8.8
// Version			: V02
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
#include "card.h"
#include <stdint.h>
#include <string.h>
#include "common.h"
#include "fm175xx.h"
#include "user_data.h"

#include "type_a.h"
#include "type_b.h"
#include "mifare_card.h"
#include "crypt.h"


//******************************************* 外部函数声明 *********************************************//
extern void ble_heartbeat(void);
extern uint32 mem_var;

//********************************************************************************************************
// Function name	: detect_nfc_card
// Description		: 检测是否有卡
// Parameter		: card_id	: 返回卡的UID
//					: sz		: UID的长度
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 专属读取eja IC卡
//********************************************************************************************************
#if card_scan 
uint8 detect_nfc_card(uint8 *card_id, uint16 sz)
{
	if (CardRead(card_id) == 1)
	{
		return RET_OK;
	}
	if(1 == nfc_detect_type_b(card_id, 8))
	{
		return RET_OK;
	}
	return RET_ERR;
}
#else	//备份
uint8 detect_nfc_card(uint8 *card_id, uint16 sz)
{
	if (nfc_detect_type_a(card_id, sz) != 0)
	{
		return RET_OK;
	}
	if (nfc_detect_type_b(card_id, sz) != 0)
	{
		return RET_OK;
	}
	return RET_ERR;
}
#endif


//********************************************************************************************************
// Function name	: detect_nfc_card_continus
// Description		: 循环打开40次
// Parameter		: card_id	: 返回卡的UID
//					: sz		: UID的长度
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint8 detect_nfc_card_continus(uint8 *card_id, uint16 sz)
{
	uint32 loop = 0;
	for (loop = 0; loop < 40; loop++)
	{
		if (detect_nfc_card(card_id, sz) == RET_OK)
		{
			DBG_PRINTF("detect_nfc_card_continus--->uuid:\n");
			DBG_SOFT_PrintfHex(card_id, 8,16);
			return RET_OK;
		}
		ble_heartbeat();
	}
	return RET_ERR;
}


#if 0
//********************************************************************************************************
// Function name	: detect_nfc_card_a
// Description		: 检测是否有A卡
// Parameter		: card_id	: 返回卡的UID
//					: sz		: UID的长度
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint8 Read_nfc_cardbyone_a(uint8 *card_id)
{
	uint8 card_id_a[CARD_UID_LEN] = {0};
	//一次读取
	if ((nfc_detect_type_a((uint8 *)card_id_a, CARD_UID_LEN)) != 0)
	{
	// 滤除不准确的卡ID
		if((card_id_a[0] == 0) && (card_id_a[1] == 0) &&(card_id_a[2] == 0) && (card_id_a[3] == 0))
	{
		return RET_ERR;
	}
		memcpy(card_id,card_id_a,CARD_UID_LEN);
	return RET_OK;
	}
}

//********************************************************************************************************
// Function name	: detect_nfc_card_a
// Description		: 检测是否有B卡--身份证
// Parameter		: card_id	: 返回卡的UID
//					: sz		: UID的长度
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint8 Read_nfc_cardbyone_b(uint8 *card_id)
{

	//一次读取
	if ((nfc_detect_type_b((uint8 *)card_id, CARD_UID_LEN)) != 0)
	{
		// 滤除不准确的卡ID
		if((card_id[0] == 0) && (card_id[1] == 0) &&(card_id[2] == 0) && (card_id[3] == 0))
		{
			return RET_ERR;
		}
		#if 1
		// 滤除不准确的卡ID
		if((card_id[4] == 0) && (card_id[5] == 0) &&(card_id[6] == 0) && (card_id[7] == 0))
		{
			return RET_ERR;
		}
		#endif
		return RET_OK;
	}
}
#endif

#if 0
//********************************************************************************************************
// Function name	: Read_nfc_card2
// Description		: 检测是否有卡
// Parameter		: card_id	: 返回卡的UID
//					: sz		: UID的长度
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: a卡天线读取两遍，b卡天线读两遍
//********************************************************************************************************
uint8 Read_nfc_card2(uint8 *card_id)
{
	uint8 card_id0[8] = {0};
	uint8 card_id1[8] = {0};
	
	uint8 card_id2[8] = {0};
	uint8 card_id3[8] = {0};
	
	// 读取a卡
	if(RET_OK == Read_nfc_cardbyone_a(card_id0))
	{
		if(RET_OK == Read_nfc_cardbyone_a(card_id1))
		{
			//只是读出三个不含4个0的数据
			if(0 == memcmp(card_id0, card_id1,8))
			{
				{
					memcpy(card_id,card_id0,8);
					return RET_OK;
				}
			}
			else
			{	//不一样 丢弃
				return RET_ERR;
			}
		}
	}	
	
	//读取b卡
	if(RET_OK == Read_nfc_cardbyone_b(card_id2))
	{
		if(RET_OK == Read_nfc_cardbyone_b(card_id3))
		{
			//只是读出三个不含4个0的数据
			if(0 == memcmp(card_id2, card_id3,8))
			{
				{
					memcpy(card_id,card_id2,8);
					return RET_OK;
				}
			}
			else
			{	//不一样 丢弃
				return RET_ERR;
			}
		}
	}	
	return RET_ERR;		
}


int CardScan_b(uint8 card_id[])
{
	uint8_t statues;
    uint32_t rec_len;
    uint8_t cardFlag = 0;
    uint8_t pupi[4];
    uint8_t buff[12];
	
	statues = TypeB_WUP(&rec_len,buff,pupi);                            /* 寻卡                         */
    if ( statues == TRUE )
    {
        statues = TypeB_Select(pupi,&rec_len,buff);
    }
	
    if ( statues == TRUE )
    {
        cardFlag = 1;
        statues = TypeB_GetUID(&rec_len,&buff[0]);
        if(statues == TRUE)
        {
            memcpy(card_id, buff, 8);
        }
    }	
	return cardFlag;
}
#endif

//======================================================================================================//

//********************************************************************************************************
// Function name	: CardBlock_Wr
// Description		: 写入卡指定扇区的数据
// Parameter		: block_addr	: 卡的扇区号
// 					: pdata			: 数据
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void CardBlock_Wr(uint8 block_addr, uint8 *pdata)
{
	uint8 statues;
	uint8 blockAddr;
	uint8 key[6]= {0xff,0xff,0xfe,0xef,0xfa,0xaf};   
	uint8 blockData[16];
	uint8 i;
	
	#if 0
	FM175X_SoftReset( );
	Set_Rf( 3 );                                                /* 打开双天线                  */
	Pcd_ConfigISOType( 0 );                                     /* ISO14443寄存器初始化        */
	statues = TypeA_CardActive( picc_atqa,picc_uid,picc_sak );  /* 激活A卡片                   */
	if ( statues == TRUE ) 
	{
		DBG_SOFT_PutStr("\nCardUID:");
		DBG_SOFT_PrintfHex(picc_uid,8,16);

	}
	else 
	{
		//DBG_SOFT_PutStr("No Card!\r\n");
		TypeA_Halt( 0 );  	 					 				/* 卡片睡眠 					*/
		return;
	}
	#endif
	
	// 先读0区块
	blockAddr = block_addr;
	statues = Mifare_Auth(0x00,blockAddr>>2,key,picc_uid);      /* 密钥认证                    */
	if(statues  == TRUE)
	{
		DBG_SOFT_PutStr("Auth OK!\r\n");
	}
	else
	{
		TypeA_Halt( 0 );  	 					 				/* 卡片睡眠 					*/
		DBG_SOFT_PutStr("Auth Err!\r\n");
		return ;
	}	
	statues = Mifare_Blockread(blockAddr,blockData);            /* 第一步 读块         		   */
	if(statues  == TRUE)
	{
		DBG_SOFT_PutStr("Read OK!\r\n");
		DBG_SOFT_PrintfHex(blockData,16,16);
	}
	else
	{
		TypeA_Halt( 0 );  	 					 				/* 卡片睡眠 					*/
		DBG_SOFT_PutStr("Read Err!\r\n");
		return ;
	}
	for(i = 0; i < 16; i++)
	{
		blockData[i] = i;
	}
	
	#if 1
	// blockAddr = 0x01;
	statues = Mifare_Blockwrite(blockAddr,pdata);				/* 第二步 写块	 			  */
	if(statues  == TRUE)
	{
		DBG_SOFT_PutStr("Write OK!\r\n");
	}
	else
	{
		TypeA_Halt( 0 );  	 					 				/* 卡片睡眠 					*/
		DBG_SOFT_PutStr("Write Err!\r\n");
		return ;
	}
	#endif
	
	#if 0
	// 写完后 再读出来
	statues = Mifare_Blockread(blockAddr,blockData);            /* 第三步 读块      		  */
	TypeA_Halt( 0 );  	 					 					/* 卡片睡眠 					*/
	if(statues  == TRUE)
	{
		DBG_SOFT_PutStr("Read OK!\r\n");
		DBG_SOFT_PrintfHex(blockData,16,16);
	}
	else
	{
		DBG_SOFT_PutStr("Read Err!\r\n");
		return ;
	}
	#endif
}


//********************************************************************************************************
// Function name	: CardBlock_Rd
// Description		: 读出卡指定扇区的数据
// Parameter		: block_addr	: 卡的扇区号
// 					: pdata			: 数据
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
int CardBlock_Rd(uint8 block_addr, uint8 *pdata)
{
	uint8 statues;
	uint8 blockAddr;
	uint8 key[6]= {0xff,0xff,0xfe,0xef,0xfa,0xaf};   
	uint8 blockData[16];

	
#if 0	
	Set_Rf( 3 );                                                /* 打开双天线                  */
	Pcd_ConfigISOType( 0 );                                     /* ISO14443寄存器初始化        */
	statues = TypeA_CardActive( picc_atqa,picc_uid,picc_sak );  /* 激活A卡片                   */
	if ( statues == TRUE ) 
	{
		DBG_SOFT_PutStr("\nCardBlock_Rd CardUID:");
		DBG_SOFT_PrintfHex(picc_uid,8,16);

	}
	else 
	{
		//DBG_SOFT_PutStr("No Card!\r\n");
		TypeA_Halt( 0 );  	 					 				/* 卡片睡眠 					*/
		return 0;
	}
#endif	

	blockAddr = block_addr; 
	statues = Mifare_Auth(0x00,blockAddr>>2,key,picc_uid);      /* 密钥认证                    */
	if(statues  == TRUE)
	{
		//DBG_SOFT_PutStr("Auth OK!\r\n");
	}
	else
	{
		TypeA_Halt( 0 ); 										/* 卡片睡眠 				   */									
		DBG_SOFT_PutStr("Auth Err!\r\n");     
		return 0;
	}	
	statues = Mifare_Blockread(blockAddr,blockData);            /* 第一步 读块         		   */
	//TypeA_Halt( 0 );  											/* 卡片睡眠 				   */
	if(statues  == TRUE)
	{
		memcpy(pdata,blockData,16);
		//DBG_SOFT_PutStr("Read OK!\r\n");
		//DBG_SOFT_PrintfHex(blockData,16,16);
		return 1;
	}
	else
	{
		DBG_SOFT_PutStr("Read Err!\r\n");     
		return 0;
	}
	
}

//********************************************************************************************************
// Function name	: IsCardLegality
// Description		: 检测卡 合法性
// Parameter		: card_id	: 卡的UID
// Return			: 是否成功
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
int IsCardLegality(uint8 uid[])
{
	uint8 uid_buf1[16] = {0x00,0x00,0x00,0x00,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a};
	uint8 crypt_buf[16] = {0x6f,0x35,0x4a,0x6c,0x58,0xed,0xbd,0xd8,0x19,0x5f,0xd5,0xb8,0x90,0xaf,0xbf,0x38};
	uint8 tmp_buf[16] = {0};
	
	memcpy(uid_buf1, uid, 4);
	do_crypt(crypt_buf, uid_buf1, uid_buf1, 16, 1);
	//DBG_SOFT_PutStr("IsCardLegality -> crypt_data:\n");
	//DBG_SOFT_PrintfHex(uid_buf1,16,16);
	
	CardBlock_Rd(VALID_BLK, tmp_buf);
	
	//DBG_SOFT_PutStr("IsCardLegality -> CardBlock_Rd:\n");
	//DBG_SOFT_PrintfHex(tmp_buf,16,16);
	
	if(0 == memcmp(tmp_buf, uid_buf1,16))
	{
		//DBG_SOFT_PutStr("memcmp ok!\n");
		return 1;
	}
	else
	{
		//DBG_SOFT_PutStr("memcmp err!\n");
		return 0;
	}

}


//********************************************************************************************************
// Function name	: CardInfoUpdate
// Description		: 更新卡的信息
// Parameter		: none
// Return			: 是否成功
//--------------------------------------------------------------------------------------------------------
// Info				: TIMESTAMP_BLK: 时间戳(0-3)+内存变量初值(8-11)+内存变量当前值(12-15)
//********************************************************************************************************
int CardInfoUpdate(void)
{
	uint8 tmp_buf[16] = {0};
	uint32 timer;
	uint32 IC_curvar;

	
	CardBlock_Rd(TIMESTAMP_BLK, tmp_buf);								// 先读出原值
	DBG_SOFT_PutStr("Step1 CardBlock_Rd TIMESTAMP_BLK:\n");
	DBG_SOFT_PrintfHex(tmp_buf,16,16);
	
	if(mem_var == 0)													// 首次刷卡开门
	{		
		memcpy(tmp_buf+8, tmp_buf+12,4);								// 上电复位 备份初始mem变量值
		DBG_SOFT_PutStr("mem_var = 0!\n");
		//1.写入时间戳		
		timer = (uint32)RTC_GetUnixTime();
		LONG2BYTE(timer, tmp_buf);
		
		//2.写入当前内存变量
		mem_var++;
		LONG2BYTE(mem_var, tmp_buf+12);
		
		CardBlock_Wr(TIMESTAMP_BLK, tmp_buf);
		return 1;
	}
	else																// 非首次刷卡开门
	{
		DBG_SOFT_PutStr("mem_var = ");
		DBG_SOFT_PrintfHex((uint8 *)&mem_var,1,16);
		IC_curvar = BYTE2LONG(tmp_buf[12],tmp_buf[13],tmp_buf[14],tmp_buf[15]);
		if(mem_var == IC_curvar)										// 内存变量 = IC变量
		{
			DBG_SOFT_PutStr("mem_var = IC_curvar Branch\n");
			mem_var++;
			IC_curvar = mem_var;
			LONG2BYTE(IC_curvar, tmp_buf+12);
			CardBlock_Wr(TIMESTAMP_BLK, tmp_buf);			
		}
		else if(mem_var == IC_curvar + 1)								// 内存变量 = IC变量 + 1
		{
			DBG_SOFT_PutStr("mem_var = IC_curvar+1 Branch\n");
			LONG2BYTE(mem_var, tmp_buf+12);
			CardBlock_Wr(TIMESTAMP_BLK, tmp_buf);	
		}
		else															// 内存变量没有写入就拿开了 以及其他情况处理
		{
			DBG_SOFT_PutStr("Other Branch\n");
			LONG2BYTE(mem_var, tmp_buf+12);								// 直接先写入mem_var值 后面再读出比对
			CardBlock_Wr(TIMESTAMP_BLK, tmp_buf);	
		}
		
		CardBlock_Rd(TIMESTAMP_BLK, tmp_buf);							// 读出IC内存变量
		IC_curvar = BYTE2LONG(tmp_buf[12],tmp_buf[13],tmp_buf[14],tmp_buf[15]);
			
		DBG_SOFT_PutStr("Step2 CardBlock_Rd TIMESTAMP_BLK:\n");
		DBG_SOFT_PrintfHex(tmp_buf,16,16);
		
		DBG_SOFT_PutStr("RD Back IC_curvar = ");
		DBG_SOFT_PrintfHex((uint8 *)&IC_curvar,1,16);
		DBG_SOFT_PutStr("Cur mem_var = ");
		DBG_SOFT_PrintfHex((uint8 *)&mem_var,1,16);
		
		if(IC_curvar == mem_var)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}	
}

#if 0
//********************************************************************************************************
// Function name	: CardRegister
// Description		: 检测卡 有效性
// Parameter		: none
// Return			: 是否成功
//--------------------------------------------------------------------------------------------------------
// Info				: no use
//********************************************************************************************************
void CardRegister(uint8 uid[], uint8 timedate[])
{
	uint8 statues;
	uint8 uid_buf1[16] = {0x00,0x00,0x00,0x00,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a};
	uint8 crypt_buf[16] = {0x6f,0x35,0x4a,0x6c,0x58,0xed,0xbd,0xd8,0x19,0x5f,0xd5,0xb8,0x90,0xaf,0xbf,0x38};
	uint8 tmp_buf[16] = {0};
	
	uint8 blockcrypt_buf[16] = {0xff,0xff,0xfe,0xef,0xfa,0xaf};					// 数据块加密密码
	
	memcpy(uid_buf1, uid, 4);
	do_crypt(crypt_buf, uid_buf1, uid_buf1, 16, 1);								// 合法性加密1
	do_crypt(blockcrypt_buf, uid_buf1, uid_buf1, 16, 1);						// 块存储加密2
	
	
	#if 1
	FM175X_SoftReset( );
	Set_Rf( 3 );                                                				// 打开双天线                  */
	Pcd_ConfigISOType( 0 );                                     				// ISO14443寄存器初始化        */
	statues = TypeA_CardActive( picc_atqa,picc_uid,picc_sak );  				// 激活A卡片                   */
	if ( statues == TRUE ) 
	{
		DBG_SOFT_PutStr("\nCardUID:");
		DBG_SOFT_PrintfHex(picc_uid,8,16);
	}
	else 
	{
		//DBG_SOFT_PutStr("No Card!\r\n");
		TypeA_Halt( 0 );  	 					 								// 卡片睡眠 					*/
		return;
	}
	#endif
	CardBlock_Wr(VALID_BLK, uid_buf1);											// 存入合法性验证区块 4
	
	//..填入有效期值
	memset(tmp_buf, 0, 16);
	memcpy(tmp_buf+4, timedate, 4);												// 注册只需要写入有效期
	CardBlock_Wr(TIMESTAMP_BLK, tmp_buf);										// 存入有效期验证区块 5
}
#endif

//********************************************************************************************************
// Function name	: CardScan
// Description		: 读出卡指定扇区的数据
// Parameter		: uid: 扫描到卡 读取卡uid
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
int CardScan(uint8 uid[])
{
	uint8 statues = 0;
	//uint8 key[6]= {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};   

	
	FM175X_SoftReset( );  
	Set_Rf( 3 );                                                /* 打开双天线                  */
	Pcd_ConfigISOType( 0 );                                     /* ISO14443寄存器初始化        */
	statues = TypeA_CardActive( picc_atqa,picc_uid,picc_sak );  /* 激活A卡片                   */
	if ( statues == TRUE ) 
	{
		DBG_SOFT_PutStr("\nScaned Card:");	
		DBG_SOFT_PrintfHex(picc_uid,8,16);
	}
	else
	{
		TypeA_Halt( 0 );  	 					 				/* 卡片睡眠 					*/
		//DBG_SOFT_PutStr("\nScaned Card Failed\n");
		return 0;
	}
	
	if(0 == IsCardLegality(picc_uid))							/* 验证合法性					*/
	{
		TypeA_Halt( 0 );  	 					 				/* 卡片睡眠 					*/
		return 0;
	}
	memcpy(uid, picc_uid, 4);									/* 合法性通过 上报UID			*/
	DBG_SOFT_PutStr("\nScaned Card: IsCardLegality 1 OK!\n");
	
	if(1 == CardInfoUpdate())									/* 读写 IC内部信息				*/
	{
		DBG_SOFT_PutStr("CardInfoUpdate OK!\n");
		Set_Rf( 0 );    										/* 关闭双天线					*/
		return 1;
	}
	else
	{
		DBG_SOFT_PutStr("CardInfoUpdate ERR!\n");
		Set_Rf( 0 );    										/* 关闭双天线					*/
		return 0;
	}

}

//********************************************************************************************************
// Function name	: CardRead
// Description		: 录入卡片的UID
// Parameter		: uid: 扫描到卡 读取卡uid
// Return			: 0/1
//--------------------------------------------------------------------------------------------------------
// Info				: 适用于03指令使用
//********************************************************************************************************
int CardRead(uint8 uid[])
{
	uint8 statues = 0;

	
	FM175X_SoftReset( );  
	Set_Rf( 3 );                                                /* 打开双天线                  */
	Pcd_ConfigISOType( 0 );                                     /* ISO14443寄存器初始化        */
	statues = TypeA_CardActive( picc_atqa,picc_uid,picc_sak );  /* 激活A卡片                   */
	if ( statues == TRUE ) 
	{
		DBG_SOFT_PutStr("\nCardRead Card uid:");	
		DBG_SOFT_PrintfHex(picc_uid,8,16);
	}
	else
	{
		TypeA_Halt( 0 );  	 					 				/* 卡片睡眠 					*/
		//DBG_SOFT_PutStr("\nScaned Card Failed\n");
		
		return 0;
	}
	
	if(0 == IsCardLegality(picc_uid))							/* 验证合法性					*/
	{
		TypeA_Halt( 0 );  	 					 				/* 卡片睡眠 					*/
		return 0;
	}
	memcpy(uid, picc_uid, 4);									/* 合法性通过 上报UID			*/
	Set_Rf( 0 );    											/* 关闭双天线					*/
	DBG_SOFT_PutStr("\nCardRead Scaned Card: IsCardLegality 1 OK!\n");
	return 1;
}


//******************************************** END OF FILE *********************************************//

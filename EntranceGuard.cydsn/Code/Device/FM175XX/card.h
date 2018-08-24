//********************************************************************************************************
// File name		: card.h
// Description 		: card.c的头文件
// Author			: Keno
// Date				: 2016.8.8
// Version			: V02
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
#ifndef __CARD_H__
#define __CARD_H__


//******************************************** 头文件声明 **********************************************//
#include <project.h>
#include "fm175xx.h"

#define		card_test			0				// 测试功能
#define		card_scan			1				// 只针对eja IC卡
#define		card_normal			0				// 原正常的双卡功能


//********************************************* 函数声明 ***********************************************//
uint8 detect_nfc_card(uint8 *card_id, uint16 sz);
uint8 detect_nfc_card_continus(uint8 *card_id, uint16 sz);

uint8 Read_nfc_card(uint8 *card_id);
uint8 Read_nfc_card2(uint8 *card_id);

int IsCardLegality(uint8 uid[]);
int CardInfoUpdate(void);
void CardRegister(uint8 uid[], uint8 timedate[]);
int CardScan(uint8 uid[]);
int CardBlock_Rd(uint8 block_addr, uint8 *pdata);
void CardBlock_Wr(uint8 block_addr, uint8 *pdata);
int CardRead(uint8 uid[]);

#endif

//******************************************** END OF FILE *********************************************//

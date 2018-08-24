/****************************************Copyright (c)****************************************************
**                            Guangzhou ZLGMCU Technology Co., LTD
**
**                                 http://www.zlgmcu.com
**
**      广州周立功单片机科技有限公司所提供的所有服务内容旨在协助客户加速产品的研发进度，在服务过程中所提供
**  的任何程序、文档、测试结果、方案、支持等资料和信息，都仅供参考，客户有权不使用或自行参考修改，本公司不
**  提供任何的完整性、可靠性等保证，若在客户使用过程中因任何原因造成的特别的、偶然的或间接的损失，本公司不
**  承担任何责任。
**                                                                        ——广州周立功单片机科技有限公司
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           mifare_card.h
** Last modified Date:  2016-01-04
** Last Version:        V1.00
** Descriptions:        The LPC824 IO function example template
**
**--------------------------------------------------------------------------------------------------------
** Created by:          LinShanwen
** Created date:        2016-01-04
** Version:             V1.00
** Descriptions:        
*/
#ifndef __MIFARE_CARD_H
#define __MIFARE_CARD_H


#include <stdint.h>
extern uint8_t  Mifare_Transfer(uint8_t  block);
extern uint8_t  Mifare_Restore(uint8_t  block);
extern uint8_t  Mifare_Blockset(uint8_t  block,uint8_t  *buff);
extern uint8_t  Mifare_Blockinc(uint8_t  block,uint8_t  *buff);
extern uint8_t  Mifare_Blockdec(uint8_t  block,uint8_t  *buff);
extern uint8_t  Mifare_Blockwrite(uint8_t  block,uint8_t  *buff);
extern uint8_t  Mifare_Blockread(uint8_t  block,uint8_t  *buff);
extern uint8_t  Mifare_Auth(uint8_t  mode,uint8_t  sector,uint8_t  *mifare_key,uint8_t  *card_uid);


#endif //__MIFARE_CARD_H


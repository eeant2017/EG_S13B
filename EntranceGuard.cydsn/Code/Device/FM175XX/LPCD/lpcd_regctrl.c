/*********************************************************************
*                                                                    *
*   Copyright (c) 2010 Shanghai FuDan MicroElectronic Inc, Ltd.      *
*   All rights reserved. Licensed Software Material.                 *
*                                                                    *
*   Unauthorized use, duplication, or distribution is strictly       *
*   prohibited by law.                                               *
*                                                                    *
*********************************************************************/

/*********************************************************************************************************
**     FM175XX��չ�Ĵ����غ���                                            
**     ��Ҫ����:                                                         
**     1.ʵ����չ�Ĵ����Ķ���д���޸ĵ�����                         
**     ����:��ͦ��                                                      
**     ����ʱ��:2014��7��18��                                                                     
*********************************************************************************************************/
#include "lpcd_reg.h"


/*********************************************************************************************************
** Function name:       GetReg_Ext(uint8_t  ExtRegAddr,uint8_t * ExtRegData)
** Descriptions:        ��ȡ��չ�Ĵ���ֵ
** input parameters:    ExtRegAddr:��չ�Ĵ�����ַ   
** output parameters:   ExtRegData:��ȡ��ֵ
** Returned value:      uint8_t   TRUE����ȡ�ɹ�   FALSE:ʧ��
*********************************************************************************************************/
uint8_t  GetReg_Ext(uint8_t  ExtRegAddr,uint8_t * ExtRegData)
{
    uint8_t  res;
    uint8_t  addr,regdata;

    addr = JREG_EXT_REG_ENTRANCE;                                       /* ��չ�Ĵ���0x0f��ַ           */
    regdata = JBIT_EXT_REG_RD_ADDR + ExtRegAddr;                        /* JBIT_EXT_REG_RD_DATAд�������ַ */
                                                                        /* ExtRegAddr ��չ�Ĵ�����ַ    */
    res = spi_SetReg(addr,regdata);                                     /* д����չ�Ĵ�����ַ           */
    if (res == FALSE) 
        return FALSE;

    addr = JREG_EXT_REG_ENTRANCE;                                       /* ��չ�Ĵ���0x0f��ַ           */
    res = spi_GetReg2(addr,&regdata);                                   /* ������չ�Ĵ�������           */
    if (res == FALSE) 
        return FALSE;
    *ExtRegData = regdata;

    return TRUE;    
}


/*********************************************************************************************************
** Function name:       SetReg_Ext(uint8_t  ExtRegAddr,uint8_t * ExtRegData)
** Descriptions:        д��չ�Ĵ���
** input parameters:    ExtRegAddr:��չ�Ĵ�����ַ   
** output parameters:   ExtRegData:Ҫд���ֵ
** Returned value:      uint8_t   TRUE����ȡ�ɹ�   FALSE:ʧ��
*********************************************************************************************************/
uint8_t  SetReg_Ext(uint8_t  ExtRegAddr,uint8_t  ExtRegData)
{
    uint8_t  res;
    uint8_t  addr,regdata;

    addr = JREG_EXT_REG_ENTRANCE;                                       /* ��չ�Ĵ�����ڵ�ַ0x0f       */
    regdata = JBIT_EXT_REG_WR_ADDR + ExtRegAddr;                        /* д����չ�Ĵ�����ַ           */
    res = spi_SetReg(addr,regdata);
    if (res == FALSE) 
        return FALSE;

    addr = JREG_EXT_REG_ENTRANCE;
    regdata = JBIT_EXT_REG_WR_DATA + ExtRegData;                        /* ������չ�Ĵ�������           */
    res = spi_SetReg(addr,regdata);
    if (res == FALSE) 
        return FALSE;
        
    return TRUE;    
}

/*********************************************************************************************************
** Function name:       modifyReg_Ext(uint8_t  ExtRegAddr,uint8_t * mask,uint8_t  set)
** Descriptions:        �Ĵ���λ����
** input parameters:    ExtRegAddr:Ŀ��Ĵ�����ַ   mask:Ҫ�ı��λ  
**                      set:  0:��־��λ����   ����:��־��λ����
** output parameters:  
** Returned value:      uint8_t   TRUE����ȡ�ɹ�   FALSE:ʧ��
*********************************************************************************************************/
uint8_t  ModifyReg_Ext(uint8_t  ExtRegAddr,uint8_t  mask,uint8_t  set)
{
    uint8_t  status;
    uint8_t  regdata;
    
    status = GetReg_Ext(ExtRegAddr,&regdata);
    if(status == TRUE) {
        if(set) {
            regdata |= mask;
            }
        else {
            regdata &= ~(mask);
        }
    }
    else
        return FALSE;

    status = SetReg_Ext(ExtRegAddr,regdata);
    return status;
}



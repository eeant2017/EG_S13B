/****************************************Copyright (c)****************************************************
**                            Guangzhou ZLGMCU Technology Co., LTD
**
**                                 http://www.zlgmcu.com
**
**      ������������Ƭ���Ƽ����޹�˾���ṩ�����з�������ּ��Э���ͻ����ٲ�Ʒ���з����ȣ��ڷ�����������ṩ
**  ���κγ����ĵ������Խ����������֧�ֵ����Ϻ���Ϣ���������ο����ͻ���Ȩ��ʹ�û����вο��޸ģ�����˾��
**  �ṩ�κε������ԡ��ɿ��Եȱ�֤�����ڿͻ�ʹ�ù��������κ�ԭ����ɵ��ر�ġ�żȻ�Ļ��ӵ���ʧ������˾��
**  �е��κ����Ρ�
**                                                                        ����������������Ƭ���Ƽ����޹�˾
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           lpcd_api.c
** Last modified Date:  2016-4-14
** Last Version:        V1.00
** Descriptions:        LPCD������ʾ��ר��API����
**
**--------------------------------------------------------------------------------------------------------
*/
/********************************************************************************************************/
/*     FM175XX LPCD API������                                                                           */
/*     ��Ҫ����:                                                                                        */
/*     1.ʵ��LPCD���API����                                                                            */
/*     ����:��ͦ��                                                                                      */
/*     ����ʱ��:2014��7��18��                                                                           */
/********************************************************************************************************/
/*
** LPCDר��ͷ�ļ�����ֲʱ����Ҫʹ�õ�
*/
#include "lpcd_reg.h"
#include "lpcd_config.h"
#include "lpcd_config_user.h"
#include "lpcd_regctrl.h"
#include "spi.h"
/*
** FM175xxͷ�ļ�����ֲʱ����Ҫ�õ�
*/ 
#include "fm175xx.h"
#include "Utility.h"


extern unsigned char Event_Flag ;          		    //�Ƿ����¼�Ҫ����
extern unsigned char Event_FM175XX_IRQ  ;   		//FM175XX IRQ�Ƿ�����IRQ�������ظ��ж�����
#define COMMIEN_DEF			0x80
#define DIVIEN_DEF			0x80


#define IF_ERR_THEN_RETURN 	if (ret == FALSE) return FALSE
#define ADC_REFERNCE_MIN 	0x0                                            /* ʵ�ʲ�������ֵ��С           */
#define ADC_REFERNCE_MAX 	0x7F                                           /* ʵ�ʵ���ֵ���               */

#define uchar  uint8_t

/*
** �ڲ������б�
*/
uint8_t  WaitForLpcdIrq(uint8_t  IrqType);
uint8_t  ReadLpcdADCResult(uint8_t  *ADCResult);
uint8_t  CalibraReadADCResult(uint8_t  *ADCResult);
uint8_t  LpcdSet_PGA(uint8_t  *GainCalibraFlag, uint8_t  *ADCResult);
uint8_t  LpcdSet_ADCRefvoltage(uint8_t  *CalibraFlag, uint8_t  *ADCResult);
uint8_t  LpcdSet_DetectSensitive(float Sensitive);

/*
** ���̱����б�
*/ 
uint8_t  T3ClkDivK ;
uint8_t  LpcdBiasCurrent;                                               /* 3bit ����Ƭ����ָ�������
                                                                            ��config�ļ����趨          */
uint8_t  LpcdGainReduce;                                                /* 2bit                         */
uint8_t  LpcdGainAmplify;                                               /* 3bit                         */
uint8_t  LpcdADCRefernce;                                               /* 7bit                         */

uint8_t  Timer1Cfg;                                                     /* 4bit                         */
uint8_t  Timer2Cfg;                                                     /* 5bit                         */
uint8_t  Timer3Cfg;                                                     /* 5bit                         */

uint8_t  ADCResultFullScale;                                            /* T3������ADCResult��Ϣ        */
uint8_t  ADCResultThreshold;                                            /* �����ȣ����ó����ֵ       */
uint8_t  LpcdThreshold_L;                                               /* LPCD���ȵ���ֵ               */
uint8_t  LpcdThreshold_H;                                               /* LPCD���ȸ���ֵ               */
uint8_t  ADCResultCenter;                                               /* LPCD�������ĵ�               */
uint8_t  LpcdADCResult[10];                                             /* Lpcd������Ϣ�������󴥷��ж� */


/*********************************************************************************************************
** Function name:       LpcdParamInit()
** Descriptions:        LPCD������ʼ��
** input parameters:    
** output parameters:   
** Returned value:      uint8_t   TRUE����ȡ�ɹ�   FALSE:ʧ��
*********************************************************************************************************/
uint8_t  LpcdParamInit()
{
    LpcdBiasCurrent = LPCD_BIAS_CURRENT ;                               /* ����Ƭ����ָ�����           */
                                                                        /* ��config�ļ����趨           */
    LpcdGainReduce = 0x3;                                               /* 1x                           */
    LpcdGainAmplify = 0x0;                                              /* 1x                           */
    LpcdADCRefernce = 0;
    Timer1Cfg = TIMER1_CFG;                                             /* LPCD������߽׶�ʱ������     */
    Timer2Cfg = TIMER2_CFG;                                             /* LPCD׼����⿨Ƭʱ������     */
    Timer3Cfg = TIMER3_CFG;                                             /* LPCD���׶�ʱ������         */

    if (Timer3Cfg > 0xF)  {                                             /* Timer3Cfg�õ�5bit  16��Ƶ    */
        T3ClkDivK = 2;                                                  /* 16��Ƶ                       */
        ADCResultFullScale =  ((Timer3Cfg - 1)<<3);
        ADCResultCenter = (ADCResultFullScale >>1);
        ADCResultThreshold = (ADCResultFullScale >> LPCD_THRESHOLD_RATIO);
    }
    else if(Timer3Cfg > 0x7) {                                          /* Timer3Cfg�õ�4bit  8��Ƶ     */
        T3ClkDivK = 1;                                                  /* 8��Ƶ                        */
        ADCResultFullScale =  ((Timer3Cfg - 1)<<4);                     /* 160                          */
        ADCResultCenter = (ADCResultFullScale >>1);                     /* 80                           */
        ADCResultThreshold = (ADCResultFullScale >> LPCD_THRESHOLD_RATIO);  /* 10                       */
    }
    else  {
        T3ClkDivK = 0;                                                   /* 4��Ƶ                        */
        ADCResultFullScale =  ((Timer3Cfg - 1)<<5);
        ADCResultCenter = (ADCResultFullScale >>1);
        ADCResultThreshold = (ADCResultFullScale >> LPCD_THRESHOLD_RATIO);
    }

    LpcdThreshold_H = ADCResultCenter + ADCResultThreshold;              /* �ߴ����ż� 80+10=90          */
    LpcdThreshold_L= ADCResultCenter - ADCResultThreshold;               /* �ʹ����ż� 80-10=70          */
    
    return TRUE;
}

/*********************************************************************************************************
** Function name:       LpcdRegisterInit()
** Descriptions:        LPCD�Ĵ�����ʼ��
** input parameters:    
** output parameters:   
** Returned value:      uint8_t   TRUE����ȡ�ɹ�   FALSE:ʧ��
*********************************************************************************************************/
uint8_t  LpcdRegisterInit(void)
{
    uint8_t  ret;
    uint8_t  regdata;
    
    regdata = COMMIEN_DEF;                                              /* �ж����ã������ж�����IRQ���*/
    ret = spi_SetReg(ComIEnReg,regdata);                                /* �ж�IRQ����͵�ƽ            */
    IF_ERR_THEN_RETURN;

    regdata = DIVIEN_DEF;                                               /* ����IRQ������Ϊ��׼CMOS���  */
    ret = spi_SetReg(DivIEnReg,regdata);                                /* STatus1Reg��IRQλ�෴        */
    IF_ERR_THEN_RETURN;       
    
    /*
    ** LpcdCtrl1�Ĵ���
    */ 
    ret = SetReg_Ext(JREG_LPCD_CTRL1,JBIT_BIT_CTRL_CLR+JBIT_LPCD_RSTN); /* ��λLPCD�Ĵ���               */
    IF_ERR_THEN_RETURN;
    ret = SetReg_Ext(JREG_LPCD_CTRL1,JBIT_BIT_CTRL_SET+JBIT_LPCD_RSTN); /* ��λ�ſ�LPCD�Ĵ���           */
    IF_ERR_THEN_RETURN;
    
    ret = SetReg_Ext(JREG_LPCD_CTRL1,JBIT_BIT_CTRL_SET+JBIT_LPCD_EN);   /* ʹ��LPCD����                 */
    IF_ERR_THEN_RETURN;
    
    ret = SetReg_Ext(JREG_LPCD_CTRL1,(LPCD_IE<<5)+JBIT_LPCD_IE);        /* ����LPCD�жϼĴ���״̬       */
    IF_ERR_THEN_RETURN;                                                 /* ��ӳ��IRQ����                */
    
                                                                        /* ���ý���������             */
	/* һ�μ�⵽����Ч             																	*/
    ret = SetReg_Ext(JREG_LPCD_CTRL1,(LPCD_AUTO_DETECT_TIMES<<5)+JBIT_LPCD_CMP_1);
    IF_ERR_THEN_RETURN;

    
    /*
    ** LpcdCtrl2�Ĵ���
    */        
    ret = SetReg_Ext(JREG_LPCD_CTRL2,((LPCD_TX2RFEN<<4)+(LPCD_CWN<<3)+LPCD_CWP)); /* P������������0��7  */
    IF_ERR_THEN_RETURN;                                                 /* һ����������ѡ��3          */
        
    /*
    ** LpcdCtrl3�Ĵ���
    */   
    ret = SetReg_Ext(JREG_LPCD_CTRL3,LPCD_MODE<<3);            			/* û����ʵ������               */
    IF_ERR_THEN_RETURN;

    /*
    ** Timer1Cfg�Ĵ���
    */ 
    ret = SetReg_Ext(JREG_LPCD_T1CFG,(T3ClkDivK<<4)+Timer1Cfg);         /* ����LPCD˯��ʱ��             */
    IF_ERR_THEN_RETURN;
        
    /*
    ** Timer2Cfg�Ĵ���
    */
    ret = SetReg_Ext(JREG_LPCD_T2CFG,Timer2Cfg);                        /* ����LPCD׼�����ʱ��         */ 
    IF_ERR_THEN_RETURN;
        
    /*
    ** Timer3Cfg�Ĵ���
    */ 
    ret = SetReg_Ext(JREG_LPCD_T3CFG,Timer3Cfg);                        /* ����LPCD��⿨Ƭʱ��         */
    IF_ERR_THEN_RETURN;

    /*
    ** VmidBdCfg�Ĵ���
    */
    ret = SetReg_Ext(JREG_LPCD_VMIDBD_CFG,VMID_BG_CFG);                 /* �������û��޸�               */
    IF_ERR_THEN_RETURN;
        
    /*
    ** AutoWupCfg�Ĵ���
    */
    ret = SetReg_Ext(JREG_LPCD_AUTO_WUP_CFG,(AUTO_WUP_EN<<3)+AUTO_WUP_CFG);  /* �����Զ�����ʱ��        */
    IF_ERR_THEN_RETURN;
        
    /*
    ** LpcdThreshold_L1�Ĵ���
    */
    ret = SetReg_Ext(JREG_LPCD_THRESHOLD_MIN_L,(LpcdThreshold_L & 0x3F));  /* ���ÿ�����·�ֵ          */
    IF_ERR_THEN_RETURN;
        
    /*
    ** LpcdThreshold_L2�Ĵ���
    */ 
    ret = SetReg_Ext(JREG_LPCD_THRESHOLD_MIN_H,(LpcdThreshold_L>>6));   
    IF_ERR_THEN_RETURN;
        
    /*
    ** LpcdThreshold_H1�Ĵ���
    */
    ret = SetReg_Ext(JREG_LPCD_THRESHOLD_MAX_L,(LpcdThreshold_H& 0x3F));  /* ���ÿ�����Ϸ�ֵ           */
    IF_ERR_THEN_RETURN;
    /*
    ** LpcdThreshold_H2�Ĵ���
    */
    ret = SetReg_Ext(JREG_LPCD_THRESHOLD_MAX_H,(LpcdThreshold_H>>6));   
    IF_ERR_THEN_RETURN;
    
    /*
    ** Auto_Wup_Cfg�Ĵ���
    */
    ret=SetReg_Ext(JREG_LPCD_AUTO_WUP_CFG,(AUTO_WUP_EN<<3) + AUTO_WUP_CFG );  /* �ٴ������Զ�����ʱ��   */
    IF_ERR_THEN_RETURN;
        
    return TRUE;
}

/*********************************************************************************************************
** Function name:       LpcdSet_DetectSensitive()
** Descriptions:        ���ü��������
** input parameters:    loat Sensitive��������
** output parameters:   
** Returned value:      uint8_t   TRUE����ȡ�ɹ�   FALSE:ʧ��
*********************************************************************************************************/
uint8_t   LpcdSet_DetectSensitive(float Sensitive)
{
    uint8_t  ret,Threshold;
    Threshold =ADCResultCenter*Sensitive; 
    /* ΢����ֵ */
    /****LpcdThreshold_H = ADCResultCenter + ADCResultThreshold;*/
    /****LpcdThreshold_L= ADCResultCenter - ADCResultThreshold;*/
    LpcdThreshold_H = ADCResultCenter + Threshold;
    LpcdThreshold_L = ADCResultCenter - Threshold;
    /*
    ** LpcdThreshold_L1�Ĵ���
    */
    ret = SetReg_Ext(JREG_LPCD_THRESHOLD_MIN_L,(LpcdThreshold_L& 0x3F));
    IF_ERR_THEN_RETURN;
    /*
    ** LpcdThreshold_L2�Ĵ���
    */
    ret = SetReg_Ext(JREG_LPCD_THRESHOLD_MIN_H,(LpcdThreshold_L>>6));
    IF_ERR_THEN_RETURN;
    /*
    ** LpcdThreshold_H1�Ĵ���
    */
    ret = SetReg_Ext(JREG_LPCD_THRESHOLD_MAX_L,(LpcdThreshold_H& 0x3F));
    IF_ERR_THEN_RETURN;
    /*
    ** LpcdThreshold_H2�Ĵ���
    */
    ret = SetReg_Ext(JREG_LPCD_THRESHOLD_MAX_H,(LpcdThreshold_H>>6));
    IF_ERR_THEN_RETURN;
    //DBG_PRINTF("LpcdThreshold_H = %d \r\n",LpcdThreshold_H);
    //DBG_PRINTF("LpcdThreshold_L = %d \r\n",LpcdThreshold_L);
    //DBG_PRINTF("ADCResultCenter = %d \r\n",ADCResultCenter);
    return TRUE;    
}


/*********************************************************************************************************
** Function name:       LpcdSet_ADCRefvoltage()
** Descriptions:        ���ú��ʵ�ADC�ο���ѹ
** input parameters:    uint8_t  *CalibraFlag, uint8_t  *ADCResult   ���ȼ���������ַ
** output parameters:   
** Returned value:      uint8_t   TRUE����ȡ�ɹ�   FALSE:ʧ��
*********************************************************************************************************/
uint8_t   LpcdSet_ADCRefvoltage(uint8_t  *CalibraFlag, uint8_t  *ADCResult)
{
    uint8_t  ret;
    uint8_t  ADCResult_Pre;                                             /* ������Ϣ��ǰһ��ֵ           */
    
    //ɨ�������
    for(LpcdADCRefernce = ADC_REFERNCE_MIN;LpcdADCRefernce < ADC_REFERNCE_MAX;LpcdADCRefernce++)
    //ͨ����ѭ������ȷ�����յĲο���ѹ��ADC���ıȽ�ֵ
    {
        //���òο���ѹֵ
        ret = SetReg_Ext(JREG_LPCD_BIAS_CURRENT, ((LpcdADCRefernce & 0x40) << 5) + (LpcdBiasCurrent & 0x7));
        IF_ERR_THEN_RETURN;
        ret = SetReg_Ext(JREG_LPCD_ADC_REFERECE,LpcdADCRefernce&0x3F);
        IF_ERR_THEN_RETURN;
        //���ݷ�����Ϣ
        ADCResult_Pre = *ADCResult;
        //���̶�ȡ��ǰ������Ϣ
        ret = CalibraReadADCResult(ADCResult);
        IF_ERR_THEN_RETURN;
        /*
              ** �㷨һ
              */
        // ������ȿ�ʼ������ֵС����Ϊ��ʼ������ֵ��
        if (*ADCResult < ADCResultCenter) {
            //���̳ɹ�
            (*CalibraFlag) = TRUE;
            //��ǰһ�����̲ο���ѹ���ж��ĸ����ȸ��ӽ����ĵ�
            if((ADCResultCenter - *ADCResult) < (ADCResult_Pre-ADCResultCenter))
            {
                //ֱ���õ�ǰֵ��Ϊ���ĵ�
                ADCResultCenter = *ADCResult;
            }
            else
            {
                //ֱ���õ�ǰֵ��Ϊ���ĵ�
                ADCResultCenter = ADCResult_Pre;
                //�ο���ѹ����֮ǰ�Ĳο���ѹ
                LpcdADCRefernce--;
                //�������òο���ѹֵ
                ret = SetReg_Ext(JREG_LPCD_BIAS_CURRENT, ((LpcdADCRefernce & 0x40) << 5) + (LpcdBiasCurrent & 0x7));
                IF_ERR_THEN_RETURN;
                ret = SetReg_Ext(JREG_LPCD_ADC_REFERECE, LpcdADCRefernce&0x3F);
                IF_ERR_THEN_RETURN;
            }
            break;
        }
    }
    if(LpcdADCRefernce < ADC_REFERNCE_MAX)
        return TRUE;
    else
        return FALSE;
}

/*********************************************************************************************************
** Function name:       LpcdSet_PGA()
** Descriptions:        ���ú��ʵ�PGA����
** input parameters:    uint8_t  *CalibraFlag, uint8_t  *ADCResult   ���ȼ���������ַ
** output parameters:   
** Returned value:      uint8_t   TRUE����ȡ�ɹ�   FALSE:ʧ��
*********************************************************************************************************/
uint8_t   LpcdSet_PGA(uint8_t  *GainCalibraFlag, uint8_t  *ADCResult)
{
    uint8_t  ret;
    //�ο���ѹֵ������С
    LpcdADCRefernce = ADC_REFERNCE_MIN;
    ret = SetReg_Ext(JREG_LPCD_BIAS_CURRENT, ((LpcdADCRefernce & 0x40) << 5) + (LpcdBiasCurrent & 0x7));
    IF_ERR_THEN_RETURN;
    ret = SetReg_Ext(JREG_LPCD_ADC_REFERECE,LpcdADCRefernce&0x3F);
    IF_ERR_THEN_RETURN;

    //���̶�ȡ��ǰ������Ϣ
    ret = CalibraReadADCResult(ADCResult);
    IF_ERR_THEN_RETURN;
    //ȱʡ���治��Ҫ����
    *GainCalibraFlag = TRUE;
   
    //�ж��Ƿ����̫խ�����̫խlpcd_gain˥��
    if  (*ADCResult < ADCResultCenter)
    {
        //������Ҫ����
        *GainCalibraFlag = FALSE;
        //*GainCalibraFlag = LpcdSetPGA_GainReduce(ADCResult);   //����PGA����˥��
        while(1)
        {
			//�����ǰ�Ѿ�����С���棬����ʧ��
            if (LpcdGainReduce == 0)
            {
                *GainCalibraFlag = FALSE;
                break;
            }
            //����˥��
            LpcdGainReduce --; 
            //DBG_PRINTF("LpcdSet_PGA  LpcdGainReduce=%d !\r\n",LpcdGainReduce);
            //��������
            ret = SetReg_Ext(JREG_LPCD_CTRL4,((LpcdGainAmplify << 2) + LpcdGainReduce));
            IF_ERR_THEN_RETURN;

            //���̶�ȡ��ǰ������Ϣ
            ret = CalibraReadADCResult(ADCResult);
            //DBG_PRINTF("LpcdSet_PGA  ADCResult=%d !\r\n", *ADCResult);
            IF_ERR_THEN_RETURN;
            //���̳ɹ��������ĵ��Ƶ����ĵ��Ҳ�
            if (*ADCResult > ADCResultCenter)
            {
                *GainCalibraFlag = TRUE;
                break;
            }
         }    
    }
    else
    {
        //�ο���ѹֵ�������
        LpcdADCRefernce = ADC_REFERNCE_MAX;
        ret = SetReg_Ext(JREG_LPCD_BIAS_CURRENT, ((LpcdADCRefernce & 0x40) << 5) + (LpcdBiasCurrent & 0x7));
        IF_ERR_THEN_RETURN;
        ret = SetReg_Ext(JREG_LPCD_ADC_REFERECE,LpcdADCRefernce&0x3F);
        IF_ERR_THEN_RETURN;

        //���̶�ȡ��ǰ������Ϣ
        ret = CalibraReadADCResult(ADCResult);
        //DBG_PRINTF("LpcdSet_PGA  ADCResult=%d !\r\n",*ADCResult);
        IF_ERR_THEN_RETURN;

        //���̳ɹ���־��ʼ��
        *GainCalibraFlag = TRUE;
        
        //�ж��Ƿ����̫С�����̫Сlpcd_gain�Ŵ�
        if (*ADCResult > ADCResultCenter)
        {
            //������Ҫ����
            *GainCalibraFlag = FALSE;
            while(1)
            {
                //�����ǰ�Ѿ���������棬����ʧ��
                if (LpcdGainAmplify == 0x7)
                {
                    *GainCalibraFlag = FALSE;
                    break;
                }
                else//�����Ŵ�
                {
                    LpcdGainAmplify++;  
                }
				//��������
                ret = SetReg_Ext(JREG_LPCD_CTRL4,((LpcdGainAmplify << 2) + LpcdGainReduce));
                IF_ERR_THEN_RETURN;

                //���̶�ȡ��ǰ������Ϣ
                  ret = CalibraReadADCResult(ADCResult);
                IF_ERR_THEN_RETURN;
                
                //���̳ɹ��������ĵ��Ƶ����ĵ����
                if (*ADCResult < ADCResultCenter)
                {
                    *GainCalibraFlag = TRUE;
                     break;
                }
            }
        }
    }
    return TRUE;
}

/*********************************************************************************************************
** Function name:       LpcdInitCalibra()
** Descriptions:        ��ʼ������
** input parameters:    uint8_t  *CalibraFlag ���̱�־���Ƿ���Ҫ����
** output parameters:   
** Returned value:      uint8_t   TRUE����ȡ�ɹ�   FALSE:ʧ��
*********************************************************************************************************/
uint8_t   LpcdInitCalibra(uint8_t  *CalibraFlag)
{
	uint8_t  ret;
    uint8_t  ADCResult;                                                 /* LPCD������Ϣ                 */
    uint8_t  GainCalibraFlag;                                           /* ������̽��                 */
                                                                        /* ��������                     */
    ret = SetReg_Ext(JREG_LPCD_CTRL4,((LpcdGainAmplify << 2) + LpcdGainReduce));
    IF_ERR_THEN_RETURN;
                                                                        /* ����ƫ�õ����Ͳο���ѹ       */
    ret = SetReg_Ext(JREG_LPCD_BIAS_CURRENT, ((LpcdADCRefernce & 0x40) << 5) + (LpcdBiasCurrent & 0x7));
    IF_ERR_THEN_RETURN;

    ret = SetReg_Ext(JREG_LPCD_MISC,BFL_JBIT_CALIB_VMID_EN);            /* ������Уģʽ��Vmindʹ��      */
    IF_ERR_THEN_RETURN;

    ret = SetReg_Ext(JREG_LPCD_T1CFG,(T3ClkDivK<<4)+Timer1Cfg);         /* Timer1Cfg����                */
    IF_ERR_THEN_RETURN;


    ret = SetReg_Ext(JREG_LPCD_T2CFG,Timer2Cfg);                        /* Timer2Cfg����                */
    IF_ERR_THEN_RETURN;

    ret = SetReg_Ext(JREG_LPCD_T3CFG,Timer3Cfg);                        /* Timer3Cfg����                */
    IF_ERR_THEN_RETURN;
                                                                          
    ret = LpcdSet_PGA(&GainCalibraFlag,&ADCResult);                     /* PGA��������                  */
    IF_ERR_THEN_RETURN;                                                 /* ��У����PGA�������˥�������趨 */

    if (GainCalibraFlag == FALSE)                                       /* ����������ʧ�ܣ���ʧ��     */
    {
		(*CalibraFlag) = FALSE;
        return ADCResult;                                               /* ����ʧ�ܷ��ط���             */
    }
    //ɨ��ο���ѹֵ���ҵ����ʵĴ�Խ���ĵ������
    (*CalibraFlag) = FALSE;
    GainCalibraFlag = LpcdSet_ADCRefvoltage(CalibraFlag,&ADCResult);    /* ���ú��ʵ�ADC�ο���ѹ        */
    ret = LpcdSet_DetectSensitive(LPCD_DetectSensitive);                /* ��У���̼���������趨       */
    IF_ERR_THEN_RETURN;
    
    if (GainCalibraFlag == FALSE)
    {        
        (*CalibraFlag) = FALSE;
        ret = ModifyReg_Ext(JREG_LPCD_MISC,BFL_JBIT_CALIB_VMID_EN,0);   /* ���� ���̽����ر�CalibVmidEn */
        IF_ERR_THEN_RETURN;
        return ADCResult;                                               /* ����ʧ�ܷ��ط���             */
    }
    
    ret = ModifyReg_Ext(JREG_LPCD_MISC,BFL_JBIT_CALIB_VMID_EN,0);       /* ���̽����ر�CalibVmidEn      */
    IF_ERR_THEN_RETURN;
    return TRUE;
}

/*********************************************************************************************************
** Function name:       WaitForLpcdIrq()
** Descriptions:        �ȴ�LPCD�ж�
** input parameters:    IrqType
** output parameters:   
** Returned value:      uint8_t   TRUE����ȡ�ɹ�   FALSE:ʧ��
*********************************************************************************************************/
uint8_t  WaitForLpcdIrq(uint8_t  IrqType)
{
    uint8_t  ExtRegData;
    uint8_t  ret;
    uint8_t  TimeOutCount;
    
    TimeOutCount = 0;
    ret = GetReg_Ext(JREG_LPCD_IRQ,&ExtRegData);
    if (ret == 0)
    {
        ret = GetReg_Ext(JREG_LPCD_IRQ,&ExtRegData);
        ret = GetReg_Ext(JREG_LPCD_IRQ,&ExtRegData);
        ret = GetReg_Ext(JREG_LPCD_IRQ,&ExtRegData);
        ret =1;
    }
    IF_ERR_THEN_RETURN;
    while ((ExtRegData & IrqType) != IrqType)
    {
        ret = GetReg_Ext(JREG_LPCD_IRQ,&ExtRegData);
        if (ret == FALSE)
        {
            ret = FALSE;
        }
        IF_ERR_THEN_RETURN;
		CyDelay(10);													/* ��ʱ10ms                     */
        TimeOutCount++;
        if (TimeOutCount > IRQ_TIMEOUT)  
        {
            return FALSE;                                               /* 150ms ��ʱ�˳�               */
        }
    }
    return TRUE; 
}

/*********************************************************************************************************
** Function name:       ReadLpcdADCResult()
** Descriptions:        ��ȡLPCD����
** input parameters:    uchar *ADCResult
** output parameters:   
** Returned value:      uint8_t   TRUE����ȡ�ɹ�   FALSE:ʧ��
*********************************************************************************************************/
uint8_t  ReadLpcdADCResult(uint8_t  *ADCResult)
{
    uint8_t  ExtRegData;
    uint8_t  ret;
    uint8_t  ADCResultTemp;

    *ADCResult = 0;
    
    ret = GetReg_Ext(JREG_LPCD_ADC_RESULT_H,&ExtRegData);               /* ��ȡ�����Ϣ                 */
    IF_ERR_THEN_RETURN;
    ADCResultTemp = (ExtRegData & 0x3) << 6;
    
    ret = GetReg_Ext(JREG_LPCD_ADC_RESULT_L,&ExtRegData);
    IF_ERR_THEN_RETURN;
    
    ADCResultTemp += (ExtRegData&0x3F) ;
    *ADCResult = ADCResultTemp;

    ret = SetReg_Ext(JREG_LPCD_CTRL1,JBIT_BIT_CTRL_CLR+JBIT_LPCD_RSTN); /* ��λLPCD�Ĵ���               */
    IF_ERR_THEN_RETURN;

    ret = SetReg_Ext(JREG_LPCD_CTRL1,JBIT_BIT_CTRL_SET+JBIT_LPCD_RSTN); /* ��λ�ſ�LPCD�Ĵ���           */
    IF_ERR_THEN_RETURN;
        
    //DBG_PRINTF("ADCResultTemp = %d \r\n",ADCResultTemp);
    return TRUE;
}

/*********************************************************************************************************
** Function name:       CalibraReadADCResult()
** Descriptions:        ���̲���ȡLPCD����
** input parameters:    uchar *ADCResult
** output parameters:   
** Returned value:      uint8_t   TRUE����ȡ�ɹ�   FALSE:ʧ��
*********************************************************************************************************/
uint8_t  CalibraReadADCResult(uint8_t  *ADCResult)
{
    //ʹ�ܵ���ģʽ
    uint8_t  ret;
    ret = SetReg_Ext(JREG_LPCD_CTRL1,JBIT_BIT_CTRL_CLR+JBIT_LPCD_RSTN);
    IF_ERR_THEN_RETURN;
    ret = SetReg_Ext(JREG_LPCD_CTRL1,JBIT_BIT_CTRL_CLR+JBIT_LPCD_CALIBRA_EN);
    IF_ERR_THEN_RETURN;
    ret = SetReg_Ext(JREG_LPCD_CTRL1,JBIT_BIT_CTRL_SET+JBIT_LPCD_RSTN);
    IF_ERR_THEN_RETURN;
    ret = SetReg_Ext(JREG_LPCD_CTRL1,JBIT_BIT_CTRL_SET+JBIT_LPCD_CALIBRA_EN);
    IF_ERR_THEN_RETURN;
    CyDelay(10);														/* ��ʱ10ms						*/
    ret = WaitForLpcdIrq(JBIT_CALIB_IRQ);                               /* �ȴ����̽����ж�             */
    if (ret == 0)
    {
        ret =1;
    }
    IF_ERR_THEN_RETURN;
    ret = SetReg_Ext(JREG_LPCD_CTRL1,JBIT_BIT_CTRL_CLR+JBIT_LPCD_CALIBRA_EN); /* �رյ���ģʽ           */
    IF_ERR_THEN_RETURN;
        
    ret = ReadLpcdADCResult(ADCResult);                                 /* ��ȡ������Ϣ                 */
    IF_ERR_THEN_RETURN;
    return TRUE;
}

/*********************************************************************************************************
** Function name:       LpcdCardIn_IRQHandler()
** Descriptions:        ��Ƭ�����������
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void LpcdCardIn_IRQHandler(void)
{
    uint8_t  ExtRegData;
    uint8_t  ADCResultTemp;

    GetReg_Ext(JREG_LPCD_ADC_RESULT_H,&ExtRegData);                     /* ��ȡ�����Ϣ                 */
    //IF_ERR_THEN_RETURN;
    ADCResultTemp = (ExtRegData & 0x3) << 6;
    
    GetReg_Ext(JREG_LPCD_ADC_RESULT_L,&ExtRegData);
    //IF_ERR_THEN_RETURN;
      
    ADCResultTemp += (ExtRegData&0x3F) ;//& 0x3F)
    //DBG_PRINTF("ADCResultTemp = %d\r\n",ADCResultTemp);        
    SetReg_Ext(JREG_LPCD_CTRL1,JBIT_BIT_CTRL_CLR+JBIT_LPCD_RSTN);       /* ��λLPCD�Ĵ���               */
    SetReg_Ext(JREG_LPCD_CTRL1,JBIT_BIT_CTRL_SET+JBIT_LPCD_RSTN);       /* ��λ�ſ�LPCD�Ĵ���           */
    //��Ϊÿ�λ��ѣ����ԼĴ����ᱻ��λ
    //LpcdAuxSelect(ON);                    							/* ����AUX�۲�ͨ��				*/
    //��Ҫ��ʱ�������п������һ���Ĵ���д����
	CyDelay(10);														/* ��ʱ10ms						*/
    /*
      ** �û��ڴ�����жϿ�Ƭ����֮��Ĵ�������request��
      ** Ҳ���Ը��ݿ�Ƭ�Ƿ���ʵ���������µ����Զ���У����
        */
    //DBG_PRINTF("FM17550 into LPCD mode.\r\n");
    NRSTPD_CTRL(0);														/* ��������LPCDģʽ             */
	CyDelay(10);														/* ��ʱ10ms						*/
}

/*********************************************************************************************************
** Function name:       LpcdAutoWakeUp_IRQHandler()
** Descriptions:        �Զ������жϷ������
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void LpcdAutoWakeUp_IRQHandler(void)
{
    uchar CalibraFlag;
    uint8_t  ExtRegData;
    uint8_t  ADCResultTemp;

    GetReg_Ext(JREG_LPCD_ADC_RESULT_H,&ExtRegData);                     /* ��ȡ�����Ϣ                 */
    //IF_ERR_THEN_RETURN;
    ADCResultTemp = (ExtRegData & 0x3) << 6;
    
    GetReg_Ext(JREG_LPCD_ADC_RESULT_L,&ExtRegData);                     /* ��ȡ�����Ϣ                */
    //IF_ERR_THEN_RETURN;
    
    ADCResultTemp += (ExtRegData&0x3F) ;
    //DBG_PRINTF("LpcdAutoWakeUp_IRQHandler.\r\n");
    LpcdParamInit();                                                    /* LPCD������ʼ��               */
    LpcdRegisterInit();                                                 /* LPCD�Ĵ�����ʼ��             */
    //LpcdAuxSelect(ON);                                                /* ����AUX�۲�ͨ��              */
    LpcdInitCalibra(&CalibraFlag);                                      /* LPCD��ʼ������               */
    if (CalibraFlag == TRUE)                                            /* ������̳ɹ���������         */
    {
        //DBG_PRINTF("FM17550 LpcdInitCalibra  OK.\r\n");
        CyDelay(1000);
        NRSTPD_CTRL(0);                                                 /* ����LPCDģʽ                 */
    } 
    else
    {
        //DBG_PRINTF("FM17550 LpcdInitCalibra  FALSE.\r\n");
    }
}


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
** File name:           lpcd_api.c
** Last modified Date:  2016-4-14
** Last Version:        V1.00
** Descriptions:        LPCD功能演示，专用API函数
**
**--------------------------------------------------------------------------------------------------------
*/
/********************************************************************************************************/
/*     FM175XX LPCD API函数库                                                                           */
/*     主要功能:                                                                                        */
/*     1.实现LPCD相关API函数                                                                            */
/*     编制:罗挺松                                                                                      */
/*     编制时间:2014年7月18日                                                                           */
/********************************************************************************************************/
/*
** LPCD专用头文件，移植时都需要使用到
*/
#include "lpcd_reg.h"
#include "lpcd_config.h"
#include "lpcd_config_user.h"
#include "lpcd_regctrl.h"
#include "spi.h"
/*
** FM175xx头文件，移植时都需要用到
*/ 
#include "fm175xx.h"
#include "Utility.h"


extern unsigned char Event_Flag ;          		    //是否有事件要处理
extern unsigned char Event_FM175XX_IRQ  ;   		//FM175XX IRQ是否被置起，IRQ的上升沿该中断起来
#define COMMIEN_DEF			0x80
#define DIVIEN_DEF			0x80


#define IF_ERR_THEN_RETURN 	if (ret == FALSE) return FALSE
#define ADC_REFERNCE_MIN 	0x0                                            /* 实际并联电容值最小           */
#define ADC_REFERNCE_MAX 	0x7F                                           /* 实际电容值最大               */

#define uchar  uint8_t

/*
** 内部函数列表
*/
uint8_t  WaitForLpcdIrq(uint8_t  IrqType);
uint8_t  ReadLpcdADCResult(uint8_t  *ADCResult);
uint8_t  CalibraReadADCResult(uint8_t  *ADCResult);
uint8_t  LpcdSet_PGA(uint8_t  *GainCalibraFlag, uint8_t  *ADCResult);
uint8_t  LpcdSet_ADCRefvoltage(uint8_t  *CalibraFlag, uint8_t  *ADCResult);
uint8_t  LpcdSet_DetectSensitive(float Sensitive);

/*
** 调教变量列表
*/ 
uint8_t  T3ClkDivK ;
uint8_t  LpcdBiasCurrent;                                               /* 3bit 由样片测试指标决定，
                                                                            在config文件中设定          */
uint8_t  LpcdGainReduce;                                                /* 2bit                         */
uint8_t  LpcdGainAmplify;                                               /* 3bit                         */
uint8_t  LpcdADCRefernce;                                               /* 7bit                         */

uint8_t  Timer1Cfg;                                                     /* 4bit                         */
uint8_t  Timer2Cfg;                                                     /* 5bit                         */
uint8_t  Timer3Cfg;                                                     /* 5bit                         */

uint8_t  ADCResultFullScale;                                            /* T3下满幅ADCResult信息        */
uint8_t  ADCResultThreshold;                                            /* 检测幅度，设置成相对值       */
uint8_t  LpcdThreshold_L;                                               /* LPCD幅度低阈值               */
uint8_t  LpcdThreshold_H;                                               /* LPCD幅度高阈值               */
uint8_t  ADCResultCenter;                                               /* LPCD幅度中心点               */
uint8_t  LpcdADCResult[10];                                             /* Lpcd幅度信息，用于误触发判断 */


/*********************************************************************************************************
** Function name:       LpcdParamInit()
** Descriptions:        LPCD参数初始化
** input parameters:    
** output parameters:   
** Returned value:      uint8_t   TRUE：读取成功   FALSE:失败
*********************************************************************************************************/
uint8_t  LpcdParamInit()
{
    LpcdBiasCurrent = LPCD_BIAS_CURRENT ;                               /* 由样片测试指标决定           */
                                                                        /* 在config文件中设定           */
    LpcdGainReduce = 0x3;                                               /* 1x                           */
    LpcdGainAmplify = 0x0;                                              /* 1x                           */
    LpcdADCRefernce = 0;
    Timer1Cfg = TIMER1_CFG;                                             /* LPCD检测休眠阶段时间配置     */
    Timer2Cfg = TIMER2_CFG;                                             /* LPCD准备检测卡片时间配置     */
    Timer3Cfg = TIMER3_CFG;                                             /* LPCD检测阶段时间配置         */

    if (Timer3Cfg > 0xF)  {                                             /* Timer3Cfg用到5bit  16分频    */
        T3ClkDivK = 2;                                                  /* 16分频                       */
        ADCResultFullScale =  ((Timer3Cfg - 1)<<3);
        ADCResultCenter = (ADCResultFullScale >>1);
        ADCResultThreshold = (ADCResultFullScale >> LPCD_THRESHOLD_RATIO);
    }
    else if(Timer3Cfg > 0x7) {                                          /* Timer3Cfg用到4bit  8分频     */
        T3ClkDivK = 1;                                                  /* 8分频                        */
        ADCResultFullScale =  ((Timer3Cfg - 1)<<4);                     /* 160                          */
        ADCResultCenter = (ADCResultFullScale >>1);                     /* 80                           */
        ADCResultThreshold = (ADCResultFullScale >> LPCD_THRESHOLD_RATIO);  /* 10                       */
    }
    else  {
        T3ClkDivK = 0;                                                   /* 4分频                        */
        ADCResultFullScale =  ((Timer3Cfg - 1)<<5);
        ADCResultCenter = (ADCResultFullScale >>1);
        ADCResultThreshold = (ADCResultFullScale >> LPCD_THRESHOLD_RATIO);
    }

    LpcdThreshold_H = ADCResultCenter + ADCResultThreshold;              /* 高触发门槛 80+10=90          */
    LpcdThreshold_L= ADCResultCenter - ADCResultThreshold;               /* 低触发门槛 80-10=70          */
    
    return TRUE;
}

/*********************************************************************************************************
** Function name:       LpcdRegisterInit()
** Descriptions:        LPCD寄存器初始化
** input parameters:    
** output parameters:   
** Returned value:      uint8_t   TRUE：读取成功   FALSE:失败
*********************************************************************************************************/
uint8_t  LpcdRegisterInit(void)
{
    uint8_t  ret;
    uint8_t  regdata;
    
    regdata = COMMIEN_DEF;                                              /* 中断设置，设置中断引脚IRQ输出*/
    ret = spi_SetReg(ComIEnReg,regdata);                                /* 中断IRQ输出低电平            */
    IF_ERR_THEN_RETURN;

    regdata = DIVIEN_DEF;                                               /* 配置IRQ引脚作为标准CMOS输出  */
    ret = spi_SetReg(DivIEnReg,regdata);                                /* STatus1Reg的IRQ位相反        */
    IF_ERR_THEN_RETURN;       
    
    /*
    ** LpcdCtrl1寄存器
    */ 
    ret = SetReg_Ext(JREG_LPCD_CTRL1,JBIT_BIT_CTRL_CLR+JBIT_LPCD_RSTN); /* 复位LPCD寄存器               */
    IF_ERR_THEN_RETURN;
    ret = SetReg_Ext(JREG_LPCD_CTRL1,JBIT_BIT_CTRL_SET+JBIT_LPCD_RSTN); /* 复位放开LPCD寄存器           */
    IF_ERR_THEN_RETURN;
    
    ret = SetReg_Ext(JREG_LPCD_CTRL1,JBIT_BIT_CTRL_SET+JBIT_LPCD_EN);   /* 使能LPCD功能                 */
    IF_ERR_THEN_RETURN;
    
    ret = SetReg_Ext(JREG_LPCD_CTRL1,(LPCD_IE<<5)+JBIT_LPCD_IE);        /* 配置LPCD中断寄存器状态       */
    IF_ERR_THEN_RETURN;                                                 /* 反映到IRQ引脚                */
    
                                                                        /* 配置进场检测次数             */
	/* 一次检测到卡有效             																	*/
    ret = SetReg_Ext(JREG_LPCD_CTRL1,(LPCD_AUTO_DETECT_TIMES<<5)+JBIT_LPCD_CMP_1);
    IF_ERR_THEN_RETURN;

    
    /*
    ** LpcdCtrl2寄存器
    */        
    ret = SetReg_Ext(JREG_LPCD_CTRL2,((LPCD_TX2RFEN<<4)+(LPCD_CWN<<3)+LPCD_CWP)); /* P管驱动能力从0到7  */
    IF_ERR_THEN_RETURN;                                                 /* 一次增大，这里选了3          */
        
    /*
    ** LpcdCtrl3寄存器
    */   
    ret = SetReg_Ext(JREG_LPCD_CTRL3,LPCD_MODE<<3);            			/* 没看出实际意义               */
    IF_ERR_THEN_RETURN;

    /*
    ** Timer1Cfg寄存器
    */ 
    ret = SetReg_Ext(JREG_LPCD_T1CFG,(T3ClkDivK<<4)+Timer1Cfg);         /* 设置LPCD睡眠时间             */
    IF_ERR_THEN_RETURN;
        
    /*
    ** Timer2Cfg寄存器
    */
    ret = SetReg_Ext(JREG_LPCD_T2CFG,Timer2Cfg);                        /* 设置LPCD准备检测时间         */ 
    IF_ERR_THEN_RETURN;
        
    /*
    ** Timer3Cfg寄存器
    */ 
    ret = SetReg_Ext(JREG_LPCD_T3CFG,Timer3Cfg);                        /* 设置LPCD检测卡片时间         */
    IF_ERR_THEN_RETURN;

    /*
    ** VmidBdCfg寄存器
    */
    ret = SetReg_Ext(JREG_LPCD_VMIDBD_CFG,VMID_BG_CFG);                 /* 不建议用户修改               */
    IF_ERR_THEN_RETURN;
        
    /*
    ** AutoWupCfg寄存器
    */
    ret = SetReg_Ext(JREG_LPCD_AUTO_WUP_CFG,(AUTO_WUP_EN<<3)+AUTO_WUP_CFG);  /* 设置自动唤醒时间        */
    IF_ERR_THEN_RETURN;
        
    /*
    ** LpcdThreshold_L1寄存器
    */
    ret = SetReg_Ext(JREG_LPCD_THRESHOLD_MIN_L,(LpcdThreshold_L & 0x3F));  /* 设置卡检测下阀值          */
    IF_ERR_THEN_RETURN;
        
    /*
    ** LpcdThreshold_L2寄存器
    */ 
    ret = SetReg_Ext(JREG_LPCD_THRESHOLD_MIN_H,(LpcdThreshold_L>>6));   
    IF_ERR_THEN_RETURN;
        
    /*
    ** LpcdThreshold_H1寄存器
    */
    ret = SetReg_Ext(JREG_LPCD_THRESHOLD_MAX_L,(LpcdThreshold_H& 0x3F));  /* 设置卡检测上阀值           */
    IF_ERR_THEN_RETURN;
    /*
    ** LpcdThreshold_H2寄存器
    */
    ret = SetReg_Ext(JREG_LPCD_THRESHOLD_MAX_H,(LpcdThreshold_H>>6));   
    IF_ERR_THEN_RETURN;
    
    /*
    ** Auto_Wup_Cfg寄存器
    */
    ret=SetReg_Ext(JREG_LPCD_AUTO_WUP_CFG,(AUTO_WUP_EN<<3) + AUTO_WUP_CFG );  /* 再次设置自动唤醒时间   */
    IF_ERR_THEN_RETURN;
        
    return TRUE;
}

/*********************************************************************************************************
** Function name:       LpcdSet_DetectSensitive()
** Descriptions:        设置检测灵敏度
** input parameters:    loat Sensitive：灵敏度
** output parameters:   
** Returned value:      uint8_t   TRUE：读取成功   FALSE:失败
*********************************************************************************************************/
uint8_t   LpcdSet_DetectSensitive(float Sensitive)
{
    uint8_t  ret,Threshold;
    Threshold =ADCResultCenter*Sensitive; 
    /* 微调阈值 */
    /****LpcdThreshold_H = ADCResultCenter + ADCResultThreshold;*/
    /****LpcdThreshold_L= ADCResultCenter - ADCResultThreshold;*/
    LpcdThreshold_H = ADCResultCenter + Threshold;
    LpcdThreshold_L = ADCResultCenter - Threshold;
    /*
    ** LpcdThreshold_L1寄存器
    */
    ret = SetReg_Ext(JREG_LPCD_THRESHOLD_MIN_L,(LpcdThreshold_L& 0x3F));
    IF_ERR_THEN_RETURN;
    /*
    ** LpcdThreshold_L2寄存器
    */
    ret = SetReg_Ext(JREG_LPCD_THRESHOLD_MIN_H,(LpcdThreshold_L>>6));
    IF_ERR_THEN_RETURN;
    /*
    ** LpcdThreshold_H1寄存器
    */
    ret = SetReg_Ext(JREG_LPCD_THRESHOLD_MAX_L,(LpcdThreshold_H& 0x3F));
    IF_ERR_THEN_RETURN;
    /*
    ** LpcdThreshold_H2寄存器
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
** Descriptions:        设置合适的ADC参考电压
** input parameters:    uint8_t  *CalibraFlag, uint8_t  *ADCResult   幅度检测结果保存地址
** output parameters:   
** Returned value:      uint8_t   TRUE：读取成功   FALSE:失败
*********************************************************************************************************/
uint8_t   LpcdSet_ADCRefvoltage(uint8_t  *CalibraFlag, uint8_t  *ADCResult)
{
    uint8_t  ret;
    uint8_t  ADCResult_Pre;                                             /* 幅度信息的前一个值           */
    
    //扫描充电电容
    for(LpcdADCRefernce = ADC_REFERNCE_MIN;LpcdADCRefernce < ADC_REFERNCE_MAX;LpcdADCRefernce++)
    //通过此循环可以确定最终的参考电压和ADC中心比较值
    {
        //配置参考电压值
        ret = SetReg_Ext(JREG_LPCD_BIAS_CURRENT, ((LpcdADCRefernce & 0x40) << 5) + (LpcdBiasCurrent & 0x7));
        IF_ERR_THEN_RETURN;
        ret = SetReg_Ext(JREG_LPCD_ADC_REFERECE,LpcdADCRefernce&0x3F);
        IF_ERR_THEN_RETURN;
        //备份幅度信息
        ADCResult_Pre = *ADCResult;
        //调教读取当前幅度信息
        ret = CalibraReadADCResult(ADCResult);
        IF_ERR_THEN_RETURN;
        /*
              ** 算法一
              */
        // 如果幅度开始比中心值小，因为开始比中心值大
        if (*ADCResult < ADCResultCenter) {
            //调教成功
            (*CalibraFlag) = TRUE;
            //与前一个调教参考电压，判断哪个幅度更接近中心点
            if((ADCResultCenter - *ADCResult) < (ADCResult_Pre-ADCResultCenter))
            {
                //直接用当前值作为中心点
                ADCResultCenter = *ADCResult;
            }
            else
            {
                //直接用当前值作为中心点
                ADCResultCenter = ADCResult_Pre;
                //参考电压采用之前的参考电压
                LpcdADCRefernce--;
                //重新配置参考电压值
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
** Descriptions:        设置合适的PGA参数
** input parameters:    uint8_t  *CalibraFlag, uint8_t  *ADCResult   幅度检测结果保存地址
** output parameters:   
** Returned value:      uint8_t   TRUE：读取成功   FALSE:失败
*********************************************************************************************************/
uint8_t   LpcdSet_PGA(uint8_t  *GainCalibraFlag, uint8_t  *ADCResult)
{
    uint8_t  ret;
    //参考电压值配置最小
    LpcdADCRefernce = ADC_REFERNCE_MIN;
    ret = SetReg_Ext(JREG_LPCD_BIAS_CURRENT, ((LpcdADCRefernce & 0x40) << 5) + (LpcdBiasCurrent & 0x7));
    IF_ERR_THEN_RETURN;
    ret = SetReg_Ext(JREG_LPCD_ADC_REFERECE,LpcdADCRefernce&0x3F);
    IF_ERR_THEN_RETURN;

    //调教读取当前幅度信息
    ret = CalibraReadADCResult(ADCResult);
    IF_ERR_THEN_RETURN;
    //缺省增益不需要调教
    *GainCalibraFlag = TRUE;
   
    //判断是否幅度太窄，如果太窄lpcd_gain衰减
    if  (*ADCResult < ADCResultCenter)
    {
        //增益需要调教
        *GainCalibraFlag = FALSE;
        //*GainCalibraFlag = LpcdSetPGA_GainReduce(ADCResult);   //设置PGA增益衰减
        while(1)
        {
			//如果当前已经是最小增益，调教失败
            if (LpcdGainReduce == 0)
            {
                *GainCalibraFlag = FALSE;
                break;
            }
            //继续衰减
            LpcdGainReduce --; 
            //DBG_PRINTF("LpcdSet_PGA  LpcdGainReduce=%d !\r\n",LpcdGainReduce);
            //配置增益
            ret = SetReg_Ext(JREG_LPCD_CTRL4,((LpcdGainAmplify << 2) + LpcdGainReduce));
            IF_ERR_THEN_RETURN;

            //调教读取当前幅度信息
            ret = CalibraReadADCResult(ADCResult);
            //DBG_PRINTF("LpcdSet_PGA  ADCResult=%d !\r\n", *ADCResult);
            IF_ERR_THEN_RETURN;
            //调教成功，把中心点移到中心点右侧
            if (*ADCResult > ADCResultCenter)
            {
                *GainCalibraFlag = TRUE;
                break;
            }
         }    
    }
    else
    {
        //参考电压值配置最大
        LpcdADCRefernce = ADC_REFERNCE_MAX;
        ret = SetReg_Ext(JREG_LPCD_BIAS_CURRENT, ((LpcdADCRefernce & 0x40) << 5) + (LpcdBiasCurrent & 0x7));
        IF_ERR_THEN_RETURN;
        ret = SetReg_Ext(JREG_LPCD_ADC_REFERECE,LpcdADCRefernce&0x3F);
        IF_ERR_THEN_RETURN;

        //调教读取当前幅度信息
        ret = CalibraReadADCResult(ADCResult);
        //DBG_PRINTF("LpcdSet_PGA  ADCResult=%d !\r\n",*ADCResult);
        IF_ERR_THEN_RETURN;

        //调教成功标志初始化
        *GainCalibraFlag = TRUE;
        
        //判断是否幅度太小，如果太小lpcd_gain放大
        if (*ADCResult > ADCResultCenter)
        {
            //增益需要调教
            *GainCalibraFlag = FALSE;
            while(1)
            {
                //如果当前已经是最大增益，调教失败
                if (LpcdGainAmplify == 0x7)
                {
                    *GainCalibraFlag = FALSE;
                    break;
                }
                else//继续放大
                {
                    LpcdGainAmplify++;  
                }
				//配置增益
                ret = SetReg_Ext(JREG_LPCD_CTRL4,((LpcdGainAmplify << 2) + LpcdGainReduce));
                IF_ERR_THEN_RETURN;

                //调教读取当前幅度信息
                  ret = CalibraReadADCResult(ADCResult);
                IF_ERR_THEN_RETURN;
                
                //调教成功，把中心点移到中心点左侧
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
** Descriptions:        初始化调教
** input parameters:    uint8_t  *CalibraFlag 调教标志，是否需要调教
** output parameters:   
** Returned value:      uint8_t   TRUE：读取成功   FALSE:失败
*********************************************************************************************************/
uint8_t   LpcdInitCalibra(uint8_t  *CalibraFlag)
{
	uint8_t  ret;
    uint8_t  ADCResult;                                                 /* LPCD幅度信息                 */
    uint8_t  GainCalibraFlag;                                           /* 增益调教结果                 */
                                                                        /* 配置增益                     */
    ret = SetReg_Ext(JREG_LPCD_CTRL4,((LpcdGainAmplify << 2) + LpcdGainReduce));
    IF_ERR_THEN_RETURN;
                                                                        /* 配置偏置电流和参考电压       */
    ret = SetReg_Ext(JREG_LPCD_BIAS_CURRENT, ((LpcdADCRefernce & 0x40) << 5) + (LpcdBiasCurrent & 0x7));
    IF_ERR_THEN_RETURN;

    ret = SetReg_Ext(JREG_LPCD_MISC,BFL_JBIT_CALIB_VMID_EN);            /* 开启调校模式中Vmind使能      */
    IF_ERR_THEN_RETURN;

    ret = SetReg_Ext(JREG_LPCD_T1CFG,(T3ClkDivK<<4)+Timer1Cfg);         /* Timer1Cfg配置                */
    IF_ERR_THEN_RETURN;


    ret = SetReg_Ext(JREG_LPCD_T2CFG,Timer2Cfg);                        /* Timer2Cfg配置                */
    IF_ERR_THEN_RETURN;

    ret = SetReg_Ext(JREG_LPCD_T3CFG,Timer3Cfg);                        /* Timer3Cfg配置                */
    IF_ERR_THEN_RETURN;
                                                                          
    ret = LpcdSet_PGA(&GainCalibraFlag,&ADCResult);                     /* PGA参数设置                  */
    IF_ERR_THEN_RETURN;                                                 /* 调校过程PGA的增益和衰减参数设定 */

    if (GainCalibraFlag == FALSE)                                       /* 如果增益调教失败，则失败     */
    {
		(*CalibraFlag) = FALSE;
        return ADCResult;                                               /* 调教失败返回幅度             */
    }
    //扫描参考电压值，找到合适的穿越中心点的配置
    (*CalibraFlag) = FALSE;
    GainCalibraFlag = LpcdSet_ADCRefvoltage(CalibraFlag,&ADCResult);    /* 设置合适的ADC参考电压        */
    ret = LpcdSet_DetectSensitive(LPCD_DetectSensitive);                /* 调校过程检测灵敏度设定       */
    IF_ERR_THEN_RETURN;
    
    if (GainCalibraFlag == FALSE)
    {        
        (*CalibraFlag) = FALSE;
        ret = ModifyReg_Ext(JREG_LPCD_MISC,BFL_JBIT_CALIB_VMID_EN,0);   /* 增加 调教结束关闭CalibVmidEn */
        IF_ERR_THEN_RETURN;
        return ADCResult;                                               /* 调教失败返回幅度             */
    }
    
    ret = ModifyReg_Ext(JREG_LPCD_MISC,BFL_JBIT_CALIB_VMID_EN,0);       /* 调教结束关闭CalibVmidEn      */
    IF_ERR_THEN_RETURN;
    return TRUE;
}

/*********************************************************************************************************
** Function name:       WaitForLpcdIrq()
** Descriptions:        等待LPCD中断
** input parameters:    IrqType
** output parameters:   
** Returned value:      uint8_t   TRUE：读取成功   FALSE:失败
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
		CyDelay(10);													/* 延时10ms                     */
        TimeOutCount++;
        if (TimeOutCount > IRQ_TIMEOUT)  
        {
            return FALSE;                                               /* 150ms 超时退出               */
        }
    }
    return TRUE; 
}

/*********************************************************************************************************
** Function name:       ReadLpcdADCResult()
** Descriptions:        读取LPCD幅度
** input parameters:    uchar *ADCResult
** output parameters:   
** Returned value:      uint8_t   TRUE：读取成功   FALSE:失败
*********************************************************************************************************/
uint8_t  ReadLpcdADCResult(uint8_t  *ADCResult)
{
    uint8_t  ExtRegData;
    uint8_t  ret;
    uint8_t  ADCResultTemp;

    *ADCResult = 0;
    
    ret = GetReg_Ext(JREG_LPCD_ADC_RESULT_H,&ExtRegData);               /* 读取宽度信息                 */
    IF_ERR_THEN_RETURN;
    ADCResultTemp = (ExtRegData & 0x3) << 6;
    
    ret = GetReg_Ext(JREG_LPCD_ADC_RESULT_L,&ExtRegData);
    IF_ERR_THEN_RETURN;
    
    ADCResultTemp += (ExtRegData&0x3F) ;
    *ADCResult = ADCResultTemp;

    ret = SetReg_Ext(JREG_LPCD_CTRL1,JBIT_BIT_CTRL_CLR+JBIT_LPCD_RSTN); /* 复位LPCD寄存器               */
    IF_ERR_THEN_RETURN;

    ret = SetReg_Ext(JREG_LPCD_CTRL1,JBIT_BIT_CTRL_SET+JBIT_LPCD_RSTN); /* 复位放开LPCD寄存器           */
    IF_ERR_THEN_RETURN;
        
    //DBG_PRINTF("ADCResultTemp = %d \r\n",ADCResultTemp);
    return TRUE;
}

/*********************************************************************************************************
** Function name:       CalibraReadADCResult()
** Descriptions:        调教并读取LPCD幅度
** input parameters:    uchar *ADCResult
** output parameters:   
** Returned value:      uint8_t   TRUE：读取成功   FALSE:失败
*********************************************************************************************************/
uint8_t  CalibraReadADCResult(uint8_t  *ADCResult)
{
    //使能调教模式
    uint8_t  ret;
    ret = SetReg_Ext(JREG_LPCD_CTRL1,JBIT_BIT_CTRL_CLR+JBIT_LPCD_RSTN);
    IF_ERR_THEN_RETURN;
    ret = SetReg_Ext(JREG_LPCD_CTRL1,JBIT_BIT_CTRL_CLR+JBIT_LPCD_CALIBRA_EN);
    IF_ERR_THEN_RETURN;
    ret = SetReg_Ext(JREG_LPCD_CTRL1,JBIT_BIT_CTRL_SET+JBIT_LPCD_RSTN);
    IF_ERR_THEN_RETURN;
    ret = SetReg_Ext(JREG_LPCD_CTRL1,JBIT_BIT_CTRL_SET+JBIT_LPCD_CALIBRA_EN);
    IF_ERR_THEN_RETURN;
    CyDelay(10);														/* 延时10ms						*/
    ret = WaitForLpcdIrq(JBIT_CALIB_IRQ);                               /* 等待调教结束中断             */
    if (ret == 0)
    {
        ret =1;
    }
    IF_ERR_THEN_RETURN;
    ret = SetReg_Ext(JREG_LPCD_CTRL1,JBIT_BIT_CTRL_CLR+JBIT_LPCD_CALIBRA_EN); /* 关闭调教模式           */
    IF_ERR_THEN_RETURN;
        
    ret = ReadLpcdADCResult(ADCResult);                                 /* 读取幅度信息                 */
    IF_ERR_THEN_RETURN;
    return TRUE;
}

/*********************************************************************************************************
** Function name:       LpcdCardIn_IRQHandler()
** Descriptions:        卡片进场服务程序
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void LpcdCardIn_IRQHandler(void)
{
    uint8_t  ExtRegData;
    uint8_t  ADCResultTemp;

    GetReg_Ext(JREG_LPCD_ADC_RESULT_H,&ExtRegData);                     /* 读取宽度信息                 */
    //IF_ERR_THEN_RETURN;
    ADCResultTemp = (ExtRegData & 0x3) << 6;
    
    GetReg_Ext(JREG_LPCD_ADC_RESULT_L,&ExtRegData);
    //IF_ERR_THEN_RETURN;
      
    ADCResultTemp += (ExtRegData&0x3F) ;//& 0x3F)
    //DBG_PRINTF("ADCResultTemp = %d\r\n",ADCResultTemp);        
    SetReg_Ext(JREG_LPCD_CTRL1,JBIT_BIT_CTRL_CLR+JBIT_LPCD_RSTN);       /* 复位LPCD寄存器               */
    SetReg_Ext(JREG_LPCD_CTRL1,JBIT_BIT_CTRL_SET+JBIT_LPCD_RSTN);       /* 复位放开LPCD寄存器           */
    //因为每次唤醒，测试寄存器会被复位
    //LpcdAuxSelect(ON);                    							/* 开启AUX观测通道				*/
    //需要延时，否则有可能最后一个寄存器写不进
	CyDelay(10);														/* 延时10ms						*/
    /*
      ** 用户在此添加判断卡片进场之后的处理，比如request等
      ** 也可以根据卡片是否真实进场，重新调用自动调校程序
        */
    //DBG_PRINTF("FM17550 into LPCD mode.\r\n");
    NRSTPD_CTRL(0);														/* 继续进入LPCD模式             */
	CyDelay(10);														/* 延时10ms						*/
}

/*********************************************************************************************************
** Function name:       LpcdAutoWakeUp_IRQHandler()
** Descriptions:        自动唤醒中断服务程序
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void LpcdAutoWakeUp_IRQHandler(void)
{
    uchar CalibraFlag;
    uint8_t  ExtRegData;
    uint8_t  ADCResultTemp;

    GetReg_Ext(JREG_LPCD_ADC_RESULT_H,&ExtRegData);                     /* 读取宽度信息                 */
    //IF_ERR_THEN_RETURN;
    ADCResultTemp = (ExtRegData & 0x3) << 6;
    
    GetReg_Ext(JREG_LPCD_ADC_RESULT_L,&ExtRegData);                     /* 读取宽度信息                */
    //IF_ERR_THEN_RETURN;
    
    ADCResultTemp += (ExtRegData&0x3F) ;
    //DBG_PRINTF("LpcdAutoWakeUp_IRQHandler.\r\n");
    LpcdParamInit();                                                    /* LPCD参数初始化               */
    LpcdRegisterInit();                                                 /* LPCD寄存器初始化             */
    //LpcdAuxSelect(ON);                                                /* 开启AUX观测通道              */
    LpcdInitCalibra(&CalibraFlag);                                      /* LPCD初始化调教               */
    if (CalibraFlag == TRUE)                                            /* 如果调教成功，则亮灯         */
    {
        //DBG_PRINTF("FM17550 LpcdInitCalibra  OK.\r\n");
        CyDelay(1000);
        NRSTPD_CTRL(0);                                                 /* 进入LPCD模式                 */
    } 
    else
    {
        //DBG_PRINTF("FM17550 LpcdInitCalibra  FALSE.\r\n");
    }
}


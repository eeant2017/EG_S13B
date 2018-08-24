//-----------------------------------------------------------------------------
//	file name:		crc16check.h
//	function:		Header file of crc16check.c
//	author:			
//	date:			
//					Copyright(c) ZhiAnTech Coporation,2017
//-----------------------------------------------------------------------------
#include "crc16check.h"



//=================================================================================
//function:		生产CRC校验
//parameters:    	uint8  MyChar 校验数据
//                    int Mylen 数据长度
//return value:  	CRC校验
//=================================================================================
const unsigned int A_VALUE = 0xFFFF;
const unsigned int DIV_VALUE = 0x4821;
uint32 MyCrc16Check(uint8 *MyChar, uint32 Mylen) 
{
    uint32 I, J;
    uint32 Crc_Value = A_VALUE;
    uint32 My_Check;
    for (I = 0; I < Mylen; I++)
    {
	    Crc_Value = Crc_Value ^ MyChar[I];
	    for (J = 0; J < 8; J++)
	    {
		    My_Check = Crc_Value & 1;
		    Crc_Value = Crc_Value >> 1;
		    if (My_Check == 1)
		    {
		    	Crc_Value = Crc_Value ^ DIV_VALUE;
		    }
	    }
    }
    return Crc_Value;
}
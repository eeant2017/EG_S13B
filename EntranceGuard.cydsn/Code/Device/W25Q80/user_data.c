//********************************************************************************************************
// File name		: user_data.c
// Description		: 用户数据相关的函数
// Author			: Keno
// Date				: 2016.8.8
// Version			: V02
//--------------------------------------------------------------------------------------------------------
// Info				: 用户编号从0开始，0-499；开门记录也从0开始，0-4999，5000为一周期；
//********************************************************************************************************
#include "user_data.h"


int Endpoint_exchange(uint8 card_data[]);
extern uint8  uploadpolling_stop;										// 轮训上报开门信息 停止位		//

//===================================== 用户信息存储 ===================================================//

//********************************************************************************************************
// Function name	: user_init_valid_flag
// Description		: 初始化用户有效位标志，清零所有用户标志位
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void user_init_valid_flag(void)
{
	uint8 flag[USER_FLAG_SIZE];
	
	memset(flag, 0, sizeof(flag));
	W25QXX_Write(flag, USER_FLAG_BASE, (uint16)sizeof(flag));
}

//********************************************************************************************************
// Function name	: user_find_free_id
// Description		: 查找空闲的用户有效位标志 
// Parameter		: user_id
// Return			: RET_OK : 找到 / RET_ERR : 未找到
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint8 user_find_free_id(uint16 *user_id)
{
	uint16 user_id_local;
	uint8 flag[USER_FLAG_SIZE];
	
	W25QXX_Read(flag, USER_FLAG_BASE, (uint16)sizeof(flag));
	for (user_id_local = 0; user_id_local < MAX_USR_NUM; user_id_local++)
	{
		if ((flag[user_id_local] == 0) || (flag[user_id_local] == 0xFF))
		{
			*user_id = user_id_local;
			return RET_OK;
		}
	}
	return RET_ERR;
}

//********************************************************************************************************
// Function name	: user_id_if_valid
// Description		: 判断对应编号的用户是否存在
// Parameter		: user_id
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 用户编号从0开始，0-499.
//********************************************************************************************************
uint8 user_id_if_valid(uint16 user_id)
{
	uint8 flag[USER_FLAG_SIZE];
	
	W25QXX_Read(flag, USER_FLAG_BASE, USER_FLAG_SIZE);
	if (flag[user_id] == TRUE)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//********************************************************************************************************
// Function name	: user_get_reg_qty
// Description		: 获取已注册用户数量
// Parameter		: none
// Return			: qty	: 已注册用户数量
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint16 user_get_reg_qty(void)
{
	uint8 flag[USER_FLAG_SIZE];
	uint16 user_id_local;
	uint16 qty = 0;
	
	W25QXX_Read(flag, USER_FLAG_BASE, USER_FLAG_SIZE);
	for (user_id_local = 0; user_id_local < MAX_USR_NUM; user_id_local++)
	{
		if (flag[user_id_local] == TRUE)
		{
			qty += 1;
		}
	}
	return qty;
}

//********************************************************************************************************
// Function name	: user_set_valid_flag
// Description		: 置位用户有效位标志
// Parameter		: user_id
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void user_set_valid_flag(uint16 user_id)
{
	uint8 flag = TRUE;
	
	W25QXX_Write(&flag, USER_FLAG_BASE + user_id, 1);
}

//********************************************************************************************************
// Function name	: user_clr_valid_flag
// Description		: 清除用户有效位标志
// Parameter		: user_id
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void user_clr_valid_flag(uint16 user_id)
{
	uint8 flag = FALSE;
	
	W25QXX_Write(&flag, USER_FLAG_BASE + user_id, 1);
}

//********************************************************************************************************
// Function name	: user_add_info
// Description		: 添加新的用户信息
// Parameter		: user_id，user_info_t
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void user_add_info(uint16 user_id, user_info_t *info)
{
	W25QXX_Write((uint8 *)info, USER_INFO_BASE + (sizeof(user_info_t) * user_id), (uint16)sizeof(user_info_t));
}

//********************************************************************************************************
// Function name	: user_del_info
// Description		: 删除新的用户信息
// Parameter		: user_id
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void user_del_info(uint16 user_id)
{
	uint8_t flag[sizeof(user_info_t)] = {0};
	
	W25QXX_Write(flag, USER_INFO_BASE + sizeof(user_info_t) * user_id, (uint16)sizeof(user_info_t));
}

//********************************************************************************************************
// Function name	: user_get_info
// Description		: 根据user_id来获取Info用户信息
// Parameter		: user_id，user_info_t
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void user_get_info(uint16 user_id, user_info_t *info)
{
	W25QXX_Read((uint8 *)info, USER_INFO_BASE + sizeof(user_info_t) * user_id, (uint16)sizeof(user_info_t));
}

//********************************************************************************************************
// Function name	: user_get_card_uid
// Description		: 根据user_id来获取卡uid
// Parameter		: user_id，*card_uid
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void user_get_card_uid(uint16 user_id, uint8 *card_uid)
{
	W25QXX_Read(card_uid, USER_INFO_BASE + sizeof(user_info_t) * user_id + offsetof(user_info_t, card_uid), (uint16)CARD_UID_LEN);
}

//********************************************************************************************************
// Function name	: user_get_user_id
// Description		: 根据uid信息来获取user_id
// Parameter		: *user_id ，*card_uid
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 读到卡了，返回卡用户编号
//					: 没读到卡，返回MAX_USR_NUM
//********************************************************************************************************
void user_get_user_id(uint16 *user_id, uint8 *card_uid)
{
	uint16 i;
	uint8 ret;
	uint8 flag[USER_FLAG_SIZE];
	uint8 card_id_eeprom[CARD_UID_LEN];
	
	W25QXX_Read(flag, USER_FLAG_BASE, USER_FLAG_SIZE);
	for (i = 0; i < USER_FLAG_SIZE; i++)
	{
		if (flag[i] == TRUE)
		{
			user_get_card_uid(i, card_id_eeprom);
			ret = memcmp(card_uid, card_id_eeprom, CARD_UID_LEN);
			if (ret == 0)											
			{
				*user_id = i;
				return;
			}
		}
	}
	*user_id = MAX_USR_NUM;
}

//********************************************************************************************************
// Function name	: user_get_valid_time
// Description		: 获取已存用户的有效时间
// Parameter		: user_id，*valid_time
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void user_get_valid_time(uint16 user_id, uint8 *valid_time)
{
	W25QXX_Read(valid_time, USER_INFO_BASE + sizeof(user_info_t) * user_id + offsetof(user_info_t, valid_time), (uint16)TIME_LEN);
}

//********************************************************************************************************
// Function name	: add_user
// Description		: 添加用户信息
// Parameter		: user_id，*user_info_t
// Return			: TRUE / FALSE
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint8 add_user(uint16 user_id,  user_info_t *info)
{
	user_info_t info_tmp;
	
	user_add_info(user_id, info);
	user_get_info(user_id, &info_tmp);
	if (0 == memcmp(info, &info_tmp, sizeof(user_info_t)))
	{
		user_set_valid_flag(user_id);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//********************************************************************************************************
// Function name	: chg_user
// Description		: 修改用户信息
// Parameter		: user_id，*user_info_t
// Return			: TRUE / FALSE
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint8 chg_user(uint16 user_id, user_info_t *info)
{
	user_info_t info_tmp;
	
	user_add_info(user_id, info);
	user_get_info(user_id, &info_tmp);
	if (0 == memcmp(info, &info_tmp, sizeof(user_info_t)))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//********************************************************************************************************
// Function name	: del_user
// Description		: 删除用户信息
// Parameter		: user_id
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 删除后取卡UID和有效期的一个数来验证确认是否删除.
//--------------------------------------------------------------------------------------------------------
//					: 还可以再写一个根据UID来删除的函数
//********************************************************************************************************
uint8 del_user(uint16 user_id)
{
	user_info_t info_tmp;
	
	user_del_info(user_id);
	user_get_info(user_id, &info_tmp);
	if ((info_tmp.card_uid[5] == 0) && (info_tmp.valid_time[3] == 0))
	{
		user_clr_valid_flag(user_id);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//********************************************************************************************************
// Function name	: clr_user
// Description		: 清空所有用户
// Parameter		: user_id
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 清空所有用户时，是否保留房东？
//					: 与平涛沟通后，确认全删，锁不管业务层的处理.170829-19:29.
//********************************************************************************************************
uint8 clr_user(void)
{
	uint8_t userflag[USER_FLAG_SIZE] = {0};
	uint8_t userinfo[USER_FLAG_SIZE * sizeof(user_info_t)] = {0};
	
	W25QXX_Write(userflag, USER_FLAG_BASE, USER_FLAG_SIZE);
	W25QXX_Write(userinfo, USER_INFO_BASE, MAX_USR_NUM * sizeof(user_info_t));
	return TRUE;
}

//********************************************************************************************************
// Function name	: UserData_Add_User
// Description		: 添加用户
// Parameter		: info	: 用户信息
// Return			: 处理结果
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint8 UserData_Add_User(user_info_t *info)
{
	uint16_t user_id = 0;
	
	user_get_user_id(&user_id, info->card_uid);
	if (user_id < MAX_USR_NUM)
	{
		if (TRUE == chg_user(user_id, info))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		if (RET_OK != (user_find_free_id(&user_id)))
		{
			return FALSE;
		}
		else
		{
			if (TRUE == add_user(user_id, info))
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
	}
}

//********************************************************************************************************
// Function name	: UserData_DEL_User
// Description		: 删除用户
// Parameter		: card_uid
// Return			: 处理结果
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint8 UserData_DEL_User(uint8 *card_uid)
{
	uint16_t user_id = 0;
	
	user_get_user_id(&user_id, card_uid);
	if (user_id >= MAX_USR_NUM)
	{
		return 0x10;
	}
	else
	{
		if (TRUE == del_user(user_id))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}

//********************************************************************************************************
// Function name	: UserData_CLR_User
// Description		: 清空用户
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint8 UserData_CLR_User(void)
{
	return (clr_user());
}


//********************************************************************************************************
// Function name	: UserData_Judge_If_Open
// Description		: 根据用户信息来判断是否能开门
// Parameter		: info	: 用户信息
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 用户信息包括UID和开门时间
//********************************************************************************************************
uint8 UserData_Judge_If_Open(user_info_t *info)
{
	uint16_t user_id = 0;
	uint8_t val_time[4] = {0};
	
	user_get_user_id(&user_id, info->card_uid);
	if (user_id < MAX_USR_NUM)
	{
		user_get_valid_time(user_id, val_time);
		if((BYTE2LONG(val_time[0], val_time[1], val_time[2], val_time[3])) > 
		   (BYTE2LONG(info->valid_time[0], info->valid_time[1], info->valid_time[2], info->valid_time[3])))
		{
			DBG_SOFT_PutStr("\nUID OK!");	
			return TRUE;
		}
		else
		{
			DBG_SOFT_PutStr("\nOut of date!");	
			return 0x08;								// 超出有效期
		}
	}
	else
	{
		DBG_SOFT_PutStr("\nNo UID!");	
		return 0x06;									// 不存在
	}
}



//=================================== 开门记录信息存储 =================================================//

//********************************************************************************************************
// Function name	: Write_Open_UploadFlag
// Description		: 写开门记录信息上报成功与否标志
// Parameter		: open_num	: 开门编号
//					: SendFlag	: 发送标志位
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void Write_Open_UploadFlag(uint16 open_num, uint8 UploadFlag)
{
	uint8 flag;
	uint16 tmp;
	
	tmp = open_num % MAX_OPEN_NUM;
	W25QXX_Read((uint8 *)&flag, OPEN_FLAG_BASE + tmp, 1);
	if (UploadFlag == TRUE)
	{
		flag |= 0x80;
	}
	else
	{
		flag &= 0x7F;
	}
	W25QXX_Write((uint8 *)&flag, OPEN_FLAG_BASE + tmp, 1);
}

//********************************************************************************************************
// Function name	: Read_Open_UploadFlag
// Description		: 读开门记录信息上报成功与否标志
// Parameter		: open_num	: 开门编号
// Return			: 上报结果
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint8 Read_Open_UploadFlag(uint16 open_num)
{
	uint8 flag;
	uint16 tmp;
	
	tmp = open_num % MAX_OPEN_NUM;
	W25QXX_Read((uint8 *)&flag, OPEN_FLAG_BASE + tmp, 1);
	if (flag & 0x80)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//********************************************************************************************************
// Function name	: Write_Open_Type
// Description		: 写开门记录的开门类型
// Parameter		: open_num	: 开门编号
//					: type		: 开门类型
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void Write_Open_Type(uint16 open_num, uint8 type)
{
	uint8 flag;
	uint16 tmp;
	
	tmp = open_num % MAX_OPEN_NUM;
	//W25QXX_Read((uint8 *)&flag, OPEN_FLAG_BASE + tmp, 1);
	//flag = ((flag & 0x80) | (type & 0x7F));
	flag = type;												// @12.06 modify
	W25QXX_Write((uint8 *)&flag, OPEN_FLAG_BASE + tmp, 1);
}

//********************************************************************************************************
// Function name	: Read_Open_Type
// Description		: 读开门记录的开门类型
// Parameter		: open_num	: 开门编号
// Return			: 开门类型
//--------------------------------------------------------------------------------------------------------
// Info				: ？？
//********************************************************************************************************
uint8 Read_Open_Type(uint16 open_num)
{
	uint8 flag;
	uint16 tmp;
	
	tmp = open_num % MAX_OPEN_NUM;
	W25QXX_Read((uint8 *)&flag, OPEN_FLAG_BASE + tmp, 1);
	return (flag & 0x7F);
}

//********************************************************************************************************
// Function name	: Write_Open_Qty
// Description		: 写开门记录总数量
// Parameter		: openQty
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void Write_Open_Qty(uint16 openQty)
{
    uint8 qty[2];
	
    qty[0] = (openQty >> 8) & 0xff;
    qty[1] = openQty & 0xff;
	W25QXX_Write(qty, OPEN_QTY_BASE, (uint16)sizeof(qty));
}

//********************************************************************************************************
// Function name	: Read_Open_Qty
// Description		: 读开门记录总数量
// Parameter		: openQty
// Return			: openQty
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint16 Read_Open_Qty(void)
{
	uint8 qty[2];
	
	W25QXX_Read(qty, OPEN_QTY_BASE, (uint16)sizeof(qty));
	return ((uint16)((qty[0] << 8) | qty[1]));
}

//********************************************************************************************************
// Function name	: Write_OpenInfo
// Description		: 写开门记录信息(自带数量处理)
// Parameter		: openInfo	: 开门信息
//					: openType	: 开门类型
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 开门记录也从0开始，0-4999，5000为一周期；
//********************************************************************************************************
void Write_OpenInfo(open_door_t *openInfo, uint8_t openType)
{
	uint16 open_qty, tmp;
	
	open_qty = Read_Open_Qty();										// 读出的数量即是下一个要写的偏移量		//
	tmp = open_qty % MAX_OPEN_NUM;
	uint8 openinfo_tmp[17] = {0x01};
	
	
	memcpy(openinfo_tmp+1,openInfo,16);
		#if 0
		// 读出写入前的flash数据测试
		//W25QXX_Read((uint8 *)&tmp_t, OPEN_INFO_BASE + sizeof(open_door_t) * tmp, (uint16)sizeof(open_door_t));
		DBG_SOFT_PutStr("\nTest before write in open date :\n");
		DBG_SOFT_PrintfHex((uint8 *)openinfo_tmp,(uint16)sizeof(open_door_t)+1,16);
		#endif	
		
	W25QXX_Write((uint8 *)openinfo_tmp, OPEN_INFO_BASE + (1+sizeof(open_door_t)) * tmp, (uint16)sizeof(open_door_t)+1);
	if(openType)
	{
		Write_Open_Type(open_qty, openType);						// 写入开门类型信息						//
	
		open_qty++;
		Write_Open_Qty(open_qty);									// 更新开门数量							//
	}
		#if 0
		// 读出写入的flash数据测试
		uint8 temp[17] = {0x00};	
		W25QXX_Read((uint8 *)temp, OPEN_INFO_BASE + (1+sizeof(open_door_t)) * tmp, (uint16)sizeof(open_door_t)+1);
		DBG_SOFT_PutStr("\nTest after write in open date :\n");
		DBG_SOFT_PrintfHex((uint8 *)temp,(uint16)sizeof(open_door_t)+1,16);
		#endif	
	uploadpolling_stop = 0;											// 清除开门信息轮询上报停止位			// 
}

//********************************************************************************************************
// Function name	: Read_OpenInfo
// Description		: 读开门记录信息
// Parameter		: open_num	: 开门记录编号
//					  openInfo	: 读回来的开门信息
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 开门记录也从0开始，0-4999，5000为一周期；
//********************************************************************************************************
void Read_OpenInfo(uint16 open_num, open_door_t *openInfo)
{
    uint16 tmp;
	uint8 data_temp[17] = {0};		//每条开门记录开头加一个标志数据0x01
	uint32 readoff = 0;
	
	tmp = open_num % MAX_OPEN_NUM;
	//W25QXX_Read((uint8 *)data_temp, OPEN_INFO_BASE + (1+sizeof(open_door_t)) * tmp, (uint16)sizeof(open_door_t)+1);
	readoff = (1+sizeof(open_door_t)) * tmp;
	if((readoff % 0xff) == 0)
	{
		readoff += 1;
		W25QXX_Read((uint8 *)data_temp+1, OPEN_INFO_BASE + (readoff), (sizeof(open_door_t)));
	}
	else
	{
		W25QXX_Read((uint8 *)data_temp, OPEN_INFO_BASE + (readoff), (sizeof(open_door_t) + 1));
	}
	
	memcpy(openInfo, data_temp+1, sizeof(open_door_t));
	
	Endpoint_exchange(openInfo->card_uid);									// A卡数据位置转换 @ 18-1-17			//
}

//********************************************************************************************************
// Function name	: Find_Open_StartNum
// Description		: 根据开始时间查找开始编号
// Parameter		: start_time
// Return			: 开门编号，找不到返回0xFFFF
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint16 Find_Open_StartNum(uint32 start_time)
{
	uint16 i, qty;
	uint32 tmp1;
	open_door_t openTmp;
    
	qty = Read_Open_Qty();
	if (qty == 0)
	{
		return 0;
	}
	else if (qty <= MAX_OPEN_NUM)
	{
		for (i = 0; i < qty; i++)
		{
			Read_OpenInfo(i, &openTmp);
			tmp1 = BYTE2LONG(openTmp.open_time[0], openTmp.open_time[1],
			                 openTmp.open_time[2], openTmp.open_time[3]);
			if (tmp1 >= start_time)
			{
				return i;
			}
		}
	}
	else
	{
		for (i = qty - MAX_OPEN_NUM; i < qty; i++)
		{
			Read_OpenInfo(i, &openTmp);
			tmp1 = BYTE2LONG(openTmp.open_time[0], openTmp.open_time[1],
							 openTmp.open_time[2], openTmp.open_time[3]);
			if (tmp1 >= start_time)
			{
				return i;
			}
		}
	}
	return 0xFFFF;
}

//********************************************************************************************************
// Function name	: Find_Open_StartNum
// Description		: 根据结束时间查找结束编号
// Parameter		: end_time
// Return			: 开门编号，找不到返回0xFFFF
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint16 Find_Open_EndNum(uint32 end_time)
{
	uint16 i, qty;
	uint32 tmp1;
	open_door_t openTmp;

	qty = Read_Open_Qty();
	if (qty == 0)
	{
		return 0;
	}
	else if (qty <= MAX_OPEN_NUM)
	{
		for (i = qty -1; i >= 0; i--)
		{
			Read_OpenInfo(i, &openTmp);
			tmp1 = BYTE2LONG(openTmp.open_time[0], openTmp.open_time[1],
							 openTmp.open_time[2], openTmp.open_time[3]);
			if (tmp1 <= end_time)
			{
				return i;
			}
		}
	}
	else
	{
		for (i = qty - 1; i >= qty - MAX_OPEN_NUM; i--)
		{
			Read_OpenInfo(i, &openTmp);
			tmp1 = BYTE2LONG(openTmp.open_time[0], openTmp.open_time[1],
							 openTmp.open_time[2], openTmp.open_time[3]);
			if (tmp1 <= end_time)
			{
				return i;
			}
		}
	}
	return 0xFFFF;
}

//********************************************************************************************************
// Function name	: Find_Open_ValidQty
// Description		: 根据开始时间、结束时间计算需回传的开门总数
// Parameter		: start_time, end_time
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint16 Find_Open_ValidQty(uint32 start_time, uint32 end_time)
{
	uint16 start_num, end_num, allqty;

	allqty = Read_Open_Qty();
	if (allqty == 0)
	{
		return 0;
	}
	start_num = Find_Open_StartNum(start_time);
	end_num = Find_Open_EndNum(end_time);
	if (start_num == 0xFFFF)
	{
		return 0;
	}
	else
	{
		if (end_num == 0xFFFF)
		{
			return (allqty - start_num);
		}
		else
		{
			return (end_num - start_num + 1);
		}
	}
}

//********************************************************************************************************
// Function name	: Get_Open_Info
// Description		: 根据开始编号、数量来提取开门记录信息
// Parameter		: start_num, qty , *data
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void Get_Open_Info(uint16 start_num, uint16 qty, uint8 *data)
{
	uint16 i, offset = 0;
	
	for (i = start_num; i < start_num + qty; i++)
	{
		Read_OpenInfo(i, (open_door_t *)(data + offset));
		offset += sizeof(open_door_t);
	}
}

//********************************************************************************************************
// Function name	: FindAndSet_UploadTime_UploadFlag
// Description		: 根据时间查找开门记录，并更新上报时间及上报成功标志
// Parameter		: open_time	: 开门时间
//					: utc_time	: 时间戳
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void FindAndSet_UploadTime_UploadFlag(uint8 *open_time, uint8 *utc_time)
{
	uint16_t i, OpenQty;
	open_door_t OpenInfo;
	uint32 open_time_utc;
	uint32 open_time_flash;
	uint8 opendate_tmp[17] = {0};
	uint32 readoff = 0;
	
	open_time_utc = BYTE2LONG(open_time[0], open_time[1], open_time[2], open_time[3]);
	OpenQty = Read_Open_Qty();
	//OpenQty %= MAX_OPEN_NUM;
	if (OpenQty <= MAX_OPEN_NUM)
	{
		for (i = OpenQty - 1; i >= 0; i--)
		{
			i %= MAX_OPEN_NUM;
			readoff = i * (1 + sizeof(open_door_t));
			if((readoff % 0xff) == 0)
			{
				readoff += 1;
				W25QXX_Read((uint8 *)opendate_tmp+1, OPEN_INFO_BASE + (readoff), (sizeof(open_door_t)));
			}
			else
			{
				W25QXX_Read((uint8 *)opendate_tmp, OPEN_INFO_BASE + (readoff), (sizeof(open_door_t) + 1));
			}
			memcpy((uint8 *)&OpenInfo, opendate_tmp+1, 16);
			//DBG_SOFT_PutStr("\n ret_code = 1, find sendtime @ i:");
			//DBG_SOFT_PrintfHex(&i,1,1);	
			//DBG_SOFT_PrintfHex(opendate_tmp,16,16);		
			
			//Read_OpenInfo(i,&OpenInfo);
			open_time_flash = BYTE2LONG(OpenInfo.open_time[0], OpenInfo.open_time[1],
							 			OpenInfo.open_time[2], OpenInfo.open_time[3]);
			//DBG_SOFT_PutStr("\nopen_time_flash :");
			//DBG_SOFT_PutHexInt(open_time_flash);
			if (open_time_flash == open_time_utc)
			{
				DBG_SOFT_PutStr("\n===>back upload open date :<===\n");
				memcpy(OpenInfo.send_time, utc_time, TIME_LEN);
				opendate_tmp[0] = 0x01;
				memcpy(opendate_tmp+1,(uint8 *)&OpenInfo,16);
				W25QXX_Write((uint8 *)opendate_tmp, OPEN_INFO_BASE + i * (1 + sizeof(open_door_t)), (sizeof(open_door_t) + 1));
				  //==Write_OpenInfo((open_door_t *)&OpenInfo, 0);				
				Write_Open_UploadFlag(i, TRUE);
			}
			else if ((open_time_flash + Delta) < open_time_utc)
			{
				return;
			}
		}
	}
	else
	{
		for (i = OpenQty - 1; i >= OpenQty - MAX_OPEN_NUM; i--)
		{
			i %= MAX_OPEN_NUM;
			readoff = i * (1 + sizeof(open_door_t));
			if((readoff % 0xff) == 0)
			{
				readoff += 1;
				W25QXX_Read((uint8 *)opendate_tmp+1, OPEN_INFO_BASE + (readoff), (sizeof(open_door_t)));
			}
			else
			{
				W25QXX_Read((uint8 *)opendate_tmp, OPEN_INFO_BASE + (readoff), (sizeof(open_door_t) + 1));
			}
			//W25QXX_Read((uint8 *)opendate_tmp, OPEN_INFO_BASE + i * (1+sizeof(open_door_t)), (sizeof(open_door_t) + 1));
			memcpy((uint8 *)&OpenInfo, opendate_tmp+1, 16);
			
			//==Read_OpenInfo(i,&OpenInfo);
			open_time_flash = BYTE2LONG(OpenInfo.open_time[0], OpenInfo.open_time[1],
							 			OpenInfo.open_time[2], OpenInfo.open_time[3]);
			if (open_time_flash == open_time_utc)
			{
				memcpy(OpenInfo.send_time, utc_time, TIME_LEN);
				opendate_tmp[0] = 0x01;
				memcpy(opendate_tmp+1,(uint8 *)&OpenInfo,16);
				W25QXX_Write((uint8 *)opendate_tmp, OPEN_INFO_BASE + i * (1 + sizeof(open_door_t)), (sizeof(open_door_t) + 1));
				
				//==Write_OpenInfo((open_door_t *)&OpenInfo, 0);	
				Write_Open_UploadFlag(i, TRUE);				
			}
			else if (open_time_flash  + Delta < open_time_utc)
			{
				return;
			}
		}
	}
}

#if 0
uint16 FindfailOpeninfo_offset(uint8 open_time[])
{
	uint16 i, OpenQty;
	open_door_t OpenInfo;
	uint32 open_time_utc;
	uint32 open_time_flash;
	
	open_time_utc = BYTE2LONG(open_time[0], open_time[1], open_time[2], open_time[3]);
	OpenQty = Read_Open_Qty();
	if (OpenQty <= MAX_OPEN_NUM)							// 总数少于5000的情况 没有覆盖
	{
		for (i = OpenQty - 1; i >= 0; i--)
		{
			i %= MAX_OPEN_NUM;
			W25QXX_Read((uint8 *)&OpenInfo, OPEN_INFO_BASE + i * sizeof(open_door_t), sizeof(open_door_t));
			open_time_flash = BYTE2LONG(OpenInfo.open_time[0], OpenInfo.open_time[1],
							 			OpenInfo.open_time[2], OpenInfo.open_time[3]);
			if (open_time_flash == open_time_utc)
			{
				return i;
			}
		}
	}
	else												// 总数超过5000 有覆盖
	{
		for (i = OpenQty - 1; i >= OpenQty - MAX_OPEN_NUM; i--)
		{
			i %= MAX_OPEN_NUM;
			W25QXX_Read((uint8 *)&OpenInfo, OPEN_INFO_BASE + i * sizeof(open_door_t), sizeof(open_door_t));
			open_time_flash = BYTE2LONG(OpenInfo.open_time[0], OpenInfo.open_time[1],
							 			OpenInfo.open_time[2], OpenInfo.open_time[3]);
			if (open_time_flash == open_time_utc)
			{
				return i;
			}
		}
	}

}
#endif


//********************************************************************************************************
// Function name	: Write_RebootType
// Description		: 写重启类型
// Parameter		: rType	    : 重启类型
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void Write_RebootType(uint8 rType)
{
	W25QXX_Write((uint8 *)&rType, REBOOT_TYPE, 1);
}

//********************************************************************************************************
// Function name	: Read_RebootType
// Description		: 读重启类型
// Parameter		: none
// Return			: rType	    : 重启类型
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint8 Read_RebootType(void)
{
    uint8 rType;
    
	W25QXX_Read((uint8 *)&rType, REBOOT_TYPE, 1);
    return rType;
}

//********************************************************************************************************
// Function name	: Write_CenterIpType
// Description		: 写中心服务器类型
// Parameter		: rType	    : 中心服务器类型
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void Write_CenterIpType(uint8 rType)
{
	W25QXX_Write((uint8 *)&rType, CenterIP_TYPE, 1);
}

//********************************************************************************************************
// Function name	: Read_RebootType
// Description		: 读中心服务器类型
// Parameter		: none
// Return			: rType	    : 中心服务器类型
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint8 Read_CenterIpType(void)
{
    uint8 rType;
    
	W25QXX_Read((uint8 *)&rType, CenterIP_TYPE, 1);
    return rType;
}

//********************************************************************************************************
// Function name	: Write_UpdateInfo
// Description		: 存升级版本和标志
// Parameter		: flag/ver
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void Write_UpdateInfo(uint8 flag, uint16 ver)
{
	uint8 tmp[3] = {0};
	
	tmp[0] = (uint8)(ver>>8);
	tmp[1] = (uint8)ver;
	tmp[2] = flag;
	
	W25QXX_Write(tmp, UPDATE_SYSTEMVER, 3);
	
}


//********************************************************************************************************
// Function name	: Read_Updateflag
// Description		: 读升级标志
// Parameter		: none
// Return			: flag
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint8 Read_Updateflag(void)
{	
	uint8 flag;
	W25QXX_Read((uint8 *)&flag, UPDATE_FLAG, 1);
	return flag;
}

//********************************************************************************************************
// Function name	: Read_UpdateVer
// Description		: 读升级版本
// Parameter		: none
// Return			: flag
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint8 Read_UpdateVer(void)
{	
	uint8 ver;
	W25QXX_Read((uint8 *)&ver, UPDATE_SYSTEMVER, 1);
	return ver;
}



//********************************************************************************************************
// Function name	: Clear_AllOPeninfo
// Description		: 清除所有openinfo相关的信息区域
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 测试用用 
//********************************************************************************************************
#if 1
void Clear_AllOPeninfo(void)
{
	uint8_t opendate[256] = {0};
	uint8_t opendate_flag[64] = {0};
	uint8_t opendate_qty[64] = {0};
	uint16_t data = 0;
	uint8_t flag[MAX_OPEN_NUM] = {0};
	//memset(opendate,0xff,256);
	//memset(opendate_flag,0xff,64);
	//memset(flag,0xff,MAX_OPEN_NUM);
	//清除flag区域
	W25QXX_Write(flag,  OPEN_FLAG_BASE, MAX_OPEN_NUM);   
	//清除数据区域
	W25QXX_Write(flag,  OPEN_INFO_BASE, MAX_OPEN_NUM);  
	//W25QXX_Write((uint8_t *)&data,  OPEN_INFO_BASE+ (MAX_OPEN_NUM/2 *sizeof(open_door_t)), MAX_OPEN_NUM/2*sizeof(open_door_t));  
	
	//清除计数qty区域 2B
	W25QXX_Write((uint8_t *)&data,  OPEN_QTY_BASE, 2);  
	
		//==测试开门信息日志表
	W25QXX_Read(opendate, OPEN_INFO_BASE,256);
	W25QXX_Read(opendate_flag, OPEN_FLAG_BASE,64);
	W25QXX_Read(opendate_qty, OPEN_QTY_BASE,64);
	
	DBG_SOFT_PutStr("\r\n Read open date:\r\n");
	DBG_SOFT_PrintfHex(opendate,256,17);
	
	DBG_SOFT_PutStr("\r\n Read open flag:\r\n");
	DBG_SOFT_PrintfHex(opendate_flag,64,16);
	
	DBG_SOFT_PutStr("\r\n Read open qty:\r\n");
	DBG_SOFT_PrintfHex(opendate_qty,8,16);
}
#endif

void test_Flash(void)
{
	uint8_t userflag[64] = {0};
	uint8_t userinfo[256] = {0};
	
	//==测试用户信息日志表
	W25QXX_Read(userflag, USER_FLAG_BASE,64);
	W25QXX_Read(userinfo, USER_INFO_BASE,256);
	
	
	DBG_SOFT_PutStr("\r\n userflag:\r\n");
	DBG_SOFT_PrintfHex(userflag,16,16);
	
	DBG_SOFT_PutStr("\r\n userinfo:\r\n");
	DBG_SOFT_PrintfHex(userinfo,96,16);
	
	uint8_t opendate[512] = {0};
	uint8_t opendate_flag[64] = {0};
	uint8_t opendate_qty[8] = {0};
	
	//==测试开门信息日志表
	W25QXX_Read(opendate, OPEN_INFO_BASE,512);
	W25QXX_Read(opendate_flag, OPEN_FLAG_BASE,64);
	W25QXX_Read(opendate_qty, OPEN_QTY_BASE,64);
	
	DBG_SOFT_PutStr("\r\n Read open date:\r\n");
	DBG_SOFT_PrintfHex(opendate,512,17);
	
	DBG_SOFT_PutStr("\r\n Read open flag:\r\n");
	DBG_SOFT_PrintfHex(opendate_flag,64,16);
	
	DBG_SOFT_PutStr("\r\n Read open qty:\r\n");
	DBG_SOFT_PrintfHex(opendate_qty,8,16);
}

void test_readopeninfo(void)
{
	open_door_t open_tmp;	
	Read_OpenInfo(5,&open_tmp);
	DBG_SOFT_PutStr("\r\n Read openinfor test:\r\n");
	DBG_SOFT_PrintfHex(&open_tmp,16,16);
}


//******************************************** END OF FILE *********************************************//
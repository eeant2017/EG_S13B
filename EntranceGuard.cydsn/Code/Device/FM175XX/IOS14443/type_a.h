#ifndef TYPE_A_H
#define TYPE_A_H
#include <project.h>


uint8_t  picc_atqa[2],picc_uid[15],picc_sak[3];

uint8_t  TypeA_Request(uint8_t  *pTagType);
uint8_t  TypeA_WakeUp(uint8_t  *pTagType);
uint8_t  TypeA_Anticollision(uint8_t  selcode,uint8_t  *pSnr);
uint8_t  TypeA_Select(uint8_t  selcode,uint8_t  *pSnr,uint8_t  *pSak);
uint8_t  TypeA_Halt(uint8_t  AnticollisionFlag);
uint8_t  TypeA_CardActive(uint8_t  *pTagType,uint8_t  *pSnr,uint8_t  *pSak);

#endif

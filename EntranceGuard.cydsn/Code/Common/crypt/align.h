#ifndef __ALIGN_H__
#define __ALIGN_H__

#define ALIGN_X(v, align_num) ((v) + (align_num-1)) & (~(align_num-1))
#define ALIGN_4(x) ALIGN_X(x, 4)
#define ALIGN_16(x) ALIGN_X(x, 16)

#define ROUND_X(v, round) ((((v) + ((round) - 1)) / (round)) * (round))

#define AES_KEY_LEN		(128UL)  //bit
#define AES_ALIGN_LEN	(AES_KEY_LEN>>3)

#define SL_ALIGN AES_ALIGN_LEN

#endif

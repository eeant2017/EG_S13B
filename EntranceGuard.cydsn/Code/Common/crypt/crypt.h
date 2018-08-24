
#ifndef __CRYPT_H__
#define __CRYPT_H__
#include <project.h>
#include <stdio.h>
	
	
#define	CONFIG_AES_ECB		1	
	
extern void init_crypt(void);
extern int do_crypt(uint8_t *key, uint8_t *in, uint8_t *out, uint32_t len, uint8_t enc_dec);

#endif

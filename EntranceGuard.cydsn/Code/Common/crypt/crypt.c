#include <string.h>
#include <project.h>
#include "crypt.h"

#if defined CONFIG_AES_ECB
#include "Aesecb.h"
#elif defined CONFIG_DES_ECB
#include "Desecb.h"
#endif


/***************************************************************************************
**@func    init encrypt/decrypt engine
****************************************************************************************/
void init_crypt(void)
{
#if defined CONFIG_AES_ECB
	bt_lock_open_crypto();
#endif
}

/**************************************************************************************
**@func     encrypt/decrypt
**@key      key for encrypt/decrypt
**@in       data to be encrypt or decrypt
**@out      data has been encrypt or decrypt
**@len      len of data, 16bytes-align
**@enc_dec  1 - encrypt, 0 - decrypt
****************************************************************************************/
int do_crypt(uint8_t *key, uint8_t *in, uint8_t *out, uint32_t len, uint8_t enc_dec)
{
#if defined CONFIG_AES_ECB
	return bt_lock_crypto(key, in, out, len, enc_dec);
#elif defined CONFIG_DES_ECB
	DES2_ECB_CTX context;
	DES2_ECBInit(&context, key, key+8, enc_dec);
	return DES2_ECBUpdate(&context, in, out, len);
#endif
	return 0;
}

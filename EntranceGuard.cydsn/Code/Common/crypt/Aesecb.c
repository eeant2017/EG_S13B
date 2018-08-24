#include <string.h>
#include <project.h>
#include "crypt.h"
#include "common.h"
#include "align.h"

#ifdef CONFIG_AES_ECB

#ifdef ARCH_CC2640
CryptoCC26XX_Handle crypto_hdl;
CryptoCC26XX_Params crypto_param;
#endif

const uint8 nonce[13] = {0xef, 0xef, 0xca, 0xb3, 0x92, 0x9e, 0xc3, 0xc7, 0x12, 0x38, 0x1d, 0xcd, 0x22};

int bt_lock_open_crypto(void) 
{
#ifdef ARCH_CC2640
	CryptoCC26XX_init();
	CryptoCC26XX_Params_init(&crypto_param);
	crypto_hdl = CryptoCC26XX_open(0, 1, &crypto_param);
	if (crypto_hdl == NULL)
		return -1;
#endif
	return 0;
}

int bt_lock_crypto(uint8_t *key, uint8_t *in, uint8_t *out, uint32_t len, uint8_t enc_dec) 
{
	uint32_t i;
	int ret = 0;
#ifdef ARCH_CC2640
	int key_index;
	CryptoCC26XX_AESECB_Transaction ecb_trans;

	if (NULL == crypto_hdl)
		return -1;
	
	CryptoCC26XX_Transac_init((CryptoCC26XX_Transaction*)&ecb_trans, 
		enc_dec? CRYPTOCC26XX_OP_AES_ECB: CRYPTOCC26XX_OP_AES_ECB_NOCRYPT);
	key_index = CryptoCC26XX_allocateKey(crypto_hdl, CRYPTOCC26XX_KEY_ANY, (uint32_t*)key);
	if (key_index < 0)
		return -1;

	for(i = 0; i < len; i+=16) {
		ecb_trans.keyIndex	= key_index;
		ecb_trans.msgIn		= &in[i];
		ecb_trans.msgOut	= &out[i];
		ret = CryptoCC26XX_transact(crypto_hdl, (CryptoCC26XX_Transaction*)&ecb_trans);
		if (ret != 0) {
			ret = -1;
			break;
		}
	}
	
	CryptoCC26XX_releaseKey(crypto_hdl, &key_index);
#else
#if 0
	uint16 left_len = len, crypt_len = 0;
	uint8 cp_len = 0;
	uint8 *in_tmp = in, *out_tmp = out;
		
	while(left_len > 0) {
		
		CyBle_AesCcmInit(); 
		if (enc_dec) {			
			cp_len = left_len > 27? 27: left_len;
			if ((ret = CyBle_AesCcmEncrypt(key, nonce, in, cp_len, out+4, out)) != CYBLE_ERROR_OK) {
				DBG_PRINTF("<%s-%d>: fail\n", __FUNCTION__, __LINE__);
				return -1;
			}
			left_len -= cp_len;
			in += cp_len;
			out += (4+cp_len);
		} else {
		
			cp_len = left_len > (27+4)? (27): left_len;
			if ((ret = CyBle_AesCcmDecrypt(key, nonce, in+4, cp_len, out, in)) != CYBLE_ERROR_OK) {
				DBG_PRINTF("<%s-%d>: fail\n", __FUNCTION__, __LINE__);
				return -1;
			}
			left_len -= (cp_len+4);
			in += (cp_len+4);
			out += cp_len;
		}
		
	}
#else
	for(i = 0; i < len; i+=16) {
		if (enc_dec)
			AES128_ECB_encrypt(&in[i], key);
		else
			AES128_ECB_decrypt(&in[i], key, &out[i]);
	}
#endif
#endif
	return ret;
}

int bt_lock_close_crypto(void)
{
#ifdef ARCH_CC2640

	if (crypto_hdl != NULL) {
		CryptoCC26XX_close(crypto_hdl);
		crypto_hdl = NULL;
	}
#endif
}

void crypto_test(void)
{
	uint8 key[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
	uint8 plain_text[] = {0x11, 0x22, 0x11, 0x22, 0x11, 0x22, 0x11, 0x22, 
							0x11, 0x22, 0x11, 0x22, 0x11, 0x22, 0x11, 0x22,
							0x11, 0x22, 0x11, 0x22, 0x11, 0x22, 0x11, 0x22,
							0x11, 0x22, 0x11
#if 1
							, 0x01, 0x02, 0x03, 0x04, 0x05,
							0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
#endif
							};
	uint8 cipher_text[ALIGN_X(sizeof(plain_text), 27)/27*4 + sizeof(plain_text)];
	uint8 decrypt_text[sizeof(cipher_text)];

	memset(cipher_text, 0, sizeof(cipher_text));
	memset(decrypt_text, 0, sizeof(decrypt_text));
	bt_lock_crypto(key, plain_text, cipher_text, sizeof(plain_text), 1);
	//DBG_SOFT_PrintfHex(cipher_text, sizeof(cipher_text));
	bt_lock_crypto(key, cipher_text, decrypt_text, sizeof(cipher_text), 0);
	//DBG_SOFT_PrintfHex(decrypt_text, sizeof(decrypt_text));
}

#if 0
const uint8 flash_var[] = {0x01, 0x02, 0x03, 0x04};
void flash_test(void)
{
	uint8 flash_var_new[4];

	DBG_PRINTF("org:\n");
	DBG_SOFT_PrintfHex(flash_var, sizeof(flash_var));
	
	GenRand(flash_var_new, sizeof(flash_var_new));
//	CyBLE_Nvram_Erase(flash_var, sizeof(flash_var));
	CyBLE_Nvram_Write(flash_var_new, flash_var, sizeof(flash_var_new));

	DBG_PRINTF("modified:\n");
	DBG_SOFT_PrintfHex(flash_var, sizeof(flash_var));
}
#endif
#endif

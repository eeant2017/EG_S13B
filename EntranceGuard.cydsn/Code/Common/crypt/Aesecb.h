#ifndef __BT_LOCK_CRYPT_H__
#define __BT_LOCK_CRYPT_H__

#include <string.h>
#include "align.h"

int bt_lock_open_crypto(void);
int bt_lock_crypto(uint8_t *key, uint8_t *in, uint8_t *out, uint32_t len, uint8_t enc_dec);
int bt_lock_close_crypto(void);

#define CALC_AES_CCM_CRYPT_LEN(x) (((ROUND_X(x, 27)/27)*4) + (x))

#endif

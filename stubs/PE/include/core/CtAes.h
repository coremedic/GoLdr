#ifndef CTAES_H
#define CTAES_H
#include <Windows.h>
#include <stdint.h>

typedef struct {
    uint16_t slice[8];
} AES_state;

typedef struct {
    AES_state rk[15];
} AES256_ctx;

typedef struct {
    AES256_ctx ctx;
    uint8_t iv[16];
} AES256_CBC_ctx;


BOOL AESDecrypt(IN PBYTE pEncryptedBuffer, IN SIZE_T sEncryptedBufferSize, IN PBYTE pAESKey, IN PBYTE pAESIV, OUT PBYTE* ppDecryptedBuffer);


#endif //CTAES_H

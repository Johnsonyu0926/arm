#ifndef _CORE_SRC_BASE_TRANSMIT_MQTT_MQTTIOTECDH_H_
#define _CORE_SRC_BASE_TRANSMIT_MQTT_MQTTIOTECDH_H_

#include "mbedtls/ecdh.h"
#include "HPR/HPR_Hpr.h"

#define OTAP_MASTER_KEY_LEN                 48
#define EZDEV_SDK_ECDH_KEY_LEN              65
#define EZDEV_SDK_ECDH_PUBLICKEY_LEN        0x41
#define EZDEV_SDK_MD5_LEN                   32
#define EZDEV_SDK_SHA256_LEN                32
#define EZDEV_SDK_SHA256_HEX_LEN            64
#define EZDEV_SDK_SHA256_OFFSET             10
#define EZDEV_SDK_TOTAL_LEN                 128
#define EZDEV_SDK_SHAREKEY_LEN              32
#define EZDEV_SDK_SHAREKEY_SALT             "www.88075998.com"
#define EZDEV_SDK_PBKDF2_HMAC_TIMES         3
#define EZDEV_SDK_MASTERKEY_LEN             16
#define OTAP_SIGNATURE_LEN                  32
#define OTAP_CHACHA_SECRET_LEN              32
#define OTAP_CHACHA_NONCE_LEN               12

#define PUT_UINT32_BE(n,b,i)                \
{                                           \
    (b)[(i)    ] = static_cast<unsigned char>((n) >> 24);   \
    (b)[(i) + 1] = static_cast<unsigned char>((n) >> 16);   \
    (b)[(i) + 2] = static_cast<unsigned char>((n) >>  8);   \
    (b)[(i) + 3] = static_cast<unsigned char>((n)      );   \
}

#ifdef __cplusplus
#ifdef NETSDK_EXPORTS
#define NET_OTAP_ENCRYPT_CLASS __declspec(dllexport)
#else
#define NET_OTAP_ENCRYPT_CLASS
#endif // !NETSDK_EXPORTS
#else
#define NET_OTAP_ENCRYPT_CLASS
#endif // !__cplusplus

namespace DevEBase {

    struct rnd_pseudo_info {
        uint32_t key[16];
        uint32_t v0, v1;
    };

    class NET_OTAP_ENCRYPT_CLASS COtapEncypt {
    public:
        COtapEncypt();
        ~COtapEncypt();
        
        void ezdev_mbedtls_ecdh_init(mbedtls_ecdh_context* ctx_client);
        void ezdev_mbedtls_ecdh_free(mbedtls_ecdh_context* ctx_client);
        int32_t ezdev_generate_publickey(mbedtls_ecdh_context* ctx_client, unsigned char* pubkey, uint32_t& pubkey_len);
        int32_t ezdev_generate_publickeyder(unsigned char* pServPBKey, int iServPBLen, unsigned char* pMasterKey, int &pMasterLen, unsigned char* pPBKey, uint32_t &dwPBLen);
        int32_t ezdev_generate_masterkey(mbedtls_ecdh_context* ctx_client, unsigned char* peer_pubkey, uint32_t peer_pubkey_len, unsigned char* masterkey, uint32_t* masterkey_len);
        int32_t generate_sharekey(unsigned char *pPublicKey, int& iPublicKey, char* pAuthCode, char* pSerial, uint8_t byUppe);
        int32_t aes_gcm_128_enc_padding(const unsigned char gcm_key[16], unsigned char *input_buf, uint32_t input_length, unsigned char *output_buf, uint32_t& output_length, unsigned char* output_tag_buf, uint32_t tag_buf_len);
        int32_t aes_gcm_128_dec_padding(const unsigned char gcm_key[16], const unsigned char *input_buf, uint32_t input_length, unsigned char *output_buf, uint32_t& output_buf_len, unsigned char* input_tag_buf, uint32_t tag_buf_len);
        int32_t digital_sign_serialize_sha384(unsigned char *out, uint32_t& out_len, unsigned char *sign_src, uint16_t sign_src_len, unsigned char *master_key, uint16_t master_key_len);
        int32_t digital_sign_serialize_and_check_sha384(unsigned char *target_sign, uint16_t target_sign_len, unsigned char *sign_src, uint16_t sign_src_len, unsigned char *master_key, uint16_t master_key_len);
        int32_t AesEncyptData(unsigned char *pEncyptKey, uint32_t dwKeyLen, unsigned char *pInput, unsigned char *pOutput, uint32_t dwLen);
        int32_t AesDecryptData(unsigned char *pDecyptKey, uint32_t dwKeyLen, unsigned char *pInput, unsigned char *pOutput, uint32_t dwLen);
        uint32_t HexStringToByteArray(const uint8_t* pSrc, int nSrcLen, uint8_t* pDst, int nDstLen);
        bool HMAC256(const uint8_t *pSecret, uint32_t dwSecretLen, const uint8_t* pContext, uint32_t dwContextLen, uint8_t bySignature[OTAP_SIGNATURE_LEN]);
        uint32_t Crc32(const unsigned char *buf, uint32_t size);
        bool ChaChaEncypt(uint8_t *pDst, const uint32_t dwDstLen, const uint8_t *pSrc, const uint32_t dwSrcLen, const uint8_t bySecret[OTAP_CHACHA_SECRET_LEN], const uint8_t byNonce[OTAP_CHACHA_NONCE_LEN]);
    };

}

#endif // _CORE_SRC_BASE_TRANSMIT_MQTT_MQTTIOTECDH_H_

#ifndef _CORE_SRC_BASE_TRANSMIT_MQTT_MQTTIOTECDH_H_
#define _CORE_SRC_BASE_TRANSMIT_MQTT_MQTTIOTECDH_H_

#include "mbedtls/ecdh.h"
#include "HPR/HPR_Hpr.h"
//#include "base_typedef.h"

#define OTAP_MASTER_KEY_LEN                 48
#define ezdev_sdk_ecdh_key_len 65
#define ezdev_sdk_ecdh_publickey_len  0x41

#define ezdev_sdk_md5_len											32		   ///<	md5长度
#define ezdev_sdk_sha256_len                                        32         ///<	sha256数据长度
#define ezdev_sdk_sha256_hex_len                                    64         ///<	sha256数据16进制长度
#define ezdev_sdk_sha256_offset                                     10         ///<	sha256密文偏移值
#define ezdev_sdk_total_len											128		   ///<	用于临时变量的存放
#define ezdev_sdk_sharekey_len										32		   ///<	设备share key 长度
#define ezdev_sdk_sharekey_salt "www.88075998.com"
#define ezdev_sdk_pbkdf2_hmac_times                                 3          ///<	
#define ezdev_sdk_masterkey_len       16     ///< 设备mastekey长度 ,默认是16个字节
#define OTAP_SIGNATURE_LEN                  32
#define OTAP_CHACHA_SECRET_LEN              32
#define OTAP_CHACHA_NONCE_LEN               12

#ifndef PUT_UINT32_BE
#define PUT_UINT32_BE(n,b,i)                            \
{                                                       \
    (b)[(i)    ] = (unsigned char) ( (n) >> 24 );       \
    (b)[(i) + 1] = (unsigned char) ( (n) >> 16 );       \
    (b)[(i) + 2] = (unsigned char) ( (n) >>  8 );       \
    (b)[(i) + 3] = (unsigned char) ( (n)       );       \
}
#endif

#ifdef __cplusplus
#ifdef NETSDK_EXPORTS
#define NET_OTAP_ENCRYPT_CLASS __declspec(dllexport)
#else
#define NET_OTAP_ENCRYPT_CLASS
#endif // !NETSDK_EXPORTS
#else
#define NET_OTAP_ENCRYPT_CLASS
#endif // !__cplusplus




namespace DevEBase
{
    typedef struct
    {
        uint32_t key[16];
        uint32_t v0, v1;
    } rnd_pseudo_info;

    class NET_OTAP_ENCRYPT_CLASS COtapEncypt
    {
    public:
        COtapEncypt();
        ~COtapEncypt();
        void  ezdev_mbedtls_ecdh_init(mbedtls_ecdh_context* ctx_client);
        void  ezdev_mbedtls_ecdh_free(mbedtls_ecdh_context* ctx_client);
        HPR_INT32 ezdev_generate_publickey(mbedtls_ecdh_context* ctx_client, unsigned char* pubkey, HPR_UINT32& pubkey_len);
        HPR_INT32 ezdev_generate_publickeyder(unsigned char* pServPBKey, int iServPBLen, unsigned char* pMasterKey, int &pMasterLen, unsigned char* pPBKey, HPR_UINT32 &dwPBLen);
        HPR_INT32 ezdev_generate_masterkey(mbedtls_ecdh_context* ctx_client, unsigned char* peer_pubkey, HPR_UINT32 peer_pubkey_len, unsigned char* masterkey, HPR_UINT32 *masterkey_len);
        HPR_INT32 generate_sharekey(unsigned char *pPublicKey, HPR_INT32& iPublicKey, char* pAuthCode, char* pSerial, HPR_UINT8 byUppe);

        HPR_INT32 aes_gcm_128_enc_padding(const unsigned char gcm_key[16], \
            unsigned char *input_buf, HPR_UINT32 input_length, \
            unsigned char *output_buf, HPR_UINT32& output_length, \
            unsigned char* output_tag_buf, HPR_UINT32 tag_buf_len);
        HPR_INT32 aes_gcm_128_dec_padding(const unsigned char gcm_key[16], \
            const unsigned char *input_buf, HPR_UINT32 input_length, \
            unsigned char *output_buf, HPR_UINT32& output_buf_len, \
            unsigned char* input_tag_buf, HPR_UINT32 tag_buf_len);

        HPR_INT32 ezdev_generate_masterkey(mbedtls_ecdh_context* ctx_client, unsigned char* peer_pubkey, HPR_UINT32 peer_pubkey_len, \
            unsigned char* masterkey, HPR_UINT32& masterkey_len);

        HPR_INT32 digital_sign_serialize_sha384(unsigned char *out, HPR_UINT32& out_len, unsigned char *sign_src, HPR_UINT16 sign_src_len, \
            unsigned char *master_key, HPR_UINT16 master_key_len);
        HPR_INT32 digital_sign_serialize_and_check_sha384(unsigned char *target_sign, HPR_UINT16 target_sign_len, \
            unsigned char *sign_src, HPR_UINT16 sign_src_len, unsigned char *master_key, HPR_UINT16 master_key_len);

        HPR_INT32 AesEncyptData(unsigned char *pEncyptKey, HPR_UINT32 dwKeyLen, unsigned char *pInput, unsigned char *pOutput, HPR_UINT32 dwLen);
        HPR_INT32 AesDecryptData(unsigned char *pDecyptKey, HPR_UINT32 dwKeyLen, unsigned char *pInput, unsigned char *pOutput, HPR_UINT32 dwLen);

        HPR_UINT32 HexStringToByteArray(const HPR_UINT8* pSrc, int nSrcLen, HPR_UINT8* pDst, int nDstLen);

        HPR_BOOL HMAC256(const HPR_UINT8 *pSecret, HPR_UINT32 dwSecretLen, const HPR_UINT8* pContext, HPR_UINT32 dwContextLen, HPR_UINT8 bySignature[OTAP_SIGNATURE_LEN]);
        HPR_UINT32 Crc32(const unsigned char *buf, HPR_UINT32 size);
        HPR_BOOL ChaChaEncypt(HPR_UINT8 *pDst, const HPR_UINT32 dwDstLen, const HPR_UINT8 *pSrc, const HPR_UINT32 dwSrcLen, \
            const HPR_UINT8 bySecret[OTAP_CHACHA_SECRET_LEN], const HPR_UINT8 byNonce[OTAP_CHACHA_NONCE_LEN]);
    };

}
#endif //H_EZDEV_ECDH_SUPPORT_H_
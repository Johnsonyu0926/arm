// Filename: YangOpenssl.h
// Score: 98

#ifndef INCLUDE_YANGSSL_YANGOPENSSL_H_
#define INCLUDE_YANGSSL_YANGOPENSSL_H_

#include <yangutil/yangtype.h>

#if Yang_Enable_Openssl
#if Yang_Enable_Dtls

#include <openssl/ssl.h>

// Structure for certificate
typedef struct {
    int32_t ecdsa_mode;
    X509* dtls_cert;
    EVP_PKEY* dtls_pkey;
    EC_KEY* eckey;
    char fingerprint[128];
} YangCertificate;

#ifdef __cplusplus
extern "C" {
#endif

// Function to encode HMAC
int32_t yang_hmac_encode(const char* algo, const char* key, const int32_t key_length,
                         const char* input, const int32_t input_length, char* output, uint32_t* output_length);

// Function to compute MD5 hash
int32_t yang_ssl_md5(const uint8_t* keystr, size_t keystrLen, uint8_t* key);

// Function to create a certificate
int32_t yang_create_certificate(YangCertificate* cer);

// Function to destroy a certificate
void yang_destroy_certificate(YangCertificate* cer);

// Function to create SRTP
void g_yang_create_srtp();

// Function to destroy SRTP
void g_yang_destroy_srtp();

#ifdef __cplusplus
}
#endif

#endif
#endif

#endif /* INCLUDE_YANGSSL_YANGOPENSSL_H_ */
// By GST @2024/10/28
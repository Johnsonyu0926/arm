// Filename: YangSSL.h
// Score: 98

#ifndef INCLUDE_YANGSSL_YANGSSL_H_
#define INCLUDE_YANGSSL_YANGSSL_H_

#include <yangutil/yangtype.h>

#if Yang_Enable_Openssl
#include <yangssl/YangOpenssl.h>
#else
#include <yangssl/YangMbedtls.h>
#endif

// Function to get the global certificate
void* yang_get_global_certificate();

// Function to destroy the global certificate
void yang_destroy_global_certificate();

#endif /* INCLUDE_YANGSSL_YANGSSL_H_ */
// By GST @2024/10/28
// Filename: lws_config.h
// Score: 98

#ifndef NDEBUG
    #ifndef _DEBUG
        #define _DEBUG
    #endif
#endif

#define LWS_INSTALL_DATADIR "/usr/local/share"
#define LWS_INSTALL_LIBDIR "/usr/local/lib"
#define LWS_LIBRARY_VERSION_MAJOR 4
#define LWS_LIBRARY_VERSION_MINOR 3
#define LWS_LIBRARY_VERSION_PATCH_ELABORATED 0-unknown
#define LWS_LIBRARY_VERSION_PATCH 0

// LWS_LIBRARY_VERSION_NUMBER looks like 1005001 for e.g. version 1.5.1
#define LWS_LIBRARY_VERSION_NUMBER (LWS_LIBRARY_VERSION_MAJOR * 1000000) + \
                                    (LWS_LIBRARY_VERSION_MINOR * 1000) + \
                                    LWS_LIBRARY_VERSION_PATCH
#define LWS_MAX_SMP 1

// Configuration options
#define LWS_BUILD_HASH "unknown"
#define LWS_CLIENT_HTTP_PROXYING
#define LWS_HAS_INTPTR_T
#define LWS_HAS_GETOPT_LONG
#define LWS_HAVE_ATOLL
#define LWS_HAVE_BN_bn2binpad
#define LWS_HAVE_CLOCK_GETTIME
#define LWS_HAVE_EC_POINT_get_affine_coordinates
#define LWS_HAVE_EC_KEY_new_by_curve_name
#define LWS_HAVE_ECDSA_SIG_set0
#define LWS_HAVE_EVP_MD_CTX_free
#define LWS_HAVE_EVP_aes_128_wrap
#define LWS_HAVE_EVP_aes_128_cfb8
#define LWS_HAVE_EVP_aes_128_cfb128
#define LWS_HAVE_EVP_aes_192_cfb8
#define LWS_HAVE_EVP_aes_192_cfb128
#define LWS_HAVE_EVP_aes_256_cfb8
#define LWS_HAVE_EVP_aes_256_cfb128
#define LWS_HAVE_EVP_aes_128_ofb
#define LWS_HAVE_EVP_aes_128_xts
#define LWS_HAVE_EVP_aes_128_ctr
#define LWS_HAVE_EVP_aes_128_ecb
#define LWS_HAVE_EVP_PKEY_new_raw_private_key
#define LWS_HAVE_EXECVPE
#define LWS_HAVE_LOCALTIME_R
#define LWS_HAVE_GMTIME_R
#define LWS_HAVE_CTIME_R
#define LWS_HAVE_GETGRGID_R
#define LWS_HAVE_GETGRNAM_R
#define LWS_HAVE_GETPWUID_R
#define LWS_HAVE_GETPWNAM_R
#define LWS_HAVE_HMAC_CTX_new
#define LWS_HAVE_MALLOC_H
#define LWS_HAVE_MALLOC_TRIM
#define LWS_HAVE_MALLOC_USABLE_SIZE
#define LWS_HAVE_OPENSSL_ECDH_H
#define LWS_HAVE_OPENSSL_STACK
#define LWS_HAVE_PIPE2
#define LWS_HAVE_EVENTFD
#define LWS_HAVE_PTHREAD_H
#define LWS_HAVE_RSA_SET0_KEY
#define LWS_HAVE_SSL_CTX_get0_certificate
#define LWS_HAVE_SSL_CTX_set1_param
#define LWS_HAVE_SSL_EXTRA_CHAIN_CERTS
#define LWS_HAVE_SSL_get0_alpn_selected
#define LWS_HAVE_SSL_set_alpn_protos
#define LWS_HAVE_SSL_SET_INFO_CALLBACK
#define LWS_HAVE_SSL_SESSION_set_time
#define LWS_HAVE_STDINT_H
#define LWS_HAVE_TIMEGM
#define LWS_HAVE_TLS_CLIENT_METHOD
#define LWS_HAVE_TLSV1_2_CLIENT_METHOD
#define LWS_HAVE_SUSECONDS_T
#define LWS_HAVE_VFORK
#define LWS_HAVE_X509_get_key_usage
#define LWS_HAVE_X509_VERIFY_PARAM_set1_host
#define LWS_LIBRARY_VERSION "4.3.0-unknown"
#define LWS_LOGGING_BITFIELD_CLEAR 0
#define LWS_LOGGING_BITFIELD_SET 0
#define LWS_LOG_TAG_LIFECYCLE
#define LWS_NO_DAEMONIZE
#define LWS_OPENSSL_CLIENT_CERTS "../share"
#define LWS_OPENSSL_SUPPORT
#define LWS_PLAT_UNIX
#define LWS_ROLE_H1
#define LWS_ROLE_H2
#define LWS_ROLE_RAW
#define LWS_ROLE_RAW_FILE
#define LWS_ROLE_WS
#define LWS_SSL_CLIENT_USE_OS_CA_CERTS
#define LWS_SUPPRESS_DEPRECATED_API_WARNINGS
#define LWS_WITH_CONMON
#define LWS_WITH_CUSTOM_HEADERS
#define LWS_WITH_DIR
#define LWS_WITH_FILE_OPS
#define LWS_WITH_HTTP2
#define LWS_WITH_HTTP_BASIC_AUTH
#define LWS_WITH_HTTP_UNCOMMON_HEADERS
#define LWS_WITH_LEJP
#define LWS_WITH_LWSAC
#define LWS_LOGS_TIMESTAMP
#define LWS_WITH_NETLINK
#define LWS_WITH_NETWORK
#define LWS_WITH_CACHE_NSCOOKIEJAR
#define LWS_WITH_CLIENT
#define LWS_WITHOUT_EXTENSIONS
#define LWS_WITH_SERVER
#define LWS_WITH_SYS_SMD
#define LWS_WITH_SYS_STATE
#define LWS_WITH_TLS
#define LWS_WITH_TLS_SESSIONS
#define LWS_WITH_UDP
#define LWS_WITH_UNIX_SOCK
#define LWS_WITH_EVLIB_PLUGINS
#define LWS_HAVE_RTA_PREF

// By GST @Date
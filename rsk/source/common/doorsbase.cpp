#include <fcntl.h>
#include <stdarg.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include <cstdio>
#include <cassert>
#include <ctime>
#include <netinet/if_ether.h>
#include <net/if_arp.h>
#include <cstring>
#include <cerrno>
#include <utmp.h>
#include <sys/stat.h>
#include <cstdlib>
#include <openssl/rsa.h>
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "doorsdefs.h"
#include "doorsbase.h"

#define CHK_NULL(x) if ((x) == nullptr) exit(1)
#define CHK_ERR(err, s) if ((err) == -1) { perror(s); exit(1); }
#define CHK_SSL(err) if ((err) == -1) { ERR_print_errors_fp(stderr); exit(2); }

#define HOME "./"
#define CERTF HOME "foo-cert.pem"
#define KEYF HOME "foo-cert.pem"

CConfig::CConfig(const char *szFile) : m_pFp(nullptr), m_szFile(szFile ? szFile : "") {}

CConfig::~CConfig() = default;

int CConfig::Read(const char *szSess, const char *szItem, char *szValue, const char *szDefault) {
    m_pFp = fopen(m_szFile.c_str(), "r");
    if (m_pFp == nullptr) {
        strncpy(szValue, szDefault, strlen(szDefault) + 1);
        return 0;
    }

    std::string szSess1 = "[" + std::string(szSess) + "]";
    bool bEnterSess = false;
    char szLine[256];

    while (fgets(szLine, 255, m_pFp)) {
        if (!bEnterSess) {
            if (strncasecmp(szLine, szSess1.c_str(), szSess1.length()) == 0)
                bEnterSess = true;
        } else {
            if (szLine[0] == '[') {
                break;
            }
            if (strncasecmp(szLine, szItem, strlen(szItem)) == 0) {
                char *p = strchr(szLine, '=');
                if (!p) return 0;
                p++;
                while (*p == ' ') p++;
                char *p2 = strchr(p, ';');
                if (!p2) {
                    return 0;
                }
                strncpy(szValue, p, p2 - p);
                szValue[p2 - p] = '\0';
                fclose(m_pFp);
                m_pFp = nullptr;
                return 1;
            }
        }
    }
    strncpy(szValue, szDefault, strlen(szDefault) + 1);
    fclose(m_pFp);
    m_pFp = nullptr;
    return 0;
}

bool CSocket::GetMyIp(char *szBuf) {
    sockaddr_in sockaddr;
    bzero(&sockaddr, sizeof(sockaddr));
    unsigned int len;
    if (getpeername(m_hSocket, (sockaddr *)&sockaddr, &len) < 0) {
        return false;
    }
    strncpy(szBuf, inet_ntoa(sockaddr.sin_addr));
    return true;
}

CSocket::CSocket() : m_hSocket(INVALID_SOCKET), m_bConnected(false) {}

CSocket::CSocket(SOCKET s) : m_hSocket(s), m_bConnected(true) {}

CSocket::~CSocket() {
    Close();
}

void CSocket::SetSocket(SOCKET s) {
    m_hSocket = s;
    m_bConnected = true;
}

int CSocket::ConnectionReset() {
    m_bConnected = false;
    return 1;
}

bool CSocket::Create(SOCKTYPE nSockType) {
    switch (nSockType) {
        case SOCKTYPE::TCP:
            m_hSocket = socket(AF_INET, SOCK_STREAM, 0);
            break;
        case SOCKTYPE::UDP:
            m_hSocket = socket(AF_INET, SOCK_DGRAM, 0);
            break;
        case SOCKTYPE::ARP:
            m_hSocket = socket(PF_PACKET, SOCK_PACKET, htons(ETH_P_ARP));
            if (m_hSocket > 0) {
                int optval = 1;
                if (setsockopt(m_hSocket, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) == -1) {
                    std::cerr << "Could not setsocketopt on raw socket\n";
                    close(m_hSocket);
                    return false;
                }
            }
            break;
        case SOCKTYPE::ALL:
            m_hSocket = socket(PF_PACKET, SOCK_PACKET, htons(ETH_P_ALL));
            if (m_hSocket > 0) {
                int optval = 1;
                if (setsockopt(m_hSocket, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) == -1) {
                    std::cerr << "Could not setsocketopt on raw socket\n";
                    close(m_hSocket);
                    return false;
                }
            }
            break;
        default:
            return false;
    }
    return (m_hSocket != INVALID_SOCKET);
}

bool CSocket::Close() {
    if (m_hSocket != INVALID_SOCKET) {
        close(m_hSocket);
        m_hSocket = INVALID_SOCKET;
    }
    return true;
}

bool CSocket::Connect(const std::string& szServAddr, int nServPort) {
    struct sockaddr_in servAddr{};
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(nServPort);
    servAddr.sin_addr.s_addr = inet_addr(szServAddr.c_str());

    if (connect(m_hSocket, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        return false;
    }
    return true;
}

bool CSocket::Connect(uint32_t dwIp, int nServPort) {
    struct sockaddr_in servAddr{};
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(nServPort);
    servAddr.sin_addr.s_addr = dwIp;

    if (connect(m_hSocket, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        return false;
    }
    return true;
}

bool CSocket::ConnectTimeOut(const std::string& szHost, uint16_t nPort, long timeout) {
    struct sockaddr_in servAddr{};
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(nPort);

    struct hostent* pHost = gethostbyname(szHost.c_str());
    if (pHost == nullptr) {
        return false;
    }
    memcpy(&servAddr.sin_addr, pHost->h_addr, pHost->h_length);

    fcntl(m_hSocket, F_SETFL, O_NONBLOCK);
    connect(m_hSocket, (struct sockaddr*)&servAddr, sizeof(servAddr));

    fd_set set;
    struct timeval tv{};
    FD_ZERO(&set);
    FD_SET(m_hSocket, &set);
    tv.tv_sec = timeout;
    tv.tv_usec = 0;

    if (select(m_hSocket + 1, nullptr, &set, nullptr, &tv) > 0) {
        int valopt;
        socklen_t lon = sizeof(int);
        getsockopt(m_hSocket, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon);
        if (valopt) {
            return false;
        }
    } else {
        return false;
    }

    fcntl(m_hSocket, F_SETFL, 0);
    return true;
}

CSThread::CSThread() : m_bAutoDelete(false), m_bJoinable(false), m_threadId(0) {}

CSThread::~CSThread() {
    if (m_bJoinable && m_threadId) {
        pthread_detach(m_threadId);
    }
}

bool CSThread::SetAutoDelete(bool b) {
    m_bAutoDelete = b;
    return true;
}

void CSThread::SetJoinable(bool b) {
    m_bJoinable = b;
}

bool CSThread::CreateThread() {
    return pthread_create(&m_threadId, nullptr, ThreadFunc, this) == 0;
}

void CSThread::WaitForEnd() {
    if (m_bJoinable && m_threadId) {
        pthread_join(m_threadId, nullptr);
    }
}

void* CSThread::ThreadFunc(void* arg) {
    auto* pThread = static_cast<CSThread*>(arg);
    pThread->Run();
    if (pThread->m_bAutoDelete) {
        delete pThread;
    }
    return nullptr;
}

CMutex::CMutex() {
    pthread_mutex_init(&m_mutex, nullptr);
}

CMutex::~CMutex() {
    pthread_mutex_destroy(&m_mutex);
}

void CMutex::Lock() {
    pthread_mutex_lock(&m_mutex);
}

void CMutex::Unlock() {
    pthread_mutex_unlock(&m_mutex);
}

CEvent::CEvent() : m_bSignaled(false) {
    pthread_cond_init(&m_cond, nullptr);
    pthread_mutex_init(&m_mutex, nullptr);
}

CEvent::~CEvent() {
    pthread_cond_destroy(&m_cond);
    pthread_mutex_destroy(&m_mutex);
}

bool CEvent::Wait() {
    pthread_mutex_lock(&m_mutex);
    while (!m_bSignaled) {
        pthread_cond_wait(&m_cond, &m_mutex);
    }
    m_bSignaled = false;
    pthread_mutex_unlock(&m_mutex);
    return true;
}

void CEvent::Signal() {
    pthread_mutex_lock(&m_mutex);
    m_bSignaled = true;
    pthread_cond_signal(&m_cond);
    pthread_mutex_unlock(&m_mutex);
}

void CEvent::Reset() {
    pthread_mutex_lock(&m_mutex);
    m_bSignaled = false;
    pthread_mutex_unlock(&m_mutex);
}

CIniFile::CIniFile() = default;

CIniFile::~CIniFile() = default;

bool CIniFile::Load(const std::string& filename) {
    m_filename = filename;
    std::ifstream file(m_filename);
    if (!file.is_open()) {
        return false;
    }

    std::string line, section;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == ';') {
            continue;
        }
        if (line[0] == '[') {
            section = line.substr(1, line.find(']') - 1);
            continue;
        }
        auto delimiterPos = line.find('=');
        auto key = line.substr(0, delimiterPos);
        auto value = line.substr(delimiterPos + 1);
        m_data[section][key] = value;
    }
    file.close();
    return true;
}

bool CIniFile::Save(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    for (const auto& section : m_data) {
        file << "[" << section.first << "]\n";
        for (const auto& kv : section.second) {
            file << kv.first << "=" << kv.second << "\n";
        }
        file << "\n";
    }
    file.close();
    return true;
}

std::string CIniFile::GetValue(const std::string& section, const std::string& key) {
    return m_data[section][key];
}

void CIniFile::SetValue(const std::string& section, const std::string& key, const std::string& value) {
    m_data[section][key] = value;
}

CDatabase::CDatabase() = default;

CDatabase::~CDatabase() = default;

bool CDatabase::Connect(const std::string& connectionString) {
    // Implementation for connecting to the database
    return true;
}

void CDatabase::Disconnect() {
    // Implementation for disconnecting from the database
}

bool CDatabase::ExecuteQuery(const std::string& query) {
    // Implementation for executing a query
    return true;
}

std::vector<std::string> CDatabase::FetchResults() {
    // Implementation for fetching results from the database
    return {};
}

class CDatabase::Impl {
    // Private implementation details for CDatabase
};

void handle_error(const char* file, int line, const char* msg) {
    fprintf(stderr, "Error at %s:%d: %s\n", file, line, msg);
    exit(EXIT_FAILURE);
}

#define HANDLE_ERROR(msg) handle_error(__FILE__, __LINE__, msg)

void InitOpenSSL() {
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
}

void CleanupOpenSSL() {
    EVP_cleanup();
}

SSL_CTX* create_context() {
    const SSL_METHOD* method;
    SSL_CTX* ctx;

    method = SSLv23_client_method();

    ctx = SSL_CTX_new(method);
    if (!ctx) {
        HANDLE_ERROR("Unable to create SSL context");
    }

    return ctx;
}

void configure_context(SSL_CTX* ctx) {
    SSL_CTX_set_ecdh_auto(ctx, 1);

    // Set the key and cert
    if (SSL_CTX_use_certificate_file(ctx, CERTF, SSL_FILETYPE_PEM) <= 0) {
        HANDLE_ERROR("Unable to set certificate file");
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, KEYF, SSL_FILETYPE_PEM) <= 0) {
        HANDLE_ERROR("Unable to set private key file");
    }
}

void ShowCerts(SSL* ssl) {
    X509* cert;
    char* line;

    cert = SSL_get_peer_certificate(ssl);
    if (cert != nullptr) {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        free(line);
        X509_free(cert);
    } else {
        printf("No certificates.\n");
    }
}

void PerformSSLHandshake(SSL* ssl) {
    if (SSL_connect(ssl) <= 0) {
        HANDLE_ERROR("SSL connect error");
    }

    printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
    ShowCerts(ssl);
}

void SendData(SSL* ssl, const std::string& data) {
    if (SSL_write(ssl, data.c_str(), data.length()) <= 0) {
        HANDLE_ERROR("SSL write error");
    }
}

std::string ReceiveData(SSL* ssl) {
    char buf[1024];
    int bytes;
    std::string data;

    do {
        bytes = SSL_read(ssl, buf, sizeof(buf));
        if (bytes > 0) {
            data.append(buf, bytes);
        }
    } while (bytes > 0);

    if (bytes < 0 && SSL_get_error(ssl, bytes) != SSL_ERROR_ZERO_RETURN) {
        HANDLE_ERROR("SSL read error");
    }

    return data;
}

void CleanupSSL(SSL* ssl, SSL_CTX* ctx) {
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    CleanupOpenSSL();
}

int main() {
    InitOpenSSL();
    SSL_CTX* ctx = create_context();

    configure_context(ctx);

    int server = socket(AF_INET, SOCK_STREAM, 0);
    if (server < 0) {
        HANDLE_ERROR("Unable to create socket");
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(443);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(server, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        HANDLE_ERROR("Unable to connect");
    }

    SSL* ssl = SSL_new(ctx);
    SSL_set_fd(ssl, server);

    PerformSSLHandshake(ssl);

    SendData(ssl, "Hello, SSL!\n");
    std::string received = ReceiveData(ssl);
    std::cout << "Received: " << received << std::endl;

    CleanupSSL(ssl, ctx);
    close(server);

    return 0;
}

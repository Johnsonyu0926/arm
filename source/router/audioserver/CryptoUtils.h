#ifndef __CRYPTOUTILS_H__
#define __CRYPTOUTILS_H__

#include <string>
#include <vector>

class CryptoUtils {
public:
    static std::vector<unsigned char> encrypt(const std::string& plaintext, const std::string& key);
    static std::string decrypt(const std::vector<unsigned char>& ciphertext, const std::string& key);
};

#endif // __CRYPTOUTILS_H__
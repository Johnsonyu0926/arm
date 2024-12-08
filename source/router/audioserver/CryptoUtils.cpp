#include "CryptoUtils.h"
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <stdexcept>

std::vector<unsigned char> CryptoUtils::encrypt(const std::string& plaintext, const std::string& key) {
    std::vector<unsigned char> ciphertext(plaintext.size() + AES_BLOCK_SIZE);
    std::vector<unsigned char> iv(AES_BLOCK_SIZE);

    if (!RAND_bytes(iv.data(), AES_BLOCK_SIZE)) {
        throw std::runtime_error("Failed to generate IV");
    }

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Failed to create EVP_CIPHER_CTX");
    }

    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, reinterpret_cast<const unsigned char*>(key.data()), iv.data()) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to initialize encryption");
    }

    int len;
    if (EVP_EncryptUpdate(ctx, ciphertext.data(), &len, reinterpret_cast<const unsigned char*>(plaintext.data()), plaintext.size()) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to encrypt data");
    }

    int ciphertext_len = len;
    if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to finalize encryption");
    }
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    ciphertext.resize(ciphertext_len);
    ciphertext.insert(ciphertext.begin(), iv.begin(), iv.end());

    return ciphertext;
}

std::string CryptoUtils::decrypt(const std::vector<unsigned char>& ciphertext, const std::string& key) {
    std::vector<unsigned char> iv(ciphertext.begin(), ciphertext.begin() + AES_BLOCK_SIZE);
    std::vector<unsigned char> encrypted_data(ciphertext.begin() + AES_BLOCK_SIZE, ciphertext.end());

    std::vector<unsigned char> plaintext(encrypted_data.size());

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Failed to create EVP_CIPHER_CTX");
    }

    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, reinterpret_cast<const unsigned char*>(key.data()), iv.data()) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to initialize decryption");
    }

    int len;
    if (EVP_DecryptUpdate(ctx, plaintext.data(), &len, encrypted_data.data(), encrypted_data.size()) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to decrypt data");
    }

    int plaintext_len = len;
    if (EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to finalize decryption");
    }
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    plaintext.resize(plaintext_len);

    return std::string(plaintext.begin(), plaintext.end());
}
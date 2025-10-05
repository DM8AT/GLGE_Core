/**
 * @file Encryption.cpp
 * @author DM8AT
 * @brief implement the encryption stuff
 * @version 0.1
 * @date 2025-10-04
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//include the encryption API
#include "Encryption.h"
//include OpenSSL for encryption / decryption stuff
#include <openssl/evp.h>

void glge_Encrypt(String* string, void* cryptoInfo)
{
    //map the crypto info structure
    CryptoInfo* cInfo = (CryptoInfo*)cryptoInfo;
    //get the length of the stored string
    uint64_t key_1_len = cInfo->key_1->size();
    uint64_t key_2_len = cInfo->key_2->size();
    //store the output
    String output;

    //check the key length
    if ((key_1_len != 16) && (key_1_len != 24) && (key_1_len != 32)) {
        //invalid length for key 1
        printf("The length %lu is not a valid length for key 1. Valid length are: 16 (bytes), 24 (bytes) or 32 (bytes).\n", key_1_len);
        return;
    }
    if ((key_2_len != 16) && (key_2_len != 24) && (key_2_len != 32)) {
        //invalid length for key 1
        printf("The length %lu is not a valid length for key 2. Valid length are: 16 (bytes), 24 (bytes) or 32 (bytes).\n", key_1_len);
        return;
    }
    //key length must be the same
    if (key_1_len != key_2_len) {
        printf("Both keys must have the same length\n");
        return;
    }

    //create the new cipher
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        printf("General OpenSSL error - ctx not created\n");
        return;
    }

    //select the correct cypher
    const EVP_CIPHER* cipher = (key_1_len == 16) ? EVP_aes_128_ecb() : ((key_1_len == 24) ? EVP_aes_192_ecb() : EVP_aes_256_ecb());
    //initialize the context for encryption
    if (EVP_EncryptInit_ex(ctx, cipher, NULL, (const uchar*)cInfo->key_1->c_str(), (const uchar*)cInfo->key_2->c_str()) != 1) {
        //error
        printf("Failed to initialize the encryption\n");
        EVP_CIPHER_CTX_free(ctx);
        return;
    }

    //enable padding
    EVP_CIPHER_CTX_set_padding(ctx, 1);

    //prepare the padded output buffer
    uint64_t maxOut = string->size() + EVP_CIPHER_block_size(cipher);
    uchar* out = new uchar[maxOut];
    if (!out) {
        //error
        printf("Failed to allocate the output buffer\n");
        EVP_CIPHER_CTX_free(ctx);
        return;
    }
    //store the outp length
    int outl_1 = 0;
    int outl_2 = 0;

    //update the cipher with the input
    if (EVP_EncryptUpdate(ctx, out, &outl_1, (const uchar*)string->c_str(), string->size()) != 1) {
        //error
        printf("EVP_EncryptUpdate failed\n");
        delete[] out;
        EVP_CIPHER_CTX_free(ctx);
        return;
    }

    //finalize the encryption
    if (EVP_EncryptFinal_ex(ctx, out + outl_1, &outl_2) != 1) {
        //error
        printf("EVP_EncryptFinal_ex failed\n");
        delete[] out;
        EVP_CIPHER_CTX_free(ctx);
        return;
    }

    //construct the encrypted output string
    output.assign((const char*)out, outl_1 + outl_2);

    //store the result back into the original string
    *string = output;

    //cleanup
    delete[] out;
    EVP_CIPHER_CTX_free(ctx);
}

void glge_Decrypt(String* string, void* cryptoInfo)
{
    //map the crypto info structure
    CryptoInfo* cInfo = (CryptoInfo*)cryptoInfo;
    //get the length of the stored string
    uint64_t key_1_len = cInfo->key_1->size();
    uint64_t key_2_len = cInfo->key_2->size();
    //store the output
    String output;

    //check the key length
    if ((key_1_len != 16) && (key_1_len != 24) && (key_1_len != 32)) {
        //invalid length for key 1
        printf("The length %lu is not a valid length for key 1. Valid length are: 16 (bytes), 24 (bytes) or 32 (bytes).\n", key_1_len);
        return;
    }
    if ((key_2_len != 16) && (key_2_len != 24) && (key_2_len != 32)) {
        //invalid length for key 2
        printf("The length %lu is not a valid length for key 2. Valid length are: 16 (bytes), 24 (bytes) or 32 (bytes).\n", key_2_len);
        return;
    }
    //key length must be the same
    if (key_1_len != key_2_len) {
        printf("Both keys must have the same length\n");
        return;
    }

    //create the new cipher
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        printf("General OpenSSL error - ctx not created\n");
        return;
    }

    //select the correct cipher
    const EVP_CIPHER* cipher = (key_1_len == 16) ? EVP_aes_128_ecb() : ((key_1_len == 24) ? EVP_aes_192_ecb() : EVP_aes_256_ecb());
    //initialize the context for decryption
    if (EVP_DecryptInit_ex(ctx, cipher, NULL, (const uchar*)cInfo->key_1->c_str(), (const uchar*)cInfo->key_2->c_str()) != 1) {
        //error
        printf("Failed to initialize the decryption\n");
        EVP_CIPHER_CTX_free(ctx);
        return;
    }

    //enable padding
    EVP_CIPHER_CTX_set_padding(ctx, 1);

    //prepare the output buffer
    uint64_t maxOut = string->size();
    uchar* out = new uchar[maxOut];
    if (!out) {
        //error
        printf("Failed to allocate the output buffer\n");
        EVP_CIPHER_CTX_free(ctx);
        return;
    }

    //store the output length
    int outl_1 = 0;
    int outl_2 = 0;

    //update the cipher with the input
    if (EVP_DecryptUpdate(ctx, out, &outl_1, (const uchar*)string->c_str(), string->size()) != 1) {
        //error
        printf("EVP_DecryptUpdate failed\n");
        delete[] out;
        EVP_CIPHER_CTX_free(ctx);
        return;
    }

    //finalize the decryption
    if (EVP_DecryptFinal_ex(ctx, out + outl_1, &outl_2) != 1) {
        //error
        printf("EVP_DecryptFinal_ex failed - possible incorrect key or corrupted input\n");
        delete[] out;
        EVP_CIPHER_CTX_free(ctx);
        return;
    }

    //construct the decrypted output string
    output.assign((const char*)out, outl_1 + outl_2);

    //store the result back into the original string
    *string = output;

    //cleanup
    delete[] out;
    EVP_CIPHER_CTX_free(ctx);
}
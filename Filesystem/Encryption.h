/**
 * @file Encryption.h
 * @author DM8AT
 * @brief define two functions that are compatable with the file decorator interface to encrypt and decrypt strings / file contents
 * @version 0.1
 * @date 2025-10-04
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_CORE_FILESYSTEM_ENCRYPT_
#define _GLGE_CORE_FILESYSTEM_ENCRYPT_

//include strings
#include "../../GLGE_BG/CBinding/String.h"
//include the types
#include "../Types.h"

/**
 * @brief store information for a cryptographic function
 */
typedef struct s_CryptoInfo {
    //a C-String containing one key for encryption / decryption
    String* key_1;
    //a C-String containing another key for encryption / decryption
    String* key_2;
} CryptoInfo;

void glge_Encrypt(String* string, void* cryptoInfo);

void glge_Decrypt(String* string, void* cryptoInfo);

#endif
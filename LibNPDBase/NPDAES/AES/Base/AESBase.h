#ifndef _AESBASE_H
#define _AESBASE_H

/************* HEADER FILE ****************/
#include <exception>
#include <cstring>
#include <string>
/******************************************/

#define BLOCK_SIZE 16

using namespace std;
 
class AESBase
{
public:
    enum
    {/* Encryption mode */
        ECB = 0, CBC = 1, CFB = 2
    };
private:
    enum
    {
        DEFAULT_BLOCK_SIZE = 16
    };
    enum
    {
        MAX_BLOCK_SIZE = 32, MAX_ROUNDS = 14, MAX_KC = 8, MAX_BC = 8
    };
public:
    AESBase();
    virtual ~AESBase();
private:
    //Key Initialization Flag
    bool m_bKeyInit;
    //Encryption (m_Ke) round key
    int m_Ke[MAX_ROUNDS + 1][MAX_BC];
    //Decryption (m_Kd) round key
    int m_Kd[MAX_ROUNDS + 1][MAX_BC];
    //Key Length
    int m_keylength;
    //Block Size
    int m_blockSize;
    //Number of Rounds
    int m_iROUNDS;
    //Chain Block
    char m_chain0[MAX_BLOCK_SIZE];
    char m_chain[MAX_BLOCK_SIZE];
    //Auxiliary private use buffers
    int tk[MAX_KC];
    int a[MAX_BC];
    int t[MAX_BC];
private:

    /*
    * @function : XOR chain and buffer
    * @param buff : Src and storage results
    * @param chain : Buffer to XOR
    * @return NULL
    */
    void Xor(char* buff, char const* chain);

    /*
    * @function : Convenience method to encrypt exactly one block of 
    *            ciphertext, assuming Rijndael's default block size (128-bit)
    * @param in : The plaintext
    * @param result : The ciphertext generated from a plaintext using the session key
    * @return NULL
    */
    void DefEncryptBlock(char const* in, char* result);

    /*
    * @function : Convenience method to decrypt exactly one block of 
    *            plaintext, assuming Rijndael's default block size (128-bit)
    * @param in : The ciphertext
    * @param result : The plaintext generated from a ciphertext using the session key
    * @return NULL
    */
    void DefDecryptBlock(char const* in, char* result);

    /*
    * @function : Encrypt exactly one block of plaintext
    * @param in : The plaintext
    * @param result : The ciphertext generated from a plaintext using the key
    * @return NULL
    */
    void EncryptBlock(char const* in, char* result);

    /*
    * @function : Decrypt exactly one block of ciphertext
    * @param in : The ciphertext
    * @param result : The plaintext generated from a ciphertext using the session key
    * @return NULL
    */
    void DecryptBlock(char const* in, char* result);
public:

    /*
    * @function : Expand a user-supplied key material into a session key
    * @param key : The 128/192/256-bit user-key to use
    * @param chain : Initial chain block for CBC and CFB modes
    * @param keylength : 16, 24 or 32 bytes
    * @param blockSize : The block size in bytes of this Rijndael (16, 24 or 32 bytes)
    * @return NULL
    */
    void MakeKey(char const* key, char const* chain, int keylength =
            DEFAULT_BLOCK_SIZE, int blockSize = DEFAULT_BLOCK_SIZE);

    /*
    * @function : Encryption
    * @param in : The plaintext
    * @param result : Encrypted string
    * @param n : Block start position
    * @param iMode : Encryption mode
    * @return NULL
    */
    void Encrypt(char const* in, char* result, size_t n, int iMode = ECB);

    /*
    * @function : Decryption
    * @param in : The plaintext
    * @param result : Decrypted string
    * @param n : Block start position
    * @param iMode : Decryption mode
    * @return NULL
    */
    void Decrypt(char const* in, char* result, size_t n, int iMode = ECB);
};

#endif // __RIJNDAEL_H__
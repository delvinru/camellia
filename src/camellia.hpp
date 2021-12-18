#ifndef __CAMELLIA_HPP__
#define __CAMELLIA_HPP__

namespace Camellia
{
#define SBOX1(n) SBOX[(n)]
#define SBOX2(n) (uint8_t)((SBOX[(n)] >> 7 ^ SBOX[(n)] << 1) & 0xff)
#define SBOX3(n) (uint8_t)((SBOX[(n)] >> 1 ^ SBOX[(n)] << 7) & 0xff)
#define SBOX4(n) SBOX[((n) << 1 ^ (n) >> 7) & 0xff]

    void ekeygen(const uint8_t *k, uint8_t *e, const uint32_t n);
    void encrypt(const uint8_t *plaintext, const uint8_t *e, uint8_t *ciphertext, const uint32_t n);
    void decrypt(const uint8_t *ciphertext, const uint8_t *e, uint8_t *plaintext, const uint32_t keysize);
}

#endif
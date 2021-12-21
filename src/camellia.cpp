#include <iostream>
#include <cstdint>
#include "camellia.hpp"

namespace Camellia
{
    // In specification uses this constants
    // Sigma1 = 0xA09E667F3BCC908B;
    // Sigma2 = 0xB67AE8584CAA73B2;
    // Sigma3 = 0xC6EF372FE94F82BE;
    // Sigma4 = 0x54FF53A5F1D36F1C;
    // Sigma5 = 0x10E527FADE682D1D;
    // Sigma6 = 0xB05688C2B3E6C1FD;
    const uint8_t SIGMA[48] = {
        0xa0, 0x9e, 0x66, 0x7f, 0x3b, 0xcc, 0x90, 0x8b,
        0xb6, 0x7a, 0xe8, 0x58, 0x4c, 0xaa, 0x73, 0xb2,
        0xc6, 0xef, 0x37, 0x2f, 0xe9, 0x4f, 0x82, 0xbe,
        0x54, 0xff, 0x53, 0xa5, 0xf1, 0xd3, 0x6f, 0x1c,
        0x10, 0xe5, 0x27, 0xfa, 0xde, 0x68, 0x2d, 0x1d,
        0xb0, 0x56, 0x88, 0xc2, 0xb3, 0xe6, 0xc1, 0xfd};

    const uint32_t KSFT1[26] = {
        0x00, 0x40, 0x00, 0x40, 0x0f, 0x4f, 0x0f, 0x4f,
        0x1e, 0x5e, 0x2d, 0x6d, 0x2d, 0x7c, 0x3c, 0x7c,
        0x4d, 0x0d, 0x5e, 0x1e, 0x5e, 0x1e, 0x6f, 0x2f,
        0x6f, 0x2f};

    const uint32_t KIDX1[26] = {
        0x0, 0x0, 0x4, 0x4, 0x0, 0x0, 0x4, 0x4,
        0x4, 0x4, 0x0, 0x0, 0x4, 0x0, 0x4, 0x4,
        0x0, 0x0, 0x0, 0x0, 0x4, 0x4, 0x0, 0x0,
        0x4, 0x4};

    const uint32_t KSFT2[34] = {
        0x00, 0x40, 0x00, 0x40, 0x0f, 0x4f, 0x0f, 0x4f,
        0x1e, 0x5e, 0x1e, 0x5e, 0x2d, 0x6d, 0x2d, 0x6d,
        0x3c, 0x7c, 0x3c, 0x7c, 0x3c, 0x7c, 0x4d, 0x0d,
        0x4d, 0x0d, 0x5e, 0x1e, 0x5e, 0x1e, 0x6f, 0x2f,
        0x6f, 0x2f};

    const uint32_t KIDX2[34] = {
        0x00, 0x00, 0x0c, 0x0c, 0x08, 0x08, 0x04,
        0x04, 0x08, 0x08, 0x0c, 0x0c, 0x00, 0x00,
        0x04, 0x04, 0x00, 0x00, 0x08, 0x08, 0x0c,
        0x0c, 0x00, 0x00, 0x04, 0x04, 0x08, 0x08,
        0x04, 0x04, 0x00, 0x00, 0x0c, 0x0c};

    const uint8_t SBOX[256] = {
        0x70, 0x82, 0x2c, 0xec, 0xb3, 0x27, 0xc0, 0xe5,
        0xe4, 0x85, 0x57, 0x35, 0xea, 0x0c, 0xae, 0x41,
        0x23, 0xef, 0x6b, 0x93, 0x45, 0x19, 0xa5, 0x21,
        0xed, 0x0e, 0x4f, 0x4e, 0x1d, 0x65, 0x92, 0xbd,
        0x86, 0xb8, 0xaf, 0x8f, 0x7c, 0xeb, 0x1f, 0xce,
        0x3e, 0x30, 0xdc, 0x5f, 0x5e, 0xc5, 0x0b, 0x1a,
        0xa6, 0xe1, 0x39, 0xca, 0xd5, 0x47, 0x5d, 0x3d,
        0xd9, 0x01, 0x5a, 0xd6, 0x51, 0x56, 0x6c, 0x4d,
        0x8b, 0x0d, 0x9a, 0x66, 0xfb, 0xcc, 0xb0, 0x2d,
        0x74, 0x12, 0x2b, 0x20, 0xf0, 0xb1, 0x84, 0x99,
        0xdf, 0x4c, 0xcb, 0xc2, 0x34, 0x7e, 0x76, 0x05,
        0x6d, 0xb7, 0xa9, 0x31, 0xd1, 0x17, 0x04, 0xd7,
        0x14, 0x58, 0x3a, 0x61, 0xde, 0x1b, 0x11, 0x1c,
        0x32, 0x0f, 0x9c, 0x16, 0x53, 0x18, 0xf2, 0x22,
        0xfe, 0x44, 0xcf, 0xb2, 0xc3, 0xb5, 0x7a, 0x91,
        0x24, 0x08, 0xe8, 0xa8, 0x60, 0xfc, 0x69, 0x50,
        0xaa, 0xd0, 0xa0, 0x7d, 0xa1, 0x89, 0x62, 0x97,
        0x54, 0x5b, 0x1e, 0x95, 0xe0, 0xff, 0x64, 0xd2,
        0x10, 0xc4, 0x00, 0x48, 0xa3, 0xf7, 0x75, 0xdb,
        0x8a, 0x03, 0xe6, 0xda, 0x09, 0x3f, 0xdd, 0x94,
        0x87, 0x5c, 0x83, 0x02, 0xcd, 0x4a, 0x90, 0x33,
        0x73, 0x67, 0xf6, 0xf3, 0x9d, 0x7f, 0xbf, 0xe2,
        0x52, 0x9b, 0xd8, 0x26, 0xc8, 0x37, 0xc6, 0x3b,
        0x81, 0x96, 0x6f, 0x4b, 0x13, 0xbe, 0x63, 0x2e,
        0xe9, 0x79, 0xa7, 0x8c, 0x9f, 0x6e, 0xbc, 0x8e,
        0x29, 0xf5, 0xf9, 0xb6, 0x2f, 0xfd, 0xb4, 0x59,
        0x78, 0x98, 0x06, 0x6a, 0xe7, 0x46, 0x71, 0xba,
        0xd4, 0x25, 0xab, 0x42, 0x88, 0xa2, 0x8d, 0xfa,
        0x72, 0x07, 0xb9, 0x55, 0xf8, 0xee, 0xac, 0x0a,
        0x36, 0x49, 0x2a, 0x68, 0x3c, 0x38, 0xf1, 0xa4,
        0x40, 0x28, 0xd3, 0x7b, 0xbb, 0xc9, 0x43, 0xc1,
        0x15, 0xe3, 0xad, 0xf4, 0x77, 0xc7, 0x80, 0x9e};

    void XOR(const uint8_t *x, const uint8_t *y, uint8_t *out)
    {
        for (int i = 0; i < 16; i++)
            out[i] = x[i] ^ y[i];
    };

    void B2W(const uint8_t *x, uint32_t *y)
    {
        for (int i = 0; i < 4; i++)
            y[i] = ((uint32_t)x[(i << 2) + 0] << 24) + ((uint32_t)x[(i << 2) + 1] << 16) + ((uint32_t)x[(i << 2) + 2] << 8) + ((uint32_t)x[(i << 2) + 3] << 0);
    }

    void W2B(const uint32_t *x, uint8_t *y)
    {
        for (int i = 0; i < 4; i++)
        {
            y[(i << 2) + 0] = (uint8_t)(x[i] >> 24 & 0xff);
            y[(i << 2) + 1] = (uint8_t)(x[i] >> 16 & 0xff);
            y[(i << 2) + 2] = (uint8_t)(x[i] >> 8 & 0xff);
            y[(i << 2) + 3] = (uint8_t)(x[i] >> 0 & 0xff);
        }
    }

    void ROT(const uint32_t *x, const uint32_t n, uint32_t *y)
    {
        int r = n & 31;
        if (r)
        {
            y[0] = x[((n >> 5) + 0) & 3] << r ^ x[((n >> 5) + 1) & 3] >> (32 - r);
            y[1] = x[((n >> 5) + 1) & 3] << r ^ x[((n >> 5) + 2) & 3] >> (32 - r);
        }
        else
        {
            y[0] = x[((n >> 5) + 0) & 3];
            y[1] = x[((n >> 5) + 1) & 3];
        }
    }

    void SWAP(uint8_t *x)
    {
        for (int i = 0; i < 8; i++)
        {
            uint8_t tmp = x[i];
            x[i] = x[i + 8];
            x[i + 8] = tmp;
        }
    }

    // F-function in specification
    void feistel(const uint8_t *x, const uint8_t *k, uint8_t *y)
    {
        uint8_t t[8];
        t[0] = SBOX1(x[0] ^ k[0]);
        t[1] = SBOX2(x[1] ^ k[1]);
        t[2] = SBOX3(x[2] ^ k[2]);
        t[3] = SBOX4(x[3] ^ k[3]);
        t[4] = SBOX2(x[4] ^ k[4]);
        t[5] = SBOX3(x[5] ^ k[5]);
        t[6] = SBOX4(x[6] ^ k[6]);
        t[7] = SBOX1(x[7] ^ k[7]);

        y[0] ^= t[0] ^ t[2] ^ t[3] ^ t[5] ^ t[6] ^ t[7];
        y[1] ^= t[0] ^ t[1] ^ t[3] ^ t[4] ^ t[6] ^ t[7];
        y[2] ^= t[0] ^ t[1] ^ t[2] ^ t[4] ^ t[5] ^ t[7];
        y[3] ^= t[1] ^ t[2] ^ t[3] ^ t[4] ^ t[5] ^ t[6];
        y[4] ^= t[0] ^ t[1] ^ t[5] ^ t[6] ^ t[7];
        y[5] ^= t[1] ^ t[2] ^ t[4] ^ t[6] ^ t[7];
        y[6] ^= t[2] ^ t[3] ^ t[4] ^ t[5] ^ t[7];
        y[7] ^= t[0] ^ t[3] ^ t[4] ^ t[5] ^ t[6];
    }

    void fllayer(uint8_t *x, const uint8_t *kl, const uint8_t *kr)
    {
        uint32_t t[4], u[4], v[4];
        B2W(x, t);
        B2W(kl, u);
        B2W(kr, v);

        t[1] ^= (t[0] & u[0]) << 1 ^ (t[0] & u[0]) >> 31;
        t[0] ^= t[1] | u[1];
        t[2] ^= t[3] | v[1];
        t[3] ^= (t[2] & v[0]) << 1 ^ (t[2] & v[0]) >> 31;

        W2B(t, x);
    }

    void ekeygen(const uint8_t *k, uint8_t *e, const uint32_t n)
    {
        uint8_t t[64] = {0};
        uint32_t u[20] = {0};
        switch (n)
        {
        case 128:
            for (int i = 0; i < 16; i++)
                t[i] = k[i];
            for (int i = 16; i < 32; i++)
                t[i] = 0;
            break;
        case 192:
            for (int i = 0; i < 24; i++)
                t[i] = k[i];
            for (int i = 24; i < 32; i++)
                t[i] = k[i - 8] ^ 0xff;
            break;
        case 256:
            for (int i = 0; i < 32; i++)
                t[i] = k[i];
            break;
        default:
            throw std::invalid_argument("Invalid keysize");
            break;
        }

        XOR(t, t + 16, t + 32);

        feistel(t + 32, SIGMA, t + 40);
        feistel(t + 40, SIGMA + 8, t + 32);

        XOR(t + 32, t, t + 32);

        feistel(t + 32, SIGMA + 16, t + 40);
        feistel(t + 40, SIGMA + 24, t + 32);

        B2W(t, u);
        B2W(t + 32, u + 4);

        if (n == 128)
        {
            for (int i = 0; i < 26; i += 2)
            {
                ROT(u + KIDX1[i], KSFT1[i], u + 16);
                ROT(u + KIDX1[i + 1], KSFT1[i + 1], u + 18);
                W2B(u + 16, e + (i << 3));
            }
        }
        else
        {
            XOR(t + 32, t + 16, t + 48);

            feistel(t + 48, SIGMA + 32, t + 56);
            feistel(t + 56, SIGMA + 40, t + 48);

            B2W(t + 16, u + 8);
            B2W(t + 48, u + 12);

            for (int i = 0; i < 34; i += 2)
            {
                ROT(u + KIDX2[i], KSFT2[i], u + 16);
                ROT(u + KIDX2[i + 1], KSFT2[i + 1], u + 18);
                W2B(u + 16, e + (i << 3));
            }
        }
    }

    void encrypt(const uint8_t *plaintext, const uint8_t *e, uint8_t *ciphertext, const uint32_t n)
    {
        XOR(plaintext, e, ciphertext);
        for (int i = 0; i < 3; i++)
        {
            feistel(ciphertext, e + 16 + (i << 4), ciphertext + 8);
            feistel(ciphertext + 8, e + 24 + (i << 4), ciphertext);
        }

        fllayer(ciphertext, e + 64, e + 72);
        for (int i = 0; i < 3; i++)
        {
            feistel(ciphertext, e + 80 + (i << 4), ciphertext + 8);
            feistel(ciphertext + 8, e + 88 + (i << 4), ciphertext);
        }

        fllayer(ciphertext, e + 128, e + 136);
        for (int i = 0; i < 3; i++)
        {
            feistel(ciphertext, e + 144 + (i << 4), ciphertext + 8);
            feistel(ciphertext + 8, e + 152 + (i << 4), ciphertext);
        }

        if (n == 128)
        {
            SWAP(ciphertext);
            XOR(ciphertext, e + 192, ciphertext);
        }
        else
        {
            fllayer(ciphertext, e + 192, e + 200);
            for (int i = 0; i < 3; i++)
            {
                feistel(ciphertext, e + 208 + (i << 4), ciphertext + 8);
                feistel(ciphertext + 8, e + 216 + (i << 4), ciphertext);
            }

            SWAP(ciphertext);
            XOR(ciphertext, e + 256, ciphertext);
        }
    };

    void decrypt(const uint8_t *ciphertext, const uint8_t *e, uint8_t *plaintext, const uint32_t keysize)
    {
        if (keysize == 128)
            XOR(ciphertext, e + 192, plaintext);
        else
        {
            XOR(ciphertext, e + 256, plaintext);
            for (int i = 2; i >= 0; i--)
            {
                feistel(plaintext, e + 216 + (i << 4), plaintext + 8);
                feistel(plaintext + 8, e + 208 + (i << 4), plaintext);
            }
            fllayer(plaintext, e + 200, e + 192);
        }

        for (int i = 2; i >= 0; i--)
        {
            feistel(plaintext, e + 152 + (i << 4), plaintext + 8);
            feistel(plaintext + 8, e + 144 + (i << 4), plaintext);
        }

        fllayer(plaintext, e + 136, e + 128);

        for (int i = 2; i >= 0; i--)
        {
            feistel(plaintext, e + 88 + (i << 4), plaintext + 8);
            feistel(plaintext + 8, e + 80 + (i << 4), plaintext);
        }

        fllayer(plaintext, e + 72, e + 64);

        for (int i = 2; i >= 0; i--)
        {
            feistel(plaintext, e + 24 + (i << 4), plaintext + 8);
            feistel(plaintext + 8, e + 16 + (i << 4), plaintext);
        }

        SWAP(plaintext);
        XOR(plaintext, e, plaintext);
    }

}
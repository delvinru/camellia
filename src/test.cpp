#include <iostream>
#include <cstdint>
#include "camellia.hpp"

int main(int argc, char **argv)
{
    uint8_t pt[] = {
        0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
        0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};

    std::cout << "[+] Test 1" << std::endl;

    uint8_t key_1[] = {
        0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
        0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};

    uint8_t test_1[] = {
        0x67, 0x67, 0x31, 0x38, 0x54, 0x96, 0x69, 0x73,
        0x08, 0x57, 0x06, 0x56, 0x48, 0xea, 0xbe, 0x43};

    uint8_t ekey_1[272] = {0};
    uint8_t ct_1[16] = {0};
    Camellia::ekeygen(key_1, ekey_1, 128);
    Camellia::encrypt(pt, ekey_1, ct_1, 128);
    for (int i = 0; i < 16; i++)
    {
        if (test_1[i] != ct_1[i])
        {
            std::cout << "[!] Test 1 failed" << std::endl;
            printf("%d: got %x expected %x\n", i, ct_1[i], test_1[i]);
            // exit(EXIT_FAILURE);
        }
    }
    std::cout << "[+] Test 1 passed succesfully" << std::endl;

    std::cout << "[+] Test 2" << std::endl;

    uint8_t key_2[] = {
        0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
        0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10,
        0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77};

    uint8_t test_2[] = {
        0xb4, 0x99, 0x34, 0x01, 0xb3, 0xe9, 0x96, 0xf8,
        0x4e, 0xe5, 0xce, 0xe7, 0xd7, 0x9b, 0x09, 0xb9};

    uint8_t ekey_2[272] = {0};
    uint8_t ct_2[16] = {0};
    Camellia::ekeygen(key_2, ekey_2, 192);
    Camellia::encrypt(pt, ekey_2, ct_2, 192);
    for (int i = 0; i < 16; i++)
    {
        if (test_2[i] != ct_2[i])
        {
            std::cout << "[!] Test 2 failed" << std::endl;
            printf("%d: got %x expected %x\n", i, ct_2[i], test_2[i]);
            exit(EXIT_FAILURE);
        }
    }

    std::cout << "[+] Test 2 passed succesfully" << std::endl;

    std::cout << "[+] Test 3" << std::endl;

    uint8_t key_3[] = {
        0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
        0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10,
        0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
        0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

    uint8_t test_3[] = {
        0x9a, 0xcc, 0x23, 0x7d, 0xff, 0x16, 0xd7, 0x6c,
        0x20, 0xef, 0x7c, 0x91, 0x9e, 0x3a, 0x75, 0x09};

    uint8_t ekey_3[272] = {0};
    uint8_t ct_3[16] = {0};
    Camellia::ekeygen(key_3, ekey_3, 256);
    Camellia::encrypt(pt, ekey_3, ct_3, 256);
    for (int i = 0; i < 16; i++)
    {
        if (test_3[i] != ct_3[i])
        {
            std::cout << "[!] Test 3 failed" << std::endl;
            printf("%d: got %x expected %x\n", i, ct_3[i], test_3[i]);
            exit(EXIT_FAILURE);
        }
    }

    std::cout << "[+] Test 3 passed succesfully" << std::endl;
}
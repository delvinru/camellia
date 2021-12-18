#include <iostream>
#include "camellia.hpp"

#define size(arr) sizeof(arr) / sizeof(uint8_t)

int main(int argc, char **argv)
{
    std::cout << "[+] Init encryption" << std::endl;

    const uint32_t keysize = 128;
    uint8_t plaintext[16] = {
        0x41, 0x42, 0x43, 0x44,
        0x41, 0x42, 0x43, 0x44,
        0x41, 0x42, 0x43, 0x44,
        0x41, 0x42, 0x43, 0x44};
    uint8_t key[32] = {
        0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
        0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10,
        0x01, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
        0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

    uint8_t ctext[16] = {0}, rtext[16] = {0}, ekey[272] = {0};

    Camellia::ekeygen(key, ekey, keysize);

    Camellia::encrypt(plaintext, ekey, ctext, keysize);
    std::cout << "[+] Done encryption" << std::endl;

    std::cout << "[+] Encrypted text" << std::endl;
    Camellia::hexdump(ctext, size(ctext));

    Camellia::decrypt(ctext, ekey, rtext, keysize);
    std::cout << "[+] Decrypted text" << std::endl;

    Camellia::hexdump(rtext, size(rtext));
    return 0;
}
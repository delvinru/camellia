#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <random>
#include <algorithm>

#include "camellia.hpp"

#define mysize(arr) sizeof(arr) / sizeof(uint8_t)

std::string get_cmd(char **begin, char **end, const std::string &option)
{
    char **itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
        return *itr;
    return std::string();
}

bool check_cmd(char **begin, char **end, const std::string &option_1, const std::string &option_2)
{
    return std::find(begin, end, option_1) != end || std::find(begin, end, option_2) != end;
}

void print_usage(char **argv)
{
    std::cout << "Usage: " << argv[0] << "[-ed] [-hv] {-f <file>} [-k <key>] [-o <file>]" << std::endl;
    std::cout << "-e (--encrypt): encrypt data" << std::endl
              << "-d (--decrypt): decrypt data" << std::endl
              << "-f (--file): input filename" << std::endl
              << "-k (--key): key for cipher" << std::endl
              << "-o (--output): output filename or empty to out in stdout" << std::endl
              << "-v (--verbose): verbose mode" << std::endl
              << "-h (--help): show this help page" << std::endl;
    exit(EXIT_SUCCESS);
}

std::string parse_filename(char **argv, char **end)
{
    std::string file_1, file_2;
    if (check_cmd(argv, end, "-f", "--file"))
    {
        file_1 = get_cmd(argv, end, "-f");
        file_2 = get_cmd(argv, end, "--file");
    }

    if (!file_1.empty())
        return file_1;

    if (!file_2.empty())
        return file_2;

    std::cout << "[!] Empty filename" << std::endl;
    exit(EXIT_FAILURE);
}

std::vector<uint8_t> generate_key()
{
    std::srand(std::time(nullptr));
    std::vector<uint8_t> res;

    uint8_t KEY_SIZE = 32;

    for (int i = 0; i < KEY_SIZE; i++)
        res.push_back(std::rand() & 0xFF);

    return res;
}

std::vector<uint8_t> read_data(std::string filename)
{
    std::ifstream myfile(filename, std::ios::binary);
    std::vector<uint8_t> data((std::istreambuf_iterator<char>(myfile)), std::istreambuf_iterator<char>());
    myfile.close();
    return data;
}

std::vector<uint8_t> encrypt(std::vector<uint8_t> &data, std::string &key)
{
}

void hexdump(uint8_t *data, uint32_t length)
{
    uint32_t width = 0;
    for (uint8_t *p = data; length > 0; ++p)
    {
        if (width >= 16)
        {
            std::cout << std::endl;
            width = 0;
        }
        printf("%02x ", *p);
        --length;
        ++width;
    }
}

int main(int argc, char **argv)
{
    if (argc == 1 || check_cmd(argv, argv + argc, "-h", "--help"))
        print_usage(argv);

    if (!check_cmd(argv, argv + argc, "-f", "--file"))
        print_usage(argv);

    if (check_cmd(argv, argv + argc, "-e", "--encrypt"))
    {
        std::string filename = parse_filename(argv, argv + argc);
        std::vector<uint8_t> data = read_data(filename);

        std::vector<uint8_t> key;

        if (!check_cmd(argv, argv + argc, "-k", "--key"))
        {
            key = generate_key();
            std::cout << "[+] Use random key" << std::endl;
        }

        uint8_t ukey[key.size()];
        std::copy(key.begin(), key.end(), ukey);
        hexdump(ukey, key.size());
    }

    return 0;
}
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

std::string parse_outputfile(char **argv, char **end)
{
    std::string file_1, file_2;
    if (check_cmd(argv, end, "-o", "--output"))
    {
        file_1 = get_cmd(argv, end, "-o");
        file_2 = get_cmd(argv, end, "--output");
    }

    if (!file_1.empty())
        return file_1;

    if (!file_2.empty())
        return file_2;

    return std::string();
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

std::vector<uint8_t> encrypt(std::vector<uint8_t> &data, std::vector<uint8_t> &key)
{
    uint8_t ekey[272] = {0};
    uint8_t ukey[key.size()];
    uint32_t keysize = key.size() * 8;

    std::copy(key.begin(), key.end(), ukey);

    Camellia::ekeygen(ukey, ekey, keysize);

    std::vector<uint8_t> result;
    for (int i = 0; i < data.size(); i += 16)
    {
        uint8_t tmp[16] = {0};
        uint8_t res[16] = {0};
        std::copy(data.begin() + i, data.begin() + i + 16, tmp);
        Camellia::encrypt(tmp, ekey, res, keysize);

        for (int j = 0; j < 16; j++)
            result.push_back(res[j]);
    }
    return result;
}

void hexdump(std::vector<uint8_t> data_vec)
{
    uint8_t data[data_vec.size()];
    std::copy(data_vec.begin(), data_vec.end(), data);

    uint32_t width = 0;
    uint32_t length = data_vec.size();
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
    std::cout << std::endl;
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
            hexdump(key);
        }
        else
        {
            // TODO: append key to 16, 24, 32 bytes
        }

        std::vector<uint8_t> encrypted = encrypt(data, key);
        std::string output_file;
        if (check_cmd(argv, argv + argc, "-o", "--output"))
            output_file = parse_outputfile(argv, argv + argc);
        else
            output_file = filename + ".enc";

        std::cout << "[+] Encrypted data saved to: " + output_file << std::endl;

        std::ofstream outfile(output_file, std::ios::out | std::ios::binary);
        outfile.write((char *)&encrypted[0], encrypted.size());
    }
    else if (check_cmd(argv, argv + argc, "-d", "--decrypt"))
    {
    }
    else
    {
        std::cout << "[!] Incorrect option" << std::endl;
        print_usage(argv);
    }

    return 0;
}
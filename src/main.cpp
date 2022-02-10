#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <random>
#include <algorithm>

#include "camellia.hpp"

#define PADDING_VALUE 0x00

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

void print_usage(char **argv)
{
    std::cout << "Usage: " << argv[0] << " [-ed] [-hv] {-f <file>} [-k <key>] [-l <key_length>] [-o <file>]" << std::endl;
    std::cout << "-e (--encrypt): encrypt data" << std::endl
              << "-d (--decrypt): decrypt data" << std::endl
              << "-f (--file): input filename" << std::endl
              << "-k (--key): key file for encryption/decryption" << std::endl
              << "-l (--key-len): the length of the key to pad" << std::endl
              << "-o (--output): output filename or use default with postfix '.enc'" << std::endl
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

        if (!file_1.empty())
            return file_1;

        if (!file_2.empty())
            return file_2;
    }

    std::cout << "[!] Empty input filename" << std::endl;
    exit(EXIT_FAILURE);
}

std::string parse_outputfile(char **argv, char **end, std::string input_filename, bool modeFlag)
{
    std::string file_1, file_2;
    if (check_cmd(argv, end, "-o", "--output"))
    {
        file_1 = get_cmd(argv, end, "-o");
        file_2 = get_cmd(argv, end, "--output");

        if (!file_1.empty())
            return file_1;

        if (!file_2.empty())
            return file_2;
    }
    return (modeFlag == 1) ? input_filename + ".enc" : input_filename + ".dec";
}

std::string parse_key(char **argv, char **end)
{
    std::string file_1, file_2;
    if (check_cmd(argv, end, "-k", "--key"))
    {
        file_1 = get_cmd(argv, end, "-k");
        file_2 = get_cmd(argv, end, "--key");

        if (!file_1.empty())
            return file_1;

        if (!file_2.empty())
            return file_2;
    }

    std::cout << "[!] The decryption key is not specified" << std::endl;
    exit(EXIT_FAILURE);
}

int parse_key_length(char **argv, char **end)
{
    std::string arg_1, arg_2;
    if (check_cmd(argv, end, "-l", "--key_len"))
    {
        arg_1 = get_cmd(argv, end, "-l");
        arg_2 = get_cmd(argv, end, "--key-len");

        if (!arg_1.empty())
            return std::stoi(arg_1);

        if (!arg_2.empty())
            return std::stoi(arg_2);
    }
    return 0;
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

void save_data(std::vector<uint8_t> data, std::string output_filename)
{
    std::ofstream outfile(output_filename, std::ios::out | std::ios::binary);
    outfile.write((char *)&data[0], data.size());
}

void pad_key(std::vector<uint8_t> *key, uint8_t n)
{
    for (int i = (*key).size(); i < n; i++)
        (*key).push_back(PADDING_VALUE);
}

std::vector<uint8_t> check_key(char **begin, char **end)
{
    std::string filename = parse_key(begin, end);
    std::vector<uint8_t> key_data = read_data(filename);
    if (key_data.size() <= 16)
        pad_key(&key_data, 16);
    else if (key_data.size() > 16 && key_data.size() <= 24)
        pad_key(&key_data, 24);
    else if (key_data.size() > 24 && key_data.size() <= 32)
        pad_key(&key_data, 32);
    else
    {
        std::cout << "[!] Key size should be less or equal 32 bytes" << std::endl;
        exit(EXIT_FAILURE);
    }
    return key_data;
}

std::vector<uint8_t> check_key(char **begin, char **end, int key_len)
{
    std::string filename = parse_key(begin, end);
    std::vector<uint8_t> key_data = read_data(filename);
    if (key_len % 8 != 0)
    {
        std::cout << "[!] Key length should be 16, 24 or 32 bytes" << std::endl;
        exit(EXIT_FAILURE);
    }
    pad_key(&key_data, key_len);
    return key_data;
}

std::vector<uint8_t> encrypt(std::vector<uint8_t> &data, std::vector<uint8_t> &key)
{
    uint8_t ekey[272] = {0};
    uint8_t ukey[key.size()];
    uint32_t keysize = key.size() * 8;
    std::cout << "[+] Keysize: " << keysize << std::endl;

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

std::vector<uint8_t> decrypt(std::vector<uint8_t> &data, std::vector<uint8_t> &key)
{
    uint8_t ekey[272] = {0};
    uint8_t ukey[key.size()];
    uint32_t keysize = key.size() * 8;

    std::cout << "[+] Keysize: " << keysize << std::endl;

    std::copy(key.begin(), key.end(), ukey);

    Camellia::ekeygen(ukey, ekey, keysize);

    std::vector<uint8_t> result;
    for (int i = 0; i < data.size(); i += 16)
    {
        uint8_t tmp[16] = {0};
        uint8_t res[16] = {0};
        std::copy(data.begin() + i, data.begin() + i + 16, tmp);
        Camellia::decrypt(tmp, ekey, res, keysize);

        for (int j = 0; j < 16; j++)
            result.push_back(res[j]);
    }
    return result;
}

int main(int argc, char **argv)
{
    if (argc == 1 || check_cmd(argv, argv + argc, "-h", "--help"))
        print_usage(argv);

    if (!check_cmd(argv, argv + argc, "-f", "--file"))
        print_usage(argv);

    std::string filename = parse_filename(argv, argv + argc);
    std::vector<uint8_t> data = read_data(filename);
    std::vector<uint8_t> key;

    if (check_cmd(argv, argv + argc, "-e", "--encrypt"))
    {
        if (!check_cmd(argv, argv + argc, "-k", "--key"))
        {
            key = generate_key();
            std::cout << "[+] Use random key" << std::endl;
            save_data(key, "key.bin");
        }
        else
        {
            std::cout << "[+] Use user key" << std::endl;
            if (check_cmd(argv, argv + argc, "-l", "--key-len"))
            {
                int key_length = parse_key_length(argv, argv + argc);
                std::cout << "[+] Key length: " << key_length << std::endl;
                key = check_key(argv, argv + argc, key_length);
            }
            else
                key = check_key(argv, argv + argc);
        }
        hexdump(key);

        // Encrypt data
        std::vector<uint8_t> encrypted = encrypt(data, key);

        // Parse output file
        std::string output_file = parse_outputfile(argv, argv + argc, filename, true);

        // Save data to file
        std::cout << "[+] Encrypted data saved to: " + output_file << std::endl;
        save_data(encrypted, output_file);
    }
    else if (check_cmd(argv, argv + argc, "-d", "--decrypt"))
    {
        if (check_cmd(argv, argv + argc, "-l", "--key-len"))
        {
            int key_length = parse_key_length(argv, argv + argc);
            std::cout << "[+] Key length: " << key_length << std::endl;
            key = check_key(argv, argv + argc, key_length);
        }
        else
            key = check_key(argv, argv + argc);

        std::cout << "[+] Read key:" << std::endl;
        hexdump(key);

        std::vector<uint8_t> decrypted = decrypt(data, key);
        std::string output_file = parse_outputfile(argv, argv + argc, filename, false);
        std::cout << "[+] Decrypted data saved to: " + output_file << std::endl;
        save_data(decrypted, output_file);
    }
    else
    {
        std::cout << "[!] Incorrect option" << std::endl;
        print_usage(argv);
    }

    return 0;
}

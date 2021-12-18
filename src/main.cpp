#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include "camellia.hpp"

#define size(arr) sizeof(arr) / sizeof(uint8_t)

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
    std::cout << "Usage: " << argv[0] << " [-hv] {-f <file>} [-o <file>]" << std::endl;
    std::cout << "-f (--file): input filename" << std::endl
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

int main(int argc, char **argv)
{
    if (argc == 1 || check_cmd(argv, argv + argc, "-h", "--help"))
        print_usage(argv);

    if (!check_cmd(argv, argv + argc, "-f", "--file"))
        print_usage(argv);

    std::string filename = parse_filename(argv, argv + argc);

    std::cout << "[+] Parsed filename: " << filename << std::endl;

    return 0;
}
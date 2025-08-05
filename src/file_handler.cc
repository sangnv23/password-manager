#include "file_handler.h"
#include <fstream>
#include <iostream>

FileHandler::FileHandler(const std::string &filename) : filename(filename) {}

bool FileHandler::loadPasswords(std::vector<Password> &passwords) {
    passwords.clear();

    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file for reading: " << filename << "\n";
        return false;
    }

    Password pwd{};
    while (file.read(reinterpret_cast<char*>(&pwd), sizeof(Password))) {
        passwords.push_back(pwd);
    }

    file.close();
    return true;
}

bool FileHandler::savePasswords(const std::vector<Password> &passwords) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file for writing: " << filename << "\n";
        return false;
    }

    for (const auto &pwd : passwords) {
        file.write(reinterpret_cast<const char*>(&pwd), sizeof(Password));
    }

    file.close();
    return true;
}

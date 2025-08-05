#pragma once

#include <string>
#include <vector>
#include "password.h"

class FileHandler {
public:
    FileHandler(const std::string &filename);
    bool loadPasswords(std::vector<Password> &passwords);
    bool savePasswords(const std::vector<Password> &passwords);

private:
    std::string filename;
};
#pragma once

#include <vector>
#include <string>
#include "password.h"
#include "file_handler.h"

class PasswordManager
{
public:
    PasswordManager(const std::string &filename);

    void addPassword(const Password &password);
    bool editPassword(const std::string &name, const Password &newPasswordData);
    bool removePassword(const std::string &name);

    void searchPasswords(const std::string &query) const;
    void sortPasswords(const std::vector<std::string> &fields);
    bool isPasswordUsed(const std::string &password) const;

    const std::vector<Password> &getPasswords() const;

    void addCategory(const std::string &category);
    void removeCategory(const std::string &category);
    void printCategories() const;

    std::string randomPassword(int length, bool upperCase, bool lowerCase, bool specialChar) const;

private:
    std::vector<Password> passwords;
    FileHandler fileHandler;
    std::vector<std::string> categories;

    void load();
    void save();
};

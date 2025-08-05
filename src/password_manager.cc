#include "password_manager.h"
#include "constants.h"

#include <algorithm>
#include <iostream>
#include <ctime>
#include <cctype>
#include <random>

PasswordManager::PasswordManager(const std::string& filename)
    : fileHandler(filename) {
    load();
}

void PasswordManager::load() {
    if (!fileHandler.loadPasswords(passwords)) {
        std::cout << "Password file could not be loaded or does not exist. Starting with empty list.\n";
    } else {
        // Load categories from passwords:
        categories.clear();
        for (const auto& pwd : passwords) {
            if (!pwd.category.empty() &&
                std::find(categories.begin(), categories.end(), pwd.category) == categories.end()) {
                categories.push_back(pwd.category);
            }
        }
    }
}

void PasswordManager::save() {
    fileHandler.savePasswords(passwords);
}

void PasswordManager::addPassword(const Password& password) {
    passwords.push_back(password);

    // Add category if new and not empty
    if (!password.category.empty() &&
        std::find(categories.begin(), categories.end(), password.category) == categories.end()) {
        categories.push_back(password.category);
    }

    save();
}

bool PasswordManager::editPassword(const std::string& name, const Password& newPasswordData) {
    for (auto& pwd : passwords) {
        if (pwd.name == name) {
            pwd = newPasswordData;

            if (!pwd.category.empty() &&
                std::find(categories.begin(), categories.end(), pwd.category) == categories.end()) {
                categories.push_back(pwd.category);
            }

            save();
            return true;
        }
    }
    return false;
}

bool PasswordManager::removePassword(const std::string& name) {
    auto it = std::remove_if(passwords.begin(), passwords.end(),
                             [&name](const Password& pwd) { return pwd.name == name; });
    if (it == passwords.end()) {
        return false;
    }
    passwords.erase(it, passwords.end());
    save();
    return true;
}

void PasswordManager::searchPasswords(const std::string& query) const {
    std::cout << "Search results:\n";
    bool found = false;
    for (const auto& pwd : passwords) {
        if (pwd.name.find(query) != std::string::npos ||
            pwd.password.find(query) != std::string::npos ||
            pwd.category.find(query) != std::string::npos ||
            pwd.website.find(query) != std::string::npos ||
            pwd.login.find(query) != std::string::npos) {

            std::cout << "Name: " << pwd.name << "\n"
                      << "Password: " << pwd.password << "\n"
                      << "Category: " << pwd.category << "\n"
                      << "Website: " << pwd.website << "\n"
                      << "Login: " << pwd.login << "\n\n";
            found = true;
        }
    }
    if (!found) {
        std::cout << "No matching passwords found.\n";
    }
}

void PasswordManager::sortPasswords(const std::vector<std::string>& fields) {
    std::vector<Password> result = passwords;

    std::sort(result.begin(), result.end(),
        [&fields](const Password& a, const Password& b) {
        for (const auto& field : fields) {
            if (field == "name") {
                if (a.name != b.name) return a.name < b.name;
            } else if (field == "password") {
                if (a.password != b.password) return a.password < b.password;
            } else if (field == "category") {
                if (a.category != b.category) return a.category < b.category;
            } else if (field == "website") {
                if (a.website != b.website) return a.website < b.website;
            } else if (field == "login") {
                if (a.login != b.login) return a.login < b.login;
            }
        }
        return false;
    });

    std::cout << "Sorted passwords:\n";
    for (const auto& pwd : result) {
        std::cout << "Name: " << pwd.name << "\n"
                  << "Password: " << pwd.password << "\n"
                  << "Category: " << pwd.category << "\n"
                  << "Website: " << pwd.website << "\n"
                  << "Login: " << pwd.login << "\n\n";
    }
}

bool PasswordManager::isPasswordUsed(const std::string& password) const {
    for (const auto& pwd : passwords) {
        if (pwd.password == password) {
            return true;
        }
    }
    return false;
}

std::string PasswordManager::randomPassword(int length, bool upperCase, bool lowerCase, bool specialChar) const {
    std::string combinedChars;
    if (upperCase) combinedChars += kUpperChars;
    if (lowerCase) combinedChars += kLowerChars;
    if (specialChar) combinedChars += kSpecialChars;

    if (combinedChars.empty() || length <= 0)
        return "";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, combinedChars.size() - 1);

    std::string result;
    for (int i = 0; i < length; ++i) {
        result += combinedChars[dis(gen)];
    }
    return result;
}

void PasswordManager::addCategory(const std::string& category) {
    if (category.empty()) return;

    if (std::find(categories.begin(), categories.end(), category) == categories.end())
        categories.push_back(category);
}

void PasswordManager::removeCategory(const std::string& category) {
    if (category.empty()) return;

    auto itPwd = std::remove_if(passwords.begin(), passwords.end(),
                                [&category](const Password& pwd) { return pwd.category == category; });
    passwords.erase(itPwd, passwords.end());

    auto itCat = std::remove(categories.begin(), categories.end(), category);
    categories.erase(itCat, categories.end());

    save();
}

void PasswordManager::printCategories() const {
    std::cout << "Available categories:\n";
    for (const auto& cat : categories) {
        std::cout << "- " << cat << "\n";
    }
}

const std::vector<Password>& PasswordManager::getPasswords() const {
    return passwords;
}

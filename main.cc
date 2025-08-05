#include "password_manager.h"

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <fstream>
#include <algorithm>

bool fileExists(const std::string &filename)
{
    std::ifstream file(filename);
    return file.good();
}

int main()
{
    std::string filename;
    std::cout << "Enter name of the file: ";
    std::getline(std::cin, filename);

    PasswordManager manager(filename);

    if (!fileExists(filename))
    {
        std::cout << "File does not exist. Starting with an empty password file.\n";
    }

    while (true)
    {
        std::cout << "1. Search password\n"
                  << "2. Sort passwords\n"
                  << "3. Add password\n"
                  << "4. Edit password\n"
                  << "5. Delete password\n"
                  << "6. Add category\n"
                  << "7. Delete category\n"
                  << "8. Exit\n"
                  << "Choose an option: ";

        int choice = 0;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice)
        {
        case 1:
        {
            std::string query;
            std::cout << "Enter search query: ";
            std::getline(std::cin, query);
            manager.searchPasswords(query);
            break;
        }
        case 2:
        {
            std::vector<std::string> fields;
            while (true)
            {
                std::string field;
                std::cout << "Enter field to sort by (name, password, category, website, login) or 'q' to finish: ";
                std::getline(std::cin, field);
                if (field == "q" || field == "Q")
                    break;
                fields.push_back(field);
            }
            if (!fields.empty())
            {
                manager.sortPasswords(fields);
            }
            else
            {
                std::cout << "No sorting fields entered.\n";
            }
            break;
        }
        case 3:
        {
            Password pwd;

            std::cout << "Enter name: ";
            std::getline(std::cin, pwd.name);
            while (pwd.name.empty())
            {
                std::cout << "Name cannot be empty. Enter name: ";
                std::getline(std::cin, pwd.name);
            }

            std::cout << "Generate random password? (y/n): ";
            char randChoice = '\0';
            std::cin >> randChoice;
            randChoice = std::tolower(randChoice);
            std::cin.ignore();

            if (randChoice == 'y')
            {
                int length = 0;
                bool uppercase = false, lowercase = false, special = false;

                std::cout << "Password length: ";
                std::cin >> length;
                std::cin.ignore();

                std::cout << "Include uppercase letters? (y/n): ";
                char c;
                std::cin >> c;
                c = std::tolower(c);
                uppercase = (c == 'y');
                std::cin.ignore();

                std::cout << "Include lowercase letters? (y/n): ";
                std::cin >> c;
                c = std::tolower(c);
                lowercase = (c == 'y');
                std::cin.ignore();

                std::cout << "Include special characters? (y/n): ";
                std::cin >> c;
                c = std::tolower(c);
                special = (c == 'y');
                std::cin.ignore();

                pwd.password = manager.randomPassword(length, uppercase, lowercase, special);
                std::cout << "Generated Password: " << pwd.password << "\n";
            }
            else if (randChoice == 'n')
            {
                do
                {
                    std::cout << "Enter password: ";
                    std::getline(std::cin, pwd.password);
                    if (manager.isPasswordUsed(pwd.password))
                    {
                        std::cout << "Password already used. Consider using a different password.\n";
                    }
                } while (pwd.password.empty());
            }
            else
            {
                std::cout << "Invalid choice.\n";
                break;
            }

            std::cout << "Existing categories:\n";
            manager.printCategories();

            std::cout << "Use an existing category? (y/n): ";
            char catChoice;
            std::cin >> catChoice;
            catChoice = std::tolower(catChoice);
            std::cin.ignore();

            if (catChoice == 'y')
            {
                std::cout << "Enter category name: ";
                std::getline(std::cin, pwd.category);
            }
            else
            {
                do
                {
                    std::cout << "Enter new category name: ";
                    std::getline(std::cin, pwd.category);
                } while (pwd.category.empty());
            }

            std::cout << "Website (optional): ";
            std::getline(std::cin, pwd.website);

            std::cout << "Login (optional): ";
            std::getline(std::cin, pwd.login);

            manager.addPassword(pwd);
            break;
        }
        case 4:
        {
            std::string name;
            std::cout << "Enter the name of the password to edit: ";
            std::getline(std::cin, name);

            const auto &passwords = manager.getPasswords();
            auto it = std::find_if(passwords.begin(), passwords.end(),
                                   [&name](const Password &p)
                                   { return p.name == name; });
            if (it == passwords.end())
            {
                std::cout << "Password not found.\n";
                break;
            }

            Password edited = *it;
            std::string input;

            std::cout << "Current name: " << edited.name << "\nNew name (press enter to keep): ";
            std::getline(std::cin, input);
            if (!input.empty())
                edited.name = input;

            std::cout << "Current password: " << edited.password << "\nNew password (press enter to keep): ";
            std::getline(std::cin, input);
            if (!input.empty())
                edited.password = input;

            std::cout << "Current category: " << edited.category << "\nNew category (press enter to keep): ";
            std::getline(std::cin, input);
            if (!input.empty())
                edited.category = input;

            std::cout << "Current website: " << edited.website << "\nNew website (press enter to keep): ";
            std::getline(std::cin, input);
            if (!input.empty())
                edited.website = input;

            std::cout << "Current login: " << edited.login << "\nNew login (press enter to keep): ";
            std::getline(std::cin, input);
            if (!input.empty())
                edited.login = input;

            if (manager.editPassword(name, edited))
            {
                std::cout << "Password edited successfully.\n";
            }
            else
            {
                std::cout << "Failed to edit password.\n";
            }

            break;
        }
        case 5:
        {
            std::string name;
            std::cout << "Enter name of the password to delete: ";
            std::getline(std::cin, name);

            std::cout << "Are you sure you want to delete this password? (y/n): ";
            char confirm;
            std::cin >> confirm;
            confirm = std::tolower(confirm);
            std::cin.ignore();

            if (confirm == 'y')
            {
                if (manager.removePassword(name))
                {
                    std::cout << "Password deleted.\n";
                }
                else
                {
                    std::cout << "Password not found.\n";
                }
            }
            else
            {
                std::cout << "Operation cancelled.\n";
            }
            break;
        }
        case 6:
        {
            std::string category;
            std::cout << "Enter new category name: ";
            std::getline(std::cin, category);
            if (category.empty())
            {
                std::cout << "Category name cannot be empty.\n";
            }
            else
            {
                manager.addCategory(category);
                std::cout << "Category added.\n";
            }
            break;
        }
        case 7:
        {
            std::string category;
            std::cout << "Enter category name to delete: ";
            std::getline(std::cin, category);
            manager.removeCategory(category);
            std::cout << "Category and its related passwords removed.\n";
            break;
        }
        case 8:
        {
            std::cout << "Exiting...\n";
            return 0;
        }
        default:
            std::cout << "Invalid option, please try again.\n";
        }
        std::cout << std::endl;
    }
}

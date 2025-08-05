# Password Manager

A lightweight C++ command-line password manager application that securely stores passwords with categories, supports searching, sorting, editing, and random password generation.

## Features

- Add, edit, delete passwords with fields: name, password, category, website, login.
- Search passwords by any field.
- Sort passwords by customizable field order.
- Generate random passwords with customizable length and character sets (upper, lower, special).
- Manage categories and delete categories along with all associated passwords.
- Saves passwords in a binary file.
- Extensible design using modular classes (`PasswordManager`, `FileHandler`, `Password`).


## Build Instructions

### Prerequisites

- C++ compiler supporting C++17 (gcc >= 7, clang >= 5, MSVC 2017+)
- [CMake](https://cmake.org/) 3.10 or higher
- Internet connection to download GoogleTest (automatic on build)

### Steps

1. Create and navigate to a build directory:
    ```
    mkdir build && cd build
    ```

2. Configure the project with CMake:
    ```
    cmake ..
    ```

3. Build the application and tests:
    ```
    cmake --build .
    ```

4. Run the password manager application:
    ```
    ./password_manager 
    ```

## Usage

On running the program, you will be prompted for a filename to load or create. Afterwards, use the menu-driven interface to:

- Search passwords by a query string.
- Sort passwords by one or multiple fields.
- Add new passwords (with option for random generation).
- Edit or delete existing passwords.
- Add or delete categories.
- Exit the program.

Follow on-screen instructions during each step.


#include "gtest/gtest.h"
#include "password_manager.h"

class PasswordManagerTest : public ::testing::Test
{
protected:
    PasswordManagerTest() : manager("test_passwords.dat")
    {
        // Clean any test data on startup to avoid interference between tests
        auto passwords = manager.getPasswords();
        for (const auto &pwd : passwords)
        {
            manager.removePassword(pwd.name);
        }
        manager.removeCategory("TestCat");
        manager.removeCategory("Work");
        manager.removeCategory("Personal");
    }
    ~PasswordManagerTest() override
    {
        // Cleanup after test run
        auto passwords = manager.getPasswords();
        for (const auto &pwd : passwords)
        {
            manager.removePassword(pwd.name);
        }
        manager.removeCategory("TestCat");
        manager.removeCategory("Work");
        manager.removeCategory("Personal");
    }

    PasswordManager manager;
};

TEST_F(PasswordManagerTest, RemovePassword)
{
    Password pwd{"RemoveMe", "12345", "TestCat", "example.com", "admin"};
    manager.addPassword(pwd);

    EXPECT_TRUE(manager.removePassword("RemoveMe"));
    EXPECT_FALSE(manager.removePassword("RemoveMe")); // Already removed
}

TEST_F(PasswordManagerTest, EditPassword)
{
    Password pwd{"EditMe", "pass", "TestCat", "example.com", "admin"};
    manager.addPassword(pwd);

    Password edited = pwd;
    edited.password = "newpass";
    edited.category = "Work";

    EXPECT_TRUE(manager.editPassword("EditMe", edited));

    // Confirm changes
    auto passwords = manager.getPasswords();
    bool found = false;
    for (const auto &p : passwords)
    {
        if (p.name == "EditMe")
        {
            EXPECT_EQ(p.password, "newpass");
            EXPECT_EQ(p.category, "Work");
            found = true;
        }
    }
    EXPECT_TRUE(found);
}

TEST_F(PasswordManagerTest, SortPasswordsByNameAndCategory)
{
    Password pwd1{"zzEntry", "aaa", "Work", "", ""};
    Password pwd2{"aaEntry", "zzz", "Personal", "", ""};
    Password pwd3{"mmEntry", "mmm", "Work", "", ""};

    manager.addPassword(pwd1);
    manager.addPassword(pwd2);
    manager.addPassword(pwd3);

    std::vector<std::string> sortFields{"category", "name"};

    testing::internal::CaptureStdout();
    manager.sortPasswords(sortFields);
    std::string output = testing::internal::GetCapturedStdout();

    // We expect the first sorted password's name to be "aaEntry" (Personal < Work)
    EXPECT_NE(output.find("Name: aaEntry"), std::string::npos);
}

TEST_F(PasswordManagerTest, SearchPasswords)
{
    Password pwd{"SearchTest", "mySecret", "Personal", "mysite.com", "loginUser"};
    manager.addPassword(pwd);

    testing::internal::CaptureStdout();
    manager.searchPasswords("Secret");
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("SearchTest"), std::string::npos);

    // Searching for a non-existing term should yield no matches
    testing::internal::CaptureStdout();
    manager.searchPasswords("NoMatch123");
    output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("No matching passwords found"), std::string::npos);
}

TEST_F(PasswordManagerTest, CategoryManagement)
{
    std::string category = "TestCategory";
    manager.addCategory(category);

    testing::internal::CaptureStdout();
    manager.printCategories();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find(category), std::string::npos);

    // Remove category deletes associated passwords too
    Password pwd{"CatPwd", "catpass", category, "", ""};
    manager.addPassword(pwd);

    manager.removeCategory(category);

    auto passwords = manager.getPasswords();
    bool found = false;
    for (const auto &p : passwords)
    {
        if (p.category == category)
        {
            found = true;
            break;
        }
    }
    EXPECT_FALSE(found);
}

TEST_F(PasswordManagerTest, RandomPasswordGeneration)
{
    std::string password = manager.randomPassword(12, true, true, true);
    EXPECT_EQ(password.size(), 12);

    // Check the presence of at least 1 uppercase, 1 lowercase and 1 special if requested
    bool hasUpper = false, hasLower = false, hasSpecial = false;
    const std::string specials = "!@#$%^&*()-+=~`;:'?/";
    for (auto c : password)
    {
        if (std::isupper(static_cast<unsigned char>(c)))
            hasUpper = true;
        else if (std::islower(static_cast<unsigned char>(c)))
            hasLower = true;
        else if (specials.find(c) != std::string::npos)
            hasSpecial = true;
    }

    EXPECT_TRUE(hasUpper);
    EXPECT_TRUE(hasLower);
    EXPECT_TRUE(hasSpecial);

    // Test no chars requested returns empty
    std::string emptyPassword = manager.randomPassword(10, false, false, false);
    EXPECT_TRUE(emptyPassword.empty());

    // Test length zero returns empty
    emptyPassword = manager.randomPassword(0, true, true, true);
    EXPECT_TRUE(emptyPassword.empty());
}

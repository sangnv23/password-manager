cmake_minimum_required(VERSION 3.2)
project(password_manager)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable(password_manager main.cc src/password_manager.cc
    src/file_handler.cc)

###

# Enable testing
enable_testing()

# Add GoogleTest
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# Test sources
set(TEST_SOURCES
    tests/password_manager_test.cc
)

add_executable(password_manager_tests ${TEST_SOURCES} src/password_manager.cc src/file_handler.cc)

target_link_libraries(password_manager_tests gtest_main)

include(GoogleTest)
gtest_discover_tests(password_manager_tests)
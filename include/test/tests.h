#ifndef TESTS_H
#define TESTS_H

#include "test/colors.h"

#define TEST_CASE(x) \
    std::cout << CYAN("Testing: ") << YELLOW(x) << std::endl;

#define TEST(x)                                                      \
    if (!(x))                                                        \
    {                                                                \
        std::cerr << RED("Test failed at the following expression: " \
                         << #x)                                      \
                  << std::endl;                                      \
        exit(1);                                                     \
    }                                                                \
    else                                                             \
    {                                                                \
        std::cout << GREEN("OK => ") << YELLOW(#x) << std::endl;     \
    }

#define TEST_SUCCESS() std::cout << GREEN("All tests passed successfully!") << std::endl

#endif // TESTS_H
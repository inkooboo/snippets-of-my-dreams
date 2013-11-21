#include <iostream>

#define TEST(NAME) \
    std::cout << "*** " << #NAME << " ***" << std::endl; \
    extern int NAME##_test(); \
    NAME##_test(); \
    std::cout << std::endl;

int main()
{
    TEST(config)
    TEST(subsystem)
    TEST(callback)
    return 0;
}
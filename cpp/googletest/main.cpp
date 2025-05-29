#include <iostream>
#include <gtest/gtest.h>

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    std::cout << "Running unit tests" << std::endl;
    int retval = RUN_ALL_TESTS();
    return retval;
}


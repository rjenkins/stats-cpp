//
// Created by rjenkins on 1/16/21.
//
#include "gtest/gtest.h"
#include <buffer.h>

TEST(Buffer, testCreateBuffer) {
    char data[65535];
    for (int i=0; i<5; i++) {
        std::cout << data[i] << std::endl;
    };
    std::string s("hello");
    memcpy(&data, s.c_str(), s.size());
    auto offset = s.size();
    std::cout << std::string(data) << std::endl;
    std::cout << sizeof(data) << std::endl;
    std::cout << s.size() << std::endl;

    std::string w(" world");
    memcpy(&data[offset], w.c_str(), w.size());
    std::cout << std::string(data) << std::endl;
    memset(data, 0, 65535);
    std::cout << std::string(data) << std::endl;
    std::cout << "done" << std::endl;

    std::vector<char> foo;
    std::copy(s.begin(), s.end(), std::back_inserter(foo));

    std::cout << std::string(foo.begin(), foo.end()) << std::endl;

    std::copy(w.begin(), w.end(), std::back_inserter(foo));
    std::cout << std::string(foo.begin(), foo.end()) << std::endl;

}
TEST(Buffer, testCreateBufferSizedPool) {
    //Buffer b(1024, 5);
}

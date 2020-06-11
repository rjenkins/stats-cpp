//
// Created by rjenkins on 6/9/20.
//

#ifndef STATS_CPP_WRITER_H
#define STATS_CPP_WRITER_H

#include <string>

class Writer {
public:
    virtual int Write(const std::string& data) = 0;
    virtual ~Writer() = default;
};
#endif//STATS_CPP_WRITER_H

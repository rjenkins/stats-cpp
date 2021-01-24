//
// Created by rjenkins on 1/16/21.
//

#ifndef STATS_CPP_BUFFER_H
#define STATS_CPP_BUFFER_H
#include "spdlog/spdlog.h"
#include "writer.h"
#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>

class InternalBuffer {
    friend class Buffer;
public:
    InternalBuffer() {
        std::cout << "in buffer constructor" << std::endl;
    }
    std::atomic<uint64_t> lock{};
    std::vector<char> data;
    uint8_t pad[32]{};// padding to avoid false sharing between threads
protected:
    bool acquire();
    void release();
};

class Buffer {
public:
    Buffer(Writer& writer, const uint &bufferSize, const uint &bufferPoolSize);
    explicit Buffer(Writer& writer, const uint &bufferSize);
    void Handle(const std::string& data);
    int Flush(const std::string& data);

private:
    Writer& writer;
    uint bufferSize;
    uint bufferPoolSize;
    std::once_flag initialized;
    std::atomic_uint64_t offset{};
    std::vector<std::unique_ptr<InternalBuffer>> buffers;
    InternalBuffer &acquireBuffer();
};


#endif//STATS_CPP_BUFFER_H

//
// Created by rjenkins on 1/16/21.
//

#include "buffer.h"

Buffer::Buffer(Writer &writer, const uint &bufferSize, const uint &bufferPoolSize) : writer(writer), bufferSize(bufferSize), bufferPoolSize(bufferPoolSize) {
    buffers.reserve(bufferSize);
    for (int i = 0; i < bufferPoolSize; i++) {
        buffers.emplace_back();
    }
}
Buffer::Buffer(Writer &writer, const uint &bufferSize) : writer(writer), bufferSize(bufferSize) {
    auto processor_count = std::thread::hardware_concurrency();
    if (processor_count < 1) {
        spdlog::info("warning: hardware_concurrency return < 1, hard coding processor_count to 1");
        processor_count = 1;
    }
    std::cout << "processor count is " << processor_count << std::endl;
    bufferPoolSize = processor_count * 2;
    buffers.reserve(bufferSize);
    for (int i = 0; i < bufferPoolSize; i++) {
        buffers.emplace_back();
    }
}

void Buffer::Handle(const std::string &data) {
    auto &b = acquireBuffer();
    std::copy(data.begin(), data.end(), std::back_inserter(b.data));
    if (b.data.size() >= bufferSize) {
        std::string d(b.data.begin(), b.data.end());
        Flush(d);
        b.data.clear();
    }
    b.release();
}

int Buffer::Flush(const std::string &data) {
    return writer.Write(data);
}

InternalBuffer& Buffer::acquireBuffer() {
    int i = 0;
    int n = buffers.size();
    while (true) {
        auto o = offset++ % n;
        InternalBuffer &b = buffers[o];
        if (b.acquire()) {
            return b;
        }
        if ((i++ % n) == 0) {
            std::this_thread::yield();
        }
    }
}

bool InternalBuffer::acquire() {
    uint64_t x = 0;
    return lock.compare_exchange_strong(x, 1);
}

void InternalBuffer::release() {
    lock.store(0);
}

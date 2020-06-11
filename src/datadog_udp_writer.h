//
// Created by rjenkins on 6/9/20.
//

#ifndef STATS_CPP_DATADOG_UDP_WRITER_H
#define STATS_CPP_DATADOG_UDP_WRITER_H

#include "spdlog/spdlog.h"
#include "writer.h"
#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>


class DatadogUDPWriter : public Writer {
    static const uint32_t optimalPayload = 65467;
    static const uint32_t maxUdpPayload = 65467;
    static const uint32_t maxBuff = 50;

public:
    DatadogUDPWriter() = delete;
    ~DatadogUDPWriter() {
        int ret = close(sockfd);
        spdlog::debug("destroying writer: close returned {}", ret);
    }
    DatadogUDPWriter(const std::string &host, const uint32_t &port) : host(host), port(port) {
        memset(&server, 0, sizeof(struct sockaddr_in));
        sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        server.sin_family = AF_INET;
        server.sin_port = htons(port);
        server.sin_addr.s_addr = inet_addr(host.c_str());
    }
    int Write(const std::string &data) override;

private:
    std::string host;
    uint32_t port;
    int sockfd;
    struct sockaddr_in server {};
};


#endif//STATS_CPP_DATADOG_UDP_WRITER_H

//
// Created by rjenkins on 6/9/20.
//

#include "datadog_udp_writer.h"

int DatadogUDPWriter::Write(const std::string &data) {
    spdlog::debug("writing {} to datadog", data);
    return sendto(this->sockfd, data.c_str(), data.length(), 0, (struct sockaddr *) &server, sizeof(server));
}

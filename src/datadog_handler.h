//
// Created by rjenkins on 6/9/20.
//

#ifndef STATS_CPP_DATADOG_HANDLER_H
#define STATS_CPP_DATADOG_HANDLER_H

#include "datadog_udp_writer.h"
#include "handler.h"
#include "writer.h"

class DatadogHandler : public Handler {
public:
    static const std::string defaultHost;
    static const uint32_t defaultPort = 8125;

    DatadogHandler() : DatadogHandler(defaultHost, 8125) {}
    DatadogHandler(const std::string &host, const uint32_t &port) : writer(make_unique<DatadogUDPWriter>(host, port)) {}
    explicit DatadogHandler(const std::string &host) : DatadogHandler(host, 8125) {}
    explicit DatadogHandler(std::unique_ptr<Writer> writer) : writer(std::move(writer)){};
    void HandleMeasures(std::unique_ptr<Measure> measure) override;

private:
    std::unique_ptr<Writer> writer;
};


#endif//STATS_CPP_DATADOG_HANDLER_H

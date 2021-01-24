//
// Created by rjenkins on 6/9/20.
//

#ifndef STATS_CPP_DATADOG_HANDLER_H
#define STATS_CPP_DATADOG_HANDLER_H

#include "buffer.h"
#include "datadog_udp_writer.h"
#include "handler.h"
#include "writer.h"

class DatadogHandler : public Handler {
public:
    static const std::string defaultHost;
    static const uint32_t defaultPort;
    //    static const int MAX_BUFFER_SIZE = 65507;
    //    static const int DEFAULT_BUFFER_SIZE = MAX_BUFFER_SIZE / 2;
    //    DatadogHandler() : DatadogHandler(defaultHost, defaultPort, DEFAULT_BUFFER_SIZE) {}
    //    DatadogHandler(const std::string &host, const uint32_t &port, Buffer const& b) : buffer(&b) { }
    //    explicit DatadogHandler(const std::string &host) : DatadogHandler(host, defaultPort, DEFAULT_BUFFER_SIZE) {}
    //    explicit DatadogHandler(Writer& writer) : :{buffer(writer, DEFAULT_BUFFER_SIZE){};
    void HandleMeasures(const Measure<int> &value) override;
    void HandleMeasures(const Measure<double> &value) override;


private:
    Buffer *buffer = nullptr;
    void process(const std::string &name, const std::string &value, const MetricType &type, const std::vector<Tag> &tags, const float &rate);
};


#endif//STATS_CPP_DATADOG_HANDLER_H

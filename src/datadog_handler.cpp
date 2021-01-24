//
// Created by rjenkins on 6/9/20.
//

#include "datadog_handler.h"
#include <sstream>

const std::string DatadogHandler::defaultHost = "127.0.0.1";
const uint32_t DatadogHandler::defaultPort = 8125;

// <METRIC_NAME>:<VALUE>|<TYPE>|@<SAMPLE_RATE>|#<TAG_KEY_1>:<TAG_VALUE_1>,<TAG_2>
// Example: users.online:1|c|@0.5|#country:china
void DatadogHandler::HandleMeasures(const Measure<int> &measure) {
    process(measure.name, measure.GetValue(), measure.type, measure.tags, measure.rate);
}
void DatadogHandler::HandleMeasures(const Measure<double> &measure) {
    auto value = measure.GetValue();
    process(measure.name, measure.GetValue(), measure.type, measure.tags, measure.rate);
}
void DatadogHandler::process(const std::string &name, const std::string &value, const MetricType &type, const std::vector<Tag> &tags, const float &rate) {
    std::ostringstream output;
    output << name << ":" << value;
    switch (type) {
        case MetricTypes::Counter:
            output << "|c";
            break;
        case MetricTypes::Gauge:
            output << "|g";
            break;
        case MetricTypes::Histogram:
            output << "|h";
            break;
    }
    if (rate < 1) {
        output << "|@" << rate;
    }
    if (tags.size()) {
        output << "|#";
        auto tagCount = tags.size();
        for (int i = 0; i < tagCount; i++) {
            output << tags[i].name << ":" << tags[i].value;
            if (tagCount > 1 && i < tagCount - 1) {
                output << ",";
            }
        }
    }

    buffer->Handle(output.str());
//    int ret = writer->Write(output.str());
//    spdlog::debug("writer returned: {} for {}", ret, output.str());

    //    if (!global_tags_str.empty()) {
    //        if (!tagging) {
    //            statd << "|#" ;
    //        } else {
    //            statd << "," ;
    //        }
    //        statd << global_tags_str;
    //    }
    //return output.str();
}

//
// Created by rjenkins on 6/9/20.
//

#include "datadog_handler.h"
#include <sstream>

const std::string DatadogHandler::defaultHost = "127.0.0.1";

// <METRIC_NAME>:<VALUE>|<TYPE>|@<SAMPLE_RATE>|#<TAG_KEY_1>:<TAG_VALUE_1>,<TAG_2>
// Example: users.online:1|c|@0.5|#country:china
void DatadogHandler::HandleMeasures(std::unique_ptr<Measure> measure) {
    std::ostringstream output;

    output << measure->name << ":" << measure->GetValue();
    switch (measure->type) {
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

    if (measure->rate < 1) {
        output << "|@" << measure->rate;
    }

    if (measure->tags.size()) {
        output << "|#";
        auto tagCount = measure->tags.size();
        for (int i = 0; i < tagCount; i++) {
            output << measure->tags[i].name << ":" << measure->tags[i].value;
            if (tagCount > 1 && i < tagCount - 1) {
                output << ",";
            }
        }
    }

    int ret = writer->Write(output.str());
    spdlog::debug("writer returned: {} for {}", ret, output.str());

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

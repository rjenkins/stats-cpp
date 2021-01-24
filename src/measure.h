//
// Created by rjenkins on 6/8/20.
//

#ifndef STATS_CPP_MEASURE_H
#define STATS_CPP_MEASURE_H

#include "metric_type.h"
#include "spdlog/spdlog.h"
#include <chrono>
#include <string>
#include <utility>
#include <vector>

using namespace std::chrono;
using namespace MetricTypes;

template<typename T>
struct Measure {
    Measure(const time_point<high_resolution_clock> &time, const std::string& name, const MetricType &type, const T& value, const std::vector<Tag>& tags)
        : time(time), name(name), type(type), rate(1.0f), value(value), tags(tags) {
        spdlog::debug("in measure constructor");
    }
    Measure(const time_point<high_resolution_clock> &time, const std::string& name, const MetricType &type, const float& rate, const T& value, const std::vector<Tag>& tags)
            : time(time), name(name), type(type), rate(rate), value(value), tags(tags) {
        spdlog::debug("in measure constructor");
    }
    // Print << overrides
    friend std::ostream& operator<<(std::ostream &out, const Measure &m) {
        return m.Print(out);
    }
    std::ostream& Print(std::ostream& out) const  {
        spdlog::info("name: {}, type: {}, time: {}, value: {}", this->name, this->type, system_clock::to_time_t(this->time), this->value);
        PrintTags();
        return out;
    }
    void PrintTags() const {
        if (!tags.empty()) {
            spdlog::info("tags:");
            for (const auto &t : tags) {
                spdlog::info("name: {} value: {}", t.name, t.value);
            }
        }
    }
    std::string GetValue() const {
        return std::to_string(value);
    }
    // Class Members
    const time_point<high_resolution_clock> time;
    const std::string name;
    const MetricType type;
    const T value;
    const float rate;
    const std::vector<Tag> tags;
};

#endif//STATS_CPP_MEASURE_H

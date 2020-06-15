//
// Created by rjenkins on 6/8/20.
//

#ifndef STATS_CPP_MEASURE_H
#define STATS_CPP_MEASURE_H

#include "metric_type.h"
#include "spdlog/spdlog.h"
#include "tag.h"
#include <chrono>
#include <string>
#include <utility>
#include <vector>

using namespace std::chrono;
using namespace MetricTypes;

struct Measure {
public:
    Measure(const time_point<high_resolution_clock> &time, std::string name, const MetricType &type, std::vector<Tag> tags)
        : time(time), name(std::move(name)), type(type), rate(1.0f), tags(std::move(tags)) {
        spdlog::debug("in measure constructor");
    }
    virtual ~Measure() {
        spdlog::debug("in Measure destructor");
    }
    const time_point<high_resolution_clock> time;
    const std::string name;
    const MetricType type;
    const float rate;
    const std::vector<Tag> tags;
    virtual void Print() const = 0;
    virtual std::string GetValue() = 0;
};

struct DoubleMeasure : public Measure {
public:
    DoubleMeasure(const time_point<high_resolution_clock> &time, const std::string &name, const MetricType &type, const std::vector<Tag> &tags, const double &value)
        : Measure(time, name, type, tags), value(value) {
        spdlog::debug("in DoubleMeasure constructor");
    }
    ~DoubleMeasure() override {
        spdlog::debug("in DoubleMeasure destructor");
    }
    void Print() const override {
        spdlog::info("name: {}, type: {}, time: {}, value: {}", this->name, this->type, system_clock::to_time_t(this->time), this->value);
    }
    std::string GetValue() override {
        return std::to_string(value);
    }
    const double value;
};

struct IntegerMeasure : public Measure {
public:
    IntegerMeasure(const time_point<high_resolution_clock> &time, const std::string &name, const MetricType &type, const std::vector<Tag> &tags, const int64_t &value)
        : Measure(time, name, type, tags), value(value) {
        spdlog::debug("in IntegerMeasure constructor");
    }
    ~IntegerMeasure() override {
        spdlog::debug("in IntegerMeasure destructor");
    }
    void Print() const override {
        spdlog::info("name: {}, type: {}, time: {}, value: {}", this->name, this->type, system_clock::to_time_t(this->time), this->value);
    }
    std::string GetValue() override {
        return std::to_string(value);
    }
    const int64_t value;
};

#endif//STATS_CPP_MEASURE_H

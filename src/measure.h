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

struct Measure {
    Measure(const time_point<high_resolution_clock> &time, std::string name, const MetricType &type, std::vector<Tag> tags)
        : time(time), name(std::move(name)), type(type), rate(1.0f), tags(std::move(tags)) {
        spdlog::debug("in measure constructor");
    }
    virtual ~Measure() = default;
    // Primarily used for testing
    virtual std::unique_ptr<Measure> clone() const = 0;
    // Print << overrides
    friend std::ostream& operator<<(std::ostream &out, const Measure &m) {
        return m.print(out);
    }
    virtual std::ostream& print(std::ostream& out) const {
        return out;
    }
    void printTags() const {
        if (!tags.empty()) {
            spdlog::info("tags:");
            for (const auto &t : tags) {
                spdlog::info("name: {} value: {}", t.name, t.value);
            }
        }
    }
    // Return value as string
    virtual std::string GetValue() const = 0;

    // Class Members
    const time_point<high_resolution_clock> time;
    const std::string name;
    const MetricType type;
    const float rate;
    const std::vector<Tag> tags;
};

struct DoubleMeasure : public Measure {
    DoubleMeasure(const time_point<high_resolution_clock> &time, const std::string &name, const MetricType &type, const std::vector<Tag> &tags, const double &value)
        : Measure(time, name, type, tags), value(value) {
        spdlog::debug("in DoubleMeasure constructor");
    }
    ~DoubleMeasure() override {
        spdlog::debug("in DoubleMeasure destructor");
    }
    std::string GetValue() const override {
        return std::to_string(value);
    }
    std::unique_ptr<Measure> clone() const override {
        std::unique_ptr<Measure> p = std::make_unique<DoubleMeasure>(time, name, type, tags, value);
        return p;
    }
    virtual std::ostream& print(std::ostream& out) const override {
        spdlog::info("name: {}, type: {}, time: {}, value: {}", this->name, this->type, system_clock::to_time_t(this->time), this->value);
        printTags();
        return out;
    }
    const double value;
};

struct IntegerMeasure : public Measure {
    IntegerMeasure(const time_point<high_resolution_clock> &time, const std::string &name, const MetricType &type, const std::vector<Tag> &tags, const int64_t &value)
        : Measure(time, name, type, tags), value(value) {
        spdlog::debug("in IntegerMeasure constructor");
    }
    ~IntegerMeasure() override {
        spdlog::debug("in IntegerMeasure destructor");
    }
    std::string GetValue() const override {
        return std::to_string(value);
    }
    virtual std::ostream& print(std::ostream& out) const override {
        spdlog::info("name: {}, type: {}, time: {}, value: {}", this->name, this->type, system_clock::to_time_t(this->time), this->value);
        printTags();
        return out;
    }
    std::unique_ptr<Measure> clone() const override {
        std::unique_ptr<Measure> p = std::make_unique<IntegerMeasure>(time, name, type, tags, value);
        return p;
    }
    const int64_t value;
};

#endif//STATS_CPP_MEASURE_H

//
// Created by rjenkins on 6/5/20.
//

#ifndef STATS_CPP_ENGINE_H
#define STATS_CPP_ENGINE_H

#include "handler.h"
#include "metric_type.h"
#include "tag.h"
#include <chrono>
#include <memory>
#include <string>
#include <utility>
#include <vector>

using namespace std::chrono;
using namespace MetricTypes;

// Engine is the the primary API for generating stats, though we provide
// static functions and a "Default" Engine in the Stats class which provide
// a wrapper to an Engine.
//
// Example:
class Engine {
public:
    // Constructors
    //
    // Create and engine with an empty prefix
    Engine() = default;
    // Create an engine and assign a prefix
    explicit Engine(const char *prefix) : prefix(prefix) {}
    // Create an engine and assign a prefix
    explicit Engine(std::string prefix) : prefix(std::move(prefix)) {}
    // Create an engine and provide a single handler
    Engine(const char *prefix, std::unique_ptr<Handler> handler);
    // Create an engine and provide a single handler
    Engine(std::string, std::unique_ptr<Handler> handler);
    // Create an engine and provide a vector of handlers
    Engine(const char *prefix, std::vector<std::unique_ptr<Handler>> &handlers);
    // Returns current handlers
    //const std::vector<std::unique_ptr<Handler>> &GetHandlers();
    std::vector<std::unique_ptr<Handler>> const &GetHandlers();
    // Register a new handler with the engine
    void RegisterHandler(std::unique_ptr<Handler> handler);

    // Metric Generating API
    //
    // Increments Counter by 1 no tags
    void Incr(const std::string &name);
    // Increments Counter by 1, vector of tags
    void Incr(const std::string &name, const std::vector<Tag> &tags);

    // Increments Counter by 1, with variadic Tags
    template<typename... Ts>
    void Incr(const std::string &name, Ts... tags) {
        std::vector<Tag> tv;
        for (auto x : {tags...}) {
            Tag t = static_cast<Tag>(static_cast<const Tag>(x));
            tv.push_back(t);
        }
        measure(system_clock::now(), name, 1, Counter, tv);
    }

    // Adds to a Counter, accepts integers or floats, vector of tags
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type * = nullptr>
    void Add(const std::string &name, const T &t) {
        Add(name, t, noTags);
    }

    // Adds to a Counter, accepts integers or floats, with variadic Tags
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type * = nullptr, typename... Ts>
    void Add(const std::string &name, const T &t, Ts... tags) {
        std::vector<Tag> tv;
        for (auto x : {tags...}) {
            Tag t = static_cast<Tag>(x);
            tv.push_back(t);
        }
        Add(name, t, tv);
    }

    // Adds to a Counter, accepts integers or floats, vector of tags
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type * = nullptr>
    void Add(const std::string &name, const T &t, const std::vector<Tag> &tags) {
        measure(system_clock::now(), name, t, Counter, tags);
    }

    // Sets a Gauge accepts integers or floats, no tags
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type * = nullptr>
    void Set(const std::string &name, const T &t) {
        Set(name, t, noTags);
    }

    // Sets a Gauge accepts integers or floats, with variadic Tags
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type * = nullptr, typename... Ts>
    void Set(const std::string &name, const T &t, Ts... tags) {
        std::vector<Tag> tv;
        for (auto x : {tags...}) {
            Tag t = static_cast<Tag>(x);
            tv.push_back(t);
        }
        Set(name, t, tv);
    }

    // Sets a Gauge accepts integers or floats, vector of tags
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type * = nullptr>
    void Set(const std::string &name, const T &t, const std::vector<Tag> &tags) {
        measure(system_clock::now(), name, t, Gauge, tags);
    }

    // Observe integer or float for a histogram, no tags
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type * = nullptr>
    void Observe(const std::string &name, const T &t) {
        Observe(name, t, noTags);
    }
    // Observers integers or floats, with variadic Tags
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type * = nullptr, typename... Ts>
    void Observe(const std::string &name, const T &t, Ts... tags) {
        std::vector<Tag> tv;
        for (auto x : {tags...}) {
            Tag t = static_cast<Tag>(x);
            tv.push_back(t);
        }
        Observe(name, t, tv);
    }
    // Observers integers or floats, with vector of Tags
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type * = nullptr>
    void Observe(const std::string &name, const T &t, const std::vector<Tag> &tags) {
        measure(system_clock::now(), name, t, Histogram, tags);
    }

    // Public member variables
    std::string prefix;

private:
    std::vector<std::unique_ptr<Handler>> handlers;
    static const std::vector<Tag> noTags;

    // Private Functions
    template<typename T, typename std::enable_if<std::is_integral<T>::value>::type * = nullptr>
    void measure(const time_point<high_resolution_clock> &observedTime, const std::string &name, const T &value, const MetricTypes::MetricType &type, const std::vector<Tag> &tags) {
        if (!prefix.empty()) {
            auto fname = prefix + ".";
            fname += name;
            sendMeasure(std::make_unique<DoubleMeasure>(observedTime, fname, type, tags, value));
            return;
        }
        sendMeasure(std::make_unique<IntegerMeasure>(observedTime, name, type, tags, value));
    }

    // Private Functions
    template<typename T, typename std::enable_if<std::is_floating_point<T>::value>::type * = nullptr>
    void measure(const time_point<high_resolution_clock> &observedTime, const std::string &name, const T &value, const MetricTypes::MetricType &type, const std::vector<Tag> &tags) {
        if (!prefix.empty()) {
            auto fname = prefix + ".";
            fname += name;
            sendMeasure(std::make_unique<DoubleMeasure>(observedTime, fname, type, tags, value));
            return;
        }
        sendMeasure(std::make_unique<DoubleMeasure>(observedTime, name, type, tags, value));
    }

    void sendMeasure(std::unique_ptr<Measure> m) {
        for (const auto &h : handlers) {
            h->HandleMeasures(std::move(m));
        }
    }
};

#endif//STATS_CPP_ENGINE_H

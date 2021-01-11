#pragma once

#include "engine.h"
#include "handler.h"
#include <iostream>
#include <string>
#include <vector>

// Static Wrapper class for ease of use across project without having to pass an Engine around everywhere.
//
// Example:
//
// Stats::SetPrefix("my-cool-app");
// Stats::RegisterHandler(std::make_unique<DatadogHandler>());
// Stats::Incr("cache.hit");
// Stats::Incr("error", Tag("page", "/foo"));
// std::vector<Tag> tags;
// tags.reserve(1);
// Tag::appendTag(tags, "type", "rpc-request");
// Stats::Add("rtt", 5.5, tags);


class Stats {
public:
    static Engine engine;
    static void SetPrefix(std::string prefix) {
        Stats::engine.prefix = prefix;
    }
    static void RegisterHandler(std::unique_ptr<Handler> handler) {
        Stats::engine.RegisterHandler(std::move(handler));
    }
    // Metric Generating API
    //
    // Increments Counter by 1 no tags
    static void Incr(const std::string &name) {
        Stats::engine.Incr(name);
    }
    // Increments Counter by 1, vector of tags
    static void Incr(const std::string &name, const std::vector<Tag> &tags) {
        Stats::engine.Incr(name, tags);
    }
    // Increments Counter by 1, with variadic Tags
    template<typename... Ts>
    static void Incr(const std::string &name, Ts... tags) {
        Stats::engine.Incr(name, tags...);
    }
    // Adds to a Counter, accepts integers or floats, vector of tags
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type * = nullptr>
    static void Add(const std::string &name, const T &t) {
        Stats::engine.Add(name, t);
    }

    // Adds to a Counter, accepts integers or floats, with variadic Tags
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type * = nullptr, typename... Ts>
    static void Add(const std::string &name, const T &t, Ts... tags) {
        Stats::engine.Add(name, t, tags...);
    }

    // Adds to a Counter, accepts integers or floats, vector of tags
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type * = nullptr>
    static void Add(const std::string &name, const T &t, const std::vector<Tag> &tags) {
        measure(system_clock::now(), name, t, Counter, tags);
    }

    // Sets a Gauge accepts integers or floats, no tags
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type * = nullptr>
    static void Set(const std::string &name, const T &t) {
        Stats::engine.Set(name, t);
    }

    // Sets a Gauge accepts integers or floats, with variadic Tags
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type * = nullptr, typename... Ts>
    static void Set(const std::string &name, const T &t, Ts... tags) {
        Stats::engine.Set(name, t, tags...);
    }

    // Sets a Gauge accepts integers or floats, vector of tags
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type * = nullptr>
    static void Set(const std::string &name, const T &t, const std::vector<Tag> &tags) {
        Stats::engine.Set(name, t, tags);
    }

    // Observe integer or float for a histogram, no tags
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type * = nullptr>
    static void Observe(const std::string &name, const T &t) {
        Stats::engine.Observe(name, t);
    }
    // Observers integers or floats, with variadic Tags
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type * = nullptr, typename... Ts>
    void Observe(const std::string &name, const T &t, Ts... tags) {
        Stats::engine.Observe(name, t, tags...);
    }
    // Observers integers or floats, with vector of Tags
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type * = nullptr>
    void Observe(const std::string &name, const T &t, const std::vector<Tag> &tags) {
        Stats::engine.Observe(name, t, tags);
    }
};

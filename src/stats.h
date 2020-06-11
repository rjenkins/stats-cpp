#pragma once

#include "engine.h"
#include "handler.h"
#include <iostream>
#include <string>
#include <vector>

class Stats {
public:
    static Engine engine;
    static void SetPrefix(std::string prefix) {
        Stats::engine.prefix = prefix;
    }
    static void AddHandler(std::unique_ptr<Handler> handler) {
        Stats::engine.RegisterHandler(std::move(handler));
    }
    static void Incr(const std::string& name, const std::vector<Tag>& tags) {
        engine.Incr(name, tags);
    }
};



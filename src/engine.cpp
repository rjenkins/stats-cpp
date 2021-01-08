//
// Created by rjenkins on 6/5/20.
//

#include "engine.h"
#include <iostream>

using namespace std::chrono;
using namespace MetricTypes;

const std::vector<Tag> Engine::noTags;

Engine::Engine(std::string prefix, std::unique_ptr<Handler> handler) : prefix(std::move(prefix)) {
    handlers.push_back(std::move(handler));
}
Engine::Engine(const char *prefix, std::unique_ptr<Handler> handler) : prefix(prefix) {
    handlers.push_back(std::move(handler));
}
Engine::Engine(const char *prefix, std::vector<std::unique_ptr<Handler>> &handlers) : prefix(std::move(prefix)), handlers(std::move(handlers)) {}

void Engine::RegisterHandler(std::unique_ptr<Handler> handler) {
    // todo note about thread safety
    handlers.push_back(std::move(handler));
}
void Engine::Incr(const std::string &name, const std::vector<Tag> &tags) {
    measure(system_clock::now(), name, 1, Counter, tags);
}
void Engine::Incr(const std::string &name) {
    Incr(name, noTags);
}
std::vector<std::unique_ptr<Handler>> const &Engine::GetHandlers()  {
    return handlers;
}

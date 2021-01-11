//
// Created by rjenkins on 6/9/20.
//

#ifndef STATS_CPP_TEST_HANDLER_H
#define STATS_CPP_TEST_HANDLER_H

#include <handler.h>
class TestHandler : public Handler {
public:
    TestHandler() = default;
    void HandleMeasures(const Measure& value) override {
        measures.push_back(value.clone());
    }
    ~TestHandler() override = default;
    std::vector<std::unique_ptr<Measure>> measures;
};
#endif//STATS_CPP_TEST_HANDLER_H

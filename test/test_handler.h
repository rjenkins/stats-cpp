//
// Created by rjenkins on 6/9/20.
//

#ifndef STATS_CPP_TEST_HANDLER_H
#define STATS_CPP_TEST_HANDLER_H

#include <handler.h>
class TestHandler : public Handler {
public:
    TestHandler() = default;
    void HandleMeasures(const Measure<int>& value) override {
        intMeasures.push_back(value);
    }
    void HandleMeasures(const Measure<double>& value) override {
        doubleMeasures.push_back(value);
    }
    ~TestHandler() override = default;
    std::vector<Measure<int>> intMeasures;
    std::vector<Measure<double>> doubleMeasures;
};
#endif//STATS_CPP_TEST_HANDLER_H

//
// Created by rjenkins on 6/5/20.
//
//
#ifndef STATS_CPP_HANDLER_H
#define STATS_CPP_HANDLER_H

#include <chrono>
#include <memory>
#include <vector>

#include "tag.h"
#include "metric_type.h"
#include "measure.h"

class Handler {
public:
    virtual void HandleMeasures(std::unique_ptr<Measure> value) = 0;
    virtual void HandleMeasures(const Measure& value) = 0;
    virtual ~Handler() = default;
};

#endif//STATS_CPP_HANDLER_H


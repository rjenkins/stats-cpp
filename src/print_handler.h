//
// Created by rjenkins on 6/5/20.
//

#ifndef STATS_CPP_PRINT_HANDLER_H
#define STATS_CPP_PRINT_HANDLER_H

#include "handler.h"
#include "measure.h"


class PrintHandler : public Handler {
public:
    PrintHandler() = default;
    void HandleMeasures(const Measure<int>& value) override;
    void HandleMeasures(const Measure<double>& value) override;
};


#endif//STATS_CPP_PRINT_HANDLER_H

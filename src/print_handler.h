//
// Created by rjenkins on 6/5/20.
//

#ifndef STATS_CPP_PRINT_HANDLER_H
#define STATS_CPP_PRINT_HANDLER_H

#include "handler.h"
#include "measure.h"


// Todo rework print handler to just print measures from overridden <<
class PrintHandler : public Handler {
public:
    PrintHandler() = default;
    void HandleMeasures(std::unique_ptr<Measure> measure) override;
};


#endif//STATS_CPP_PRINT_HANDLER_H

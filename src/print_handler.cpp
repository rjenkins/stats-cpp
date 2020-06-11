//
// Created by rjenkins on 6/5/20.
//

#include "print_handler.h"
#include "spdlog/spdlog.h"

void PrintHandler::HandleMeasures(const std::unique_ptr<Measure> m) {
    spdlog::debug("In PrintHandler::HandleMeasures");
    m->Print();
}

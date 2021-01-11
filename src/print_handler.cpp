//
// Created by rjenkins on 6/5/20.
//

#include "print_handler.h"
#include "spdlog/spdlog.h"

void PrintHandler::HandleMeasures(const Measure& m) {
    std::cout << m << std::endl;
}

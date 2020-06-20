//
// Created by rjenkins on 6/5/20.
//

#ifndef STATS_CPP_TAG_H
#define STATS_CPP_TAG_H

#include <iostream>
#include <spdlog/spdlog.h>
#include <utility>
#include <vector>

struct Tag {
public:
    Tag(std::string  name, std::string  value) : name(std::move(name)), value(std::move(value)){
        spdlog::debug("in tag constructor");
    }
    ~Tag() {
        spdlog::debug("in tag destructor");
    }
    Tag(const Tag& other) : name(other.name), value(other.value){
        spdlog::debug("in tag copy constructor");
    }
    static void appendTag(std::vector<Tag>& tags, const std::string& name, const std::string& value) {
        tags.emplace_back(name, value);
    }
    std::string name;
    std::string value;
};



#endif//STATS_CPP_TAG_H

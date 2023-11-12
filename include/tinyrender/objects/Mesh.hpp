#pragma once

#include <utility>
#include <string>

#include "Object.hpp"

#define STR_SUFFIX(s, suffix) s.substr(s.size()-suffix.size(), suffix.size()) == suffix

namespace tinyrender {

    class Mesh : public Object {
    protected:
        std::string path;
    public:
        Mesh(const std::string& path);

        void onInit(Context *c, Scene *s) override;
    };
}
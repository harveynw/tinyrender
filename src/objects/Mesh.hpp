#pragma once

#include "tiny_obj_loader.h"
#include <utility>

#include "Object.hpp"
#include "loaders/obj.hpp"

#define STR_SUFFIX(s, suffix) s.substr(s.size()-suffix.size(), suffix.size()) == suffix

namespace engine {

    class Mesh : public Object {
    public:
        Mesh(Context *c, Scene *s, const std::string& path);
    };
}
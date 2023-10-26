#pragma once

#include "tiny_obj_loader.h"
#include <utility>

#include "Object.hpp"
#include "loaders/Obj.hpp"

#define STR_SUFFIX(s, suffix) s.substr(s.size()-suffix.size(), suffix.size()) == suffix

namespace engine {

    class Mesh : public Object {
    protected:
        std::string path;
    public:
        Mesh(const std::string& path);

        void onInit(Context *c, Scene *s) override;
    };
}
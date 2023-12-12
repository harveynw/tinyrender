#pragma once

#include <memory>

#include "Object.hpp"

class CubeImpl;

namespace tinyrender {

    class Cube : public Object {
    public:
        Cube();
        ~Cube();
    };

}
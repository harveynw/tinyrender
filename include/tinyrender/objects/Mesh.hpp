#pragma once

#include "Object.hpp"

class MeshImpl;

namespace tinyrender {

    class Mesh : public Object {
    public:
        Mesh(const std::string& path);
        ~Mesh();
    };

}
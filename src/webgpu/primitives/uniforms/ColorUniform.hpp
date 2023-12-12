#pragma once

#include "BufferUniform.hpp"


class ColorUniform : public BufferUniform {
protected:
    ColorUniforms uniform{};
public:
    explicit ColorUniform(Context *context);
    void set(glm::vec3 color);
};


//
// Created by Harvey Williams on 18/08/2023.
//

#pragma once

#include "AttributeBuffer.hpp"
#include "../../engine.hpp"


namespace engine {
    class IndexBuffer : public AttributeBuffer {
    public:
        IndexBuffer(Engine *engine, std::vector<uint16_t> indexData);
    };
}
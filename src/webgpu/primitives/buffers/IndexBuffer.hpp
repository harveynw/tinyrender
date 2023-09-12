//
// Created by Harvey Williams on 18/08/2023.
//

#pragma once

#include "attributes/AttributeBuffer.hpp"

namespace engine {
    class IndexBuffer : public AttributeBuffer {
    public:
        IndexBuffer(Context *context, std::vector<uint16_t> indexData);
    };
}
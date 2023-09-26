//
// Created by Harvey Williams on 18/08/2023.
//

#pragma once

#include "Buffer.hpp"

namespace engine {
    class IndexBuffer : public Buffer {
    public:
        IndexBuffer(Context *context, std::vector<uint16_t> indexData);
    };
}
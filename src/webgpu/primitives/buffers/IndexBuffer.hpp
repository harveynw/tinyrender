//
// Created by Harvey Williams on 18/08/2023.
//

#pragma once

#include "Buffer.hpp"

class IndexBuffer : public tr::Buffer {
public:
    IndexBuffer(Context *context, std::vector<uint16_t> indexData);
};
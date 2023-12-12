#pragma once

#include "tiny_obj_loader.h"
#include <webgpu/webgpu.hpp>

#include "../../objects/ObjectImpl.hpp"
#include "../Scene.hpp"
#include "../Context.hpp"
#include "ObjectPipeline.hpp"

class Pipeline {
protected:
    // Whether to clear the current swap chain view with a color, should be true for the 'first' pipeline pass.
    // Currently, controls whether to clear the supplied depth buffer in all implementations.
    bool shouldClear = false;
    wgpu::Color clearValue;
public:
    Pipeline() = default;
    virtual ~Pipeline() = default;
    virtual void onFrame(wgpu::TextureView&, wgpu::CommandEncoder&,
                         std::vector<ObjectImpl*>&) = 0;

    void enableClear(wgpu::Color color) {
        shouldClear = true;
        clearValue = color;
    }
};


//
// Created by Harvey Williams on 08/08/2023.
//

#ifndef TINYGAME_PIPELINE_H
#define TINYGAME_PIPELINE_H

#include <webgpu/webgpu.hpp>


class Pipeline {
protected:
    // Whether to clear the current swap chain view with a color, should be true for the 'first' pipeline pass.
    // Currently, controls whether to clear the supplied depth buffer in all implementations.
    bool shouldClear = false;
    wgpu::Color clearValue;
public:
    Pipeline() {};
    virtual ~Pipeline() = default;
    virtual void onFrame(wgpu::TextureView&, wgpu::CommandEncoder&) = 0;

    void enableClear(wgpu::Color color) {
        shouldClear = true;
        clearValue = color;
    }
};


#endif //TINYGAME_PIPELINE_H

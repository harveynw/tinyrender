#pragma once

#include <cmath>
#include <vector>

#include "objects/WaveSim.hpp"
#include "ObjectImpl.hpp"
#include "loaders/Polygons.hpp"
#include "loaders/Shapes.hpp"
#include "../webgpu/primitives/textures/WavesDataTexture.hpp"
#include "ObjectResources.hpp"
#include "wavesim/naive.hpp"
#include "wavesim/FFTfftw.hpp"


class WaveSimImpl : public ObjectImpl {
private:
    using ObjectImpl::setTexture; // disable
protected:
    // Properties
    float WIDTH; // World space width
    float LENGTH; // World space height
    int MESH_RES_WIDTH = 256; // Mesh resolution
    int MESH_RES_LENGTH = 256; // Mesh resolution
    int DISP_MAP_RES = 512; // Resolution of the displacement map (square)

    // Grid mesh for drawing the waves
    std::shared_ptr<AttributeBuffer> mesh = nullptr;

    // Displacement of the surface is stored in a 1D vector, and uploaded to a texture on the GPU
    std::vector<uint8_t> displacementData;
    std::shared_ptr<Texture2D::WavesDataTexture> texture;

    // Simulation methods
    std::unique_ptr<NaiveWaveSim> naive;
    std::unique_ptr<FFTfftw> fftw;
public:
    WaveSimImpl(float width, float length);

    void onInit(Context *c, Scene *s) override;
    void onUpdate(State &state, float dt) override;
    void onRemove() override;

    void setColor(glm::vec3 c) override;

    ~WaveSimImpl();
};

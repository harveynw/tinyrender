#include "objects/WaveSim.hpp"

#include "loaders/Polygons.hpp"
#include "loaders/Shapes.hpp"
#include "../webgpu/primitives/textures/WavesDataTexture.hpp"
#include "ObjectResources.hpp"
#include "wavesim/naive.hpp"
#include "wavesim/FFTfftw.hpp"
#include "objects/WaveSim.hpp"


tinyrender::WaveSim::WaveSim(float width, float length): WIDTH(width), LENGTH(length) {
    assert(MESH_RES_WIDTH % 2 == 0);
    assert(MESH_RES_LENGTH % 2 == 0);
}

void 
tinyrender::WaveSim::onInit(Context *c, Scene *s) {
    Object::onInit(c, s);

    this->displacementData = std::vector<uint8_t>(4 * DISP_MAP_RES * DISP_MAP_RES);

    Polygons p;
    loadPlaneMesh(p, WIDTH, LENGTH, MESH_RES_WIDTH, MESH_RES_LENGTH);
    this->mesh = std::make_shared<tinyrender::AttributeBuffer>(this->context, p.data, p.vertices);
    this->resources = std::make_shared<ObjectResources>(this->context, this->scene, this->mesh, ColoredTriangle);

    // Init Waves Data and reset Bind Group to point to it
    this->texture = std::make_shared<tinyrender::Texture2D::WavesDataTexture>(c, DISP_MAP_RES, DISP_MAP_RES);
    this->resources->texture = this->texture;
    this->resources->maxDisplacement = std::make_shared<tinyrender::ScalarUniform>(c);
    this->resources->maxDisplacement->set(0.5);
    this->resources->resetBindGroup(Waves);

    naive = std::make_unique<NaiveWaveSim>(DISP_MAP_RES);
    fftw = std::make_unique<FFTfftw>(DISP_MAP_RES);
}

void 
tinyrender::WaveSim::onUpdate(State &state, float dt) {
    (void) state;
    //naive->update(dt, displacementData);
    fftw->update(dt, displacementData);
    this->texture->update(displacementData);
}

void 
tinyrender::WaveSim::onRemove() {
    Object::onRemove();

    this->displacementData.clear();
    this->texture.reset();
    this->resources.reset();
    this->naive.reset();
    this->fftw.reset();
}

void
tinyrender::WaveSim::setColor(glm::vec3 c) {
    this->resources->color->set(c);
}

tinyrender::WaveSim::~WaveSim()
{
}

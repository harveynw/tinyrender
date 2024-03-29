#include "WaveSimImpl.hpp"

WaveSimImpl::WaveSimImpl(float width, float length): WIDTH(width), LENGTH(length) {
    assert(MESH_RES_WIDTH % 2 == 0);
    assert(MESH_RES_LENGTH % 2 == 0);
}

void 
WaveSimImpl::onInit(Context *c, Scene *s) {
    ObjectImpl::onInit(c, s);

    this->displacementData = std::vector<uint8_t>(4 * DISP_MAP_RES * DISP_MAP_RES);

    Polygons p;
    loadPlaneMesh(p, WIDTH, LENGTH, MESH_RES_WIDTH, MESH_RES_LENGTH);
    this->mesh = std::make_shared<AttributeBuffer>(this->context, p.data, p.vertices);
    this->resources = std::make_shared<ObjectResources>(this->context, this->scene, this->mesh, ColoredTriangle);

    // Init Waves Data and reset Bind Group to point to it
    this->texture = std::make_shared<Texture2D::WavesDataTexture>(c, DISP_MAP_RES, DISP_MAP_RES);
    this->resources->texture = this->texture;
    this->resources->maxDisplacement = std::make_shared<ScalarUniform>(c);
    this->resources->maxDisplacement->set(0.5);
    this->resources->resetBindGroup(Waves);

    naive = std::make_unique<NaiveWaveSim>(DISP_MAP_RES);
    fftw = std::make_unique<FFTfftw>(DISP_MAP_RES);
}

void 
WaveSimImpl::onUpdate(State &state, float dt) {
    (void) state;
    //naive->update(dt, displacementData);
    fftw->update(dt, displacementData);
    this->texture->update(displacementData);
}

void 
WaveSimImpl::onRemove() {
    ObjectImpl::onRemove();

    this->displacementData.clear();
    this->texture.reset();
    this->resources.reset();
    this->naive.reset();
    this->fftw.reset();
}

void
WaveSimImpl::setColor(glm::vec3 c) {
    this->resources->color->set(c);
}

WaveSimImpl::~WaveSimImpl()
{
}

tinyrender::WaveSim::WaveSim(float width, float length) {
    obj = std::make_unique<WaveSimImpl>(width, length);
}

tinyrender::WaveSim::~WaveSim() = default;
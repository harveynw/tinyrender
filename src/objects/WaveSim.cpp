#include "WaveSim.hpp"

engine::WaveSim::WaveSim(Context *c, Scene *s, float width, float length) {
    this->context = c;
    this->scene = s;
    this->targetPipeline = Waves;

    assert(MESH_RES_WIDTH % 2 == 0);
    assert(MESH_RES_LENGTH % 2 == 0);

    this->displacementData = std::vector<uint8_t>(4 * DISP_MAP_RES * DISP_MAP_RES);

    Polygons p;
    loadPlaneMesh(p, width, length, MESH_RES_WIDTH, MESH_RES_LENGTH);

    auto attrs = std::make_shared<engine::AttributeBuffer>(this->context, p.data, p.vertices);
    this->resources = std::make_shared<ObjectResources>(this->context, this->scene, attrs, ColoredTriangle);

    // Init Waves Data and reset Bind Group to point to it
    this->texture = std::make_shared<engine::Texture2D::WavesDataTexture>(c, DISP_MAP_RES, DISP_MAP_RES);
    this->resources->texture = this->texture;
    this->resources->maxDisplacement = std::make_shared<engine::ScalarUniform>(c);
    this->resources->maxDisplacement->set(0.5);
    this->resources->resetBindGroup(c, s, Waves);

    #ifndef __EMSCRIPTEN__
    openCVSim = std::make_unique<FFTopencv>(DISP_MAP_RES);
    #endif
    naive = std::make_unique<NaiveWaveSim>(DISP_MAP_RES);
    fftw = std::make_unique<FFTfftw>(DISP_MAP_RES);
}

void
engine::WaveSim::update(float dt) {
    //naive->update(dt, displacementData);
    fftw->update(dt, displacementData);
    this->texture->update(displacementData);
}

void
engine::WaveSim::setColor(glm::vec3 c) {
    this->resources->color->set(c);
}

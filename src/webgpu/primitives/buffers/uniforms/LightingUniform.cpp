#include "LightingUniform.hpp"


engine::LightingUniform::LightingUniform(Context *context) {
    this->uniforms.directions = {
            glm::vec4(1.0, 0.2, 1.0, 1.0),
            glm::vec4(1.0, 0.8, 1.0, 1.0)
    };
    this->uniforms.colors = {
            glm::vec4(1.0, 1.0, 1.0, 1.0),
            glm::vec4(0.0, 0.0, 0.0, 1.0)
    };

    // Buffer base class is initialised
    this->context = context;
    this->type = engine::UNIFORM;
    this->size = sizeof(LightingUniforms);
    this->mapped = false;
    this->initialise(&uniforms);
}

#include "LightingUniform.hpp"


engine::LightingUniform::LightingUniform(Context *context) {

        this->uniforms.directions = {
                glm::vec4(0.0, 0.0, 1.0, 1.0),
                glm::vec4(4.0/5.74, 4.0/5.74, 1.0/5.74, 1.0),
        };
        this->uniforms.colors = {
                glm::vec4(1.0, 0.0, 0.3, 0.0),
                glm::vec4(1.0, 1.0, 1.0, 0.0)
        };
        this->uniforms.ambient = 0.5;

        // Buffer base class is initialised
        this->context = context;
        this->type = engine::UNIFORM;
        this->size = sizeof(LightingUniforms);
        this->mapped = false;
        this->initialise(&uniforms);
}

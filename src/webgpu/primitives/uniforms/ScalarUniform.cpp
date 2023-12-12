#include "ScalarUniform.hpp"

ScalarUniform::ScalarUniform(Context *context) {
    this->uniform.scalar = 0.0;

    // Buffer base class is initialised
    this->context = context;
    this->type = UNIFORM;
    this->size = sizeof(ScalarUniforms);
    this->mapped = false;
    this->initialise(&uniform);
}

void
ScalarUniform::set(float value) {
    this->uniform.scalar = value;
    context->queue.writeBuffer(underlying, offsetof(ScalarUniforms, scalar),
                               &uniform.scalar,sizeof(ScalarUniforms::scalar));
}


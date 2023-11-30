#pragma once

#include <map>
#include <string>
#include <memory>
#include <glm/glm.hpp>

#include "ChunkGeometry.hpp"
#include "../../webgpu/Context.hpp"
#include "../../webgpu/Scene.hpp"
#include "../../webgpu/primitives/uniforms/ModelMatrixUniform.hpp"

// Fwd declare chunk
class Chunk;

class Chunks {
protected:
    Context *c;
    Scene *s;

    std::shared_ptr<tinyrender::ModelMatrixUniform> globalModelMatrix;
public:
    Chunks(Context *c, Scene *s, std::shared_ptr<tinyrender::ModelMatrixUniform> globalModelMatrix): c(c), s(s), globalModelMatrix(globalModelMatrix) {};
    std::shared_ptr<Chunk> getChunk(ivec2 chunkCoordinate);

    std::map<std::string, std::shared_ptr<Chunk>> map;

    bool chunkTracked(ivec2 coord); // Instance of chunk exists
    bool chunkDisplayed(ivec2 coord); // Chunk is being rendered
};
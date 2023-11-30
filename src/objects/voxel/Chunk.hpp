#pragma once

#include <array>
#include <thread>

#include "VoxelData.hpp"
#include "ChunkGeometry.hpp"
#include "Chunks.hpp"
#include "BuildMesh.hpp"
#include "../ObjectResources.hpp"
#include "../loaders/Polygons.hpp"
#include "../../webgpu/primitives/buffers/AttributeBuffer.hpp"


namespace {
    const char CHUNK_INTERNAL_UNLOADED = 0x00;
    const char CHUNK_INTERNAL_LOADED = 0x01;
    const char CHUNK_INTERNAL_GENERATING_MESH = 0x02;

    struct GPU_CHUNK {
        // Buffer containing mesh to draw chunk
        std::shared_ptr<tinyrender::AttributeBuffer> buffer = nullptr;
        // Uniforms
        std::shared_ptr<ObjectResources> resources = nullptr;

        GPU_CHUNK(Context *c, Scene *s, std::shared_ptr<VoxelMesh> cpu, ivec2 cornerCoordinate, std::shared_ptr<tinyrender::ModelMatrixUniform> globalModelMatrix);

        void onDraw(wgpu::RenderPassEncoder &renderPass, int vertexBufferSlot, int bindGroupSlot);
    };
}

const char CHUNK_VISIBLE = 0x00;
const char CHUNK_HIDDEN = 0x01;

class Chunk {
protected:
    Context *c;
    Scene *s;
    Chunks &chunks;
    
    // Internal state machine
    bool should_build_mesh = false;
    bool should_unload = false;
    std::atomic<char> state = CHUNK_INTERNAL_UNLOADED;

    std::shared_ptr<VoxelMesh> mesh = nullptr;
    std::shared_ptr<tinyrender::ModelMatrixUniform> globalModelMatrix = nullptr;
    std::unique_ptr<GPU_CHUNK> gpu = nullptr;

    void refreshNeighbours();
    void buildMeshAsync();
public:
    ivec2 chunkCoordinate; // {i, j} in chunk space
    ivec2 cornerCoordinate; // {x, y} in world space of corner

    Chunk(Context *c, Scene *s, Chunks &chunks, ivec2 chunkCoordinate, std::shared_ptr<tinyrender::ModelMatrixUniform> globalModelMatrix);
    
    void onUpdate(ivec2 cameraChunk);
    void onDraw(wgpu::RenderPassEncoder &renderPass, int vertexBufferSlot, int bindGroupSlot);
    
    array<char, N_VOXELS> voxels;

    void setVisibility(const char state);
    bool isVisible();

    void set(ivec3 voxel, char value);
    void shouldRefreshMesh();
};
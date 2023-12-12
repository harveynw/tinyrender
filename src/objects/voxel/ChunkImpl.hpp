#pragma once

#include <array>
#include <thread>

#include "BuildMesh.hpp"
#include "VoxelData.hpp"
#include "../ObjectResources.hpp"
#include "../VoxelsImpl.hpp"
#include "../loaders/Polygons.hpp"
#include "../../webgpu/primitives/buffers/AttributeBuffer.hpp"
#include "objects/voxel/Chunk.hpp"
#include "objects/voxel/ChunkGeometry.hpp"


// Forward declare
struct GPU_CHUNK;

class ChunkImpl {
protected:
    Context *c;
    Scene *s;
    VoxelsImpl &v;
    
    // Internal state machine (+ signals)
    bool should_build_mesh = false; // Used as a signal to go from UNLOADED->LOADED as well as refreshing the mesh in LOADED state
    bool should_unload = false; // When LOADED state -> UNLOADED state
    std::atomic<char> state = 0x00; // Start unloaded

    std::shared_ptr<VoxelMesh> mesh = nullptr;
    std::shared_ptr<ModelMatrixUniform> globalModelMatrix = nullptr;
    std::unique_ptr<GPU_CHUNK> gpu;

    void refreshNeighbours();
    void buildMeshAsync();
public:
    ivec2 chunkCoordinate; // {i, j} in chunk space
    ivec2 cornerCoordinate; // {x, y} in world space of corner

    ChunkImpl(Context *c, Scene *s, VoxelsImpl &v, ivec2 chunkCoordinate, std::shared_ptr<ModelMatrixUniform> globalModelMatrix);
    
    void onUpdate();
    void onDraw(wgpu::RenderPassEncoder &renderPass, int vertexBufferSlot, int bindGroupSlot);
    
    array<char, N_VOXELS> voxels;

    void setVisibility(const char state);
    bool isVisible();

    void set(ivec3 voxel, char value);
    void shouldRefreshMesh();

    ~ChunkImpl();
};
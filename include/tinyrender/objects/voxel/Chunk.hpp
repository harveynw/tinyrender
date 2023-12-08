#pragma once

#include "../../Fwd.hpp"
#include "ChunkGeometry.hpp"

const char CHUNK_VISIBLE = 0x00;
const char CHUNK_HIDDEN = 0x01;

// Forward declare
struct GPU_CHUNK;
struct VoxelVertexAttribute;
typedef vector<VoxelVertexAttribute> VoxelMesh;


class Chunk {
protected:
    Context *c;
    Scene *s;
    Chunks &chunks;
    
    // Internal state machine (+ signals)
    bool should_build_mesh = false; // Used as a signal to go from UNLOADED->LOADED as well as refreshing the mesh in LOADED state
    bool should_unload = false; // When LOADED state -> UNLOADED state
    std::atomic<char> state = 0x00; // Start unloaded

    std::shared_ptr<VoxelMesh> mesh = nullptr;
    std::shared_ptr<tinyrender::ModelMatrixUniform> globalModelMatrix = nullptr;
    std::unique_ptr<GPU_CHUNK> gpu;

    void refreshNeighbours();
    void buildMeshAsync();
public:
    ivec2 chunkCoordinate; // {i, j} in chunk space
    ivec2 cornerCoordinate; // {x, y} in world space of corner

    Chunk(Context *c, Scene *s, Chunks &chunks, ivec2 chunkCoordinate, std::shared_ptr<tinyrender::ModelMatrixUniform> globalModelMatrix);
    
    void onUpdate();
    void onDraw(wgpu::RenderPassEncoder &renderPass, int vertexBufferSlot, int bindGroupSlot);
    
    array<char, N_VOXELS> voxels;

    void setVisibility(const char state);
    bool isVisible();

    void set(ivec3 voxel, char value);
    void shouldRefreshMesh();

    ~Chunk();
};
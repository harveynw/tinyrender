#pragma once

#include <stack>
#include <atomic>
#include <thread>

#include "ChunkGeometry.hpp"
#include "VoxelData.hpp"
#include "Mesh.hpp"
#include "../ObjectResources.hpp"
#include "../loaders/Polygons.hpp"
#include "../loaders/Shapes.hpp"
#include "../../webgpu/primitives/buffers/AttributeBuffer.hpp"
#include "../../webgpu/Context.hpp"
#include "../../webgpu/Scene.hpp"

// TODO
// Lighting map 

const char CHUNK_NO_MESH = 0x00;
const char CHUNK_SHOULD_GENERATE = 0x01;
const char CHUNK_GENERATING_MESH = 0x02;
const char CHUNK_MESH_READY = 0x03; 
const char CHUNK_LOADED = 0x04;

class ChunkMap;

class Chunk {
    protected:
        Context *c;
        Scene *s;
        ChunkMap *chunkMap;

        glm::ivec2 cornerCoordinate;

        // Chunk maintains a state machine. onUpdate keeps this machine moving on the main thread
        bool refresh = false; 
        std::atomic_char state = CHUNK_NO_MESH; // Internal state
        std::thread loadingThread;

        // Uploads mesh to the GPU
        std::shared_ptr<Mesh> mesh = nullptr;
        std::shared_ptr<tinyrender::AttributeBuffer> buffer = nullptr;
        std::shared_ptr<ObjectResources> resources = nullptr;
        void syncBuffer();
    public:
        glm::ivec2 chunkCoordinate;

        Chunk(Context *c, Scene *s, ChunkMap *chunkMap, glm::ivec2 chunkCoordinate);
        void onDraw(wgpu::RenderPassEncoder &renderPass, int vertexBufferSlot, int bindGroupSlot);
        void onUpdate(glm::ivec2 cameraChunk);
        void shouldRefresh();

        std::shared_ptr<Mesh> getMesh();
};

class ChunkMap {
protected:
    Context *context;
    Scene *scene;

    std::map<std::string, AdjacentMesh> map;
    std::map<std::string, std::shared_ptr<Chunk>> chunks; 
    std::vector<std::shared_ptr<Chunk>> chunksSequential;
    void updateNeighbours(glm::ivec2 chunkCoordinate);
public:
    ChunkMap(Context *context, Scene *scene);

    std::shared_ptr<Chunk> ensureLoaded(glm::ivec2 chunkCoordinate);
    std::shared_ptr<Chunk> chunkAt(glm::ivec2 chunkCoordinate);
    void refreshNeighbours(glm::ivec2 chunkCoordinate);
    void unloadNeighbours(glm::ivec2 chunkCoordinate);
    AdjacentMesh neighbours(glm::ivec2 chunkCoordinate);
    std::vector<std::shared_ptr<Chunk>> all();
};


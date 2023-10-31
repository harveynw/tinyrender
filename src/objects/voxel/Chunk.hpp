#pragma once

#include <stack>
#include <atomic>
#include <thread>

#include "ChunkGeometry.hpp"
#include "VoxelData.hpp"
#include "../../webgpu/Scene.hpp"
#include "../ObjectResources.hpp"
#include "../loaders/Polygons.hpp"
#include "../loaders/Shapes.hpp"
#include "../../webgpu/primitives/buffers/AttributeBuffer.hpp"
#include "../../webgpu/Context.hpp"

namespace engine {
    class Voxels;
}
class Chunk;

const char CHUNK_NO_MESH = 0x00;
const char CHUNK_GENERATING_MESH = 0x01;
const char CHUNK_MESH_READY = 0x02; 
const char CHUNK_LOADED = 0x03;

struct ChunkNeighbours {
    // Along x axis
    std::weak_ptr<Chunk> west;
    std::weak_ptr<Chunk> east;

    // Along y axis
    std::weak_ptr<Chunk> north;
    std::weak_ptr<Chunk> south;
};

class Chunk {
    protected:
        Context *c;
        Scene *s;

        glm::ivec2 cornerCoordinate;
        std::array<char, N_VOXELS> voxels;

        // onUpdate executes on the main thread, keeps state machine moving
        std::atomic_char state = CHUNK_NO_MESH;
        std::thread loadingThread;
        void onUpdate();

        // CHUNK_NO_MESH -> CHUNK_MESH_READY, generates mesh and can be executed off main thread
        std::unique_ptr<Polygons> mesh;
        void generateMesh();
        
        // CHUNK_MESH_READY -> CHUNK_LOADED, uploads mesh to the GPU
        std::shared_ptr<engine::AttributeBuffer> buffer = nullptr;
        std::shared_ptr<ObjectResources> resources = nullptr;
        void syncBuffer();

        // CHUNK_MESH_READY, CHUNK_LOADED -> CHUNK_NO_MESH
        void unload();

        bool neighbourBlocked(glm::ivec3 outside);

        ChunkNeighbours neighbours{};

        friend class engine::Voxels;
    public:
        Chunk(Context *c, Scene *s, glm::ivec2 chunkCoordinate);
};

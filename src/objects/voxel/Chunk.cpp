#include "Chunk.hpp"


void 
Chunk::onUpdate() {
    switch(auto s = this->state.load()) {
        case CHUNK_NO_MESH: {
            this->state.store(CHUNK_GENERATING_MESH);

            loadingThread = std::thread([&]{
                this->generateMesh();
                this->state.store(CHUNK_MESH_READY);
            }); 
            break;
        }
        case CHUNK_MESH_READY: {
            assert(this->mesh != nullptr);

            this->syncBuffer();
            this->state.store(CHUNK_LOADED);
            break;
        }
        case CHUNK_GENERATING_MESH:
        case CHUNK_LOADED:
        default:
            break;
    }
}

void 
Chunk::generateMesh() {
    if(this->state.load() != CHUNK_GENERATING_MESH)
        throw std::runtime_error("Not in a state to generate mesh");

    printf("syncBuffer with corner (%i, %i)\n", this->cornerCoordinate.x, this->cornerCoordinate.y);
    // Generate list of colored vertices by DFS
    auto start = glm::ivec3(-1, -1, 0);
    Faces faces = visibleFrom(start, this->voxels);
    printf("%i faces found\n", (int) faces.size());

    // Generate attribute date
    this->mesh = std::make_unique<Polygons>();
    for(auto &face : faces) {
        if(neighbourBlocked(face.from))
            continue;

        auto midpoint = (glm::vec3(face.to) + glm::vec3(face.from)) * 0.5f;
        auto plane = glm::ivec3(1, 1, 1) - glm::abs(face.to - face.from);

        glm::vec3 a, b, c, d;

        if(plane.x && plane.y) {
            a = midpoint + glm::vec3(-0.5, -0.5, 0.0);
            b = midpoint + glm::vec3(-0.5, 0.5, 0.0);
            c = midpoint + glm::vec3(0.5, 0.5, 0.0);
            d = midpoint + glm::vec3(0.5, -0.5, 0.0);
        }
        if(plane.y && plane.z) {
            a = midpoint + glm::vec3(0.0, -0.5, -0.5);
            b = midpoint + glm::vec3(0.0, 0.5, -0.5);
            c = midpoint + glm::vec3(0.0, 0.5, 0.5);
            d = midpoint + glm::vec3(0.0, -0.5, 0.5);
        }
        if(plane.x && plane.z) {
            a = midpoint + glm::vec3(-0.5, 0.0, -0.5);
            b = midpoint + glm::vec3(0.5, 0.0, -0.5);
            c = midpoint + glm::vec3(0.5, 0.0, 0.5);
            d = midpoint + glm::vec3(-0.5, 0.0, 0.5);
        }

        Polygons p_new;
        loadQuad(p_new, a, b, c, d);
        *(this->mesh.get()) += p_new;
    }
}

void 
Chunk::syncBuffer() {
    if(this->state.load() != CHUNK_MESH_READY || this->mesh == nullptr)
        throw std::runtime_error("Not in state to update GPU buffer");
    
    buffer = std::make_shared<engine::AttributeBuffer>(this->c, this->mesh->data, this->mesh->vertices);
    printf("Synced %i vertices uploading chunk data\n", (int) this->mesh->vertices);
    resources = std::make_shared<ObjectResources>(this->c, this->s, this->buffer, ColoredTriangle);
    resources->modelMatrix->setTranslation(glm::vec3(this->cornerCoordinate, 0.0));
}

void 
Chunk::unload() {
    if(this->state.load() != CHUNK_LOADED)
        return;
    
    this->buffer = nullptr;
    this->resources = nullptr;
    this->state.store(CHUNK_NO_MESH);
}

#define MOD(a, b) (((a % b) + b) % b)
bool Chunk::neighbourBlocked(glm::ivec3 outside)
{
    std::weak_ptr<Chunk> neighbour;

    if(inBounds(outside))
        return false;

    glm::ivec3 test;
    test = outside + glm::ivec3(1, 0, 0);
    if(inBounds(test))
        neighbour = this->neighbours.west;
    test = outside + glm::ivec3(-1, 0, 0);
    if(inBounds(test)) 
        neighbour = this->neighbours.east;
    test = outside + glm::ivec3(0, 1, 0);
    if(inBounds(test)) 
        neighbour = this->neighbours.south;
    test = outside + glm::ivec3(0, -1, 0);
    if(inBounds(test)) 
        neighbour = this->neighbours.north;
    
    if(neighbour.expired())
        return false; // Neighbour chunk not loaded
    
    std::shared_ptr<Chunk> n = neighbour.lock();
    
    return n->voxels[idx(glm::ivec3(MOD(outside.x, SIZE_X), MOD(outside.y, SIZE_Y), outside.z))] != 0x00;
}

Chunk::Chunk(Context *context, Scene *scene, glm::ivec2 chunkCoordinate): c(context), s(scene) {
    this->cornerCoordinate = glm::ivec2(SIZE_X, SIZE_Y) * chunkCoordinate;

    minecraft(this->voxels, this->cornerCoordinate);
}
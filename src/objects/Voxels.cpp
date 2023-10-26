#include "Voxels.hpp"



namespace {
    // Voxel coordinates to array index
    static int idx(glm::ivec3 c) {
        return c.x + SIZE_X*c.y + SIZE_X*SIZE_Y*c.z;
    }

    // Hash coordinates, note different to idx due to boundary search requirement
    static int hash(glm::ivec3 c) {
        return (c.x+1) + (SIZE_X+2)*(c.y+1) + (SIZE_X+2)*(SIZE_Y+2)*(c.z+1);
    }

    // Unpack array index into voxel coordinates
    static glm::ivec3 coord(int i) {
        int x = i % SIZE_X;
        int y = (i / SIZE_X)%SIZE_Y;
        int z = i / (SIZE_X*SIZE_Y);
        return glm::ivec3(x, y, z);
    }

    struct Face {
        glm::ivec3 from; // May be from outside chunk
        glm::ivec3 to; // Always inside chunk
    };

    typedef std::vector<glm::ivec3> Coordinates;
    typedef std::vector<Face> Faces;

    bool
    inBounds(glm::ivec3 &c) {
        bool x = 0 <= c.x && c.x < SIZE_X;
        bool y = 0 <= c.y && c.y < SIZE_Y;
        bool z = 0 <= c.z && c.z < SIZE_Z;
        return x && y && z;
    }

    Coordinates 
    adjacentCoordinates(glm::ivec3 &c) {
        // Returns adjacent integer coordinates, including a unit padding around the chunk to aid search
        Coordinates adj;
        if(-1 <= c.x - 1)
            adj.push_back(c - glm::ivec3(1, 0, 0));
        if(c.x + 1 < SIZE_X + 1)
            adj.push_back(c + glm::ivec3(1, 0, 0));
        if(-1 <= c.y - 1)
            adj.push_back(c - glm::ivec3(0, 1, 0));
        if(c.y + 1 < SIZE_Y + 1)
            adj.push_back(c + glm::ivec3(0, 1, 0));
        if(-1 <= c.z - 1)
            adj.push_back(c - glm::ivec3(0, 0, 1));
        if(c.z + 1 < SIZE_Z + 1)
            adj.push_back(c + glm::ivec3(0, 0, 1));
        return adj;
    }

    Faces
    visibleFrom(glm::ivec3 &c0, std::array<char, N_VOXELS> &voxels) {
        Faces f;

        // (iterative)-DFS to populate f
        std::unordered_map<int, bool> visited;
        std::stack<glm::ivec3> s;
        s.push(c0);
        while(!s.empty()) {
            auto c = s.top();
            int c_hash = hash(c);
            s.pop();

            if(visited.count(c_hash) > 0)
                continue;
            visited[c_hash] = true;

            for(auto &c_dash : adjacentCoordinates(c)) {
                if(visited.count(hash(c_dash)) > 0)
                    continue;

                if(!inBounds(c_dash) || voxels[idx(c_dash)] == 0x00) {
                    s.push(c_dash); // Air to search from
                } else {
                    f.push_back({c, c_dash}); // Face found
                }
            }
        }

        return f;
    }

    class Chunk {
    protected:
        Context *c;
        std::array<char, N_VOXELS> voxels;

        void syncBuffer();
    public:
        Chunk(Context *c);

        std::shared_ptr<engine::AttributeBuffer> buffer = nullptr;
    };
}

void Chunk::syncBuffer()
{
    // Generate list of colored vertices by DFS
    auto start = glm::ivec3(0, 0, 0);
    Faces faces = visibleFrom(start, this->voxels);
    printf("%i faces found\n", (int) faces.size());

    // Generate attribute date
    Polygons p = {{}, 0};
    for(auto &face : faces) {
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
        p += p_new;
    }

    buffer = std::make_shared<engine::AttributeBuffer>(this->c, p.data, p.vertices);
    printf("Synced %i vertices uploading chunk data\n", (int) p.vertices);
}

Chunk::Chunk(Context *context): c(context)
{
    for(int i = 0; i < N_VOXELS; i++) {
        auto c = coord(i);
        
        // Air
        voxels[i] = 0x00;

        /*
        if(c.x == 4 && c.y == 6 && c.z > 100 && c.z < 104)
            voxels[i] = 0x01;
            */
        // Sphere around centre
        auto cc = glm::vec3(c) - glm::vec3(SIZE_X/2.0, SIZE_Y/2.0, SIZE_Z/2.0);
        float r = glm::sqrt(glm::dot(cc, cc));
        if(r < 6.0) {
            voxels[i] = 0x01;
        }
    }

    syncBuffer();
}

engine::Voxels::Voxels(Context *c, Scene *s)
{
    this->context = c;
    this->scene = s;

    // Temporarily one chunk
    chunk = std::make_shared<Chunk>(this->context);
    this->resources = std::make_shared<ObjectResources>(this->context, this->scene, this->chunk->buffer, ColoredTriangle);
}

void engine::Voxels::update(float dt)
{
    (void) dt;
}

engine::Voxels::~Voxels()
{
}
#include "ChunkGeometry.hpp"

DIRECTION 
opposite(DIRECTION d) {
    switch(d) {
        case NORTH:
            return SOUTH;
        case SOUTH:
            return NORTH;
        case WEST:
            return EAST;
        case EAST:
            return WEST;
    }
    throw std::runtime_error("Unrecognised direction");
}

glm::ivec2 
chunkDirection(DIRECTION d) {
    return glm::ivec2(d == EAST ? 1 : (d == WEST ? -1 : 0), d == NORTH ? 1 : (d == SOUTH ? -1 : 0));
}

int idx(glm::ivec3 c)
{
    // Voxel coordinates to array index
    return c.x + SIZE_XY*c.y + SIZE_XY*SIZE_XY*c.z;
}

int 
hash(glm::ivec3 c) {
    // Hash coordinates, note different to idx due to boundary search requirement
    return (c.x+1) + (SIZE_XY+2)*(c.y+1) + (SIZE_XY+2)*(SIZE_XY+2)*(c.z+1);
}

glm::ivec3 
coord(int i) {
    // Unpack array index into voxel coordinates
    int x = i % SIZE_XY;
    int y = (i / SIZE_XY)%SIZE_XY;
    int z = i / (SIZE_XY*SIZE_XY);
    return glm::ivec3(x, y, z);
}

bool
inBounds(glm::ivec3 &c) {
    bool x = 0 <= c.x && c.x < SIZE_XY;
    bool y = 0 <= c.y && c.y < SIZE_XY;
    bool z = 0 <= c.z && c.z < SIZE_Z;
    return x && y && z;
}

Coordinates 
adjacentCoordinates(glm::ivec3 &c) {
    // Returns adjacent integer coordinates, including a unit padding around the chunk to aid search
    Coordinates adj;
    if(-1 <= c.x - 1)
        adj.push_back(c - glm::ivec3(1, 0, 0));
    if(c.x + 1 < SIZE_XY + 1)
        adj.push_back(c + glm::ivec3(1, 0, 0));
    if(-1 <= c.y - 1)
        adj.push_back(c - glm::ivec3(0, 1, 0));
    if(c.y + 1 < SIZE_XY + 1)
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
                // Face found
                f.push_back({c, c_dash}); 
            }
        }
    }

    return f;
}

bool 
isTransparent(glm::ivec3 &c, std::array<char, N_VOXELS> &voxels) {
    return voxels[idx(c)] == 0x00;
}

bool 
onBoundary(glm::ivec3 &c) {
    return c.x == 0 || c.x == SIZE_XY-1 || c.y == 0 || c.y == SIZE_XY-1 || c.z == 0 || c.z == SIZE_Z-1;
}

glm::ivec3 
wrapCoordinate(glm::ivec3 &c) {
    return glm::ivec3(MOD(c.x, SIZE_XY), MOD(c.y, SIZE_XY), c.z);
}

std::string 
serializeChunkCoord(glm::ivec2 coord) {
    std::ostringstream ss;
    ss << coord.x << "," << coord.y;
    return ss.str();
}

bool 
visible(glm::ivec2 chunkA, glm::ivec2 chunkB) {
    int dx = abs(chunkA.x - chunkB.x);
    int dy = abs(chunkA.y - chunkB.y);
    return dx <= VISIBILITY_DISTANCE && dy <= VISIBILITY_DISTANCE;
}

std::vector<glm::ivec2> 
visibleFrom(glm::ivec2 chunk)
{
    std::vector<glm::ivec2> visible;
    for(int i = -VISIBILITY_DISTANCE; i <= VISIBILITY_DISTANCE; i++) {
        for(int j = -VISIBILITY_DISTANCE; j <= VISIBILITY_DISTANCE; j++)
            visible.emplace_back(i+chunk.x, j+chunk.y);
    }
    return visible;
}

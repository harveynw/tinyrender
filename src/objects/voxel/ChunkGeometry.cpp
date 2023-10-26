#include "ChunkGeometry.hpp"


int 
idx(glm::ivec3 c) {
    // Voxel coordinates to array index
    return c.x + SIZE_X*c.y + SIZE_X*SIZE_Y*c.z;
}

int 
hash(glm::ivec3 c) {
    // Hash coordinates, note different to idx due to boundary search requirement
    return (c.x+1) + (SIZE_X+2)*(c.y+1) + (SIZE_X+2)*(SIZE_Y+2)*(c.z+1);
}

glm::ivec3 
coord(int i) {
    // Unpack array index into voxel coordinates
    int x = i % SIZE_X;
    int y = (i / SIZE_X)%SIZE_Y;
    int z = i / (SIZE_X*SIZE_Y);
    return glm::ivec3(x, y, z);
}

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

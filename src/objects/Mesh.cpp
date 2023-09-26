#include "Mesh.hpp"

engine::Mesh::Mesh(Context *c, Scene *s, const std::string& path) {
    this->context = c;
    this->scene = s;

    // Populate this->data
    if(STR_SUFFIX(path, std::string(".obj")))
        loadFromObjFile(path);
    else
        throw std::runtime_error("Mesh: unsupported file type");
}

void
engine::Mesh::loadFromObjFile(std::string path) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;

    // Call the core loading procedure of TinyOBJLoader
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str());

    // Check errors
    if (!warn.empty())
        std::cout << warn << std::endl;

    if (!err.empty())
        std::cerr << err << std::endl;

    if (!ret)
        throw std::runtime_error("Unable to load OBJ");

    // Filling in vertexData:
    std::vector<UVTriangleVertexAttributes> vertexData;
    for (const auto& shape : shapes) {
        size_t offset = vertexData.size();
        vertexData.resize(offset + shape.mesh.indices.size());

        for (size_t i = 0; i < shape.mesh.indices.size(); ++i) {
            const tinyobj::index_t& idx = shape.mesh.indices[i];

            vertexData[offset + i].position = {
                    attrib.vertices[3 * idx.vertex_index + 0],
                    -attrib.vertices[3 * idx.vertex_index + 2], // Add a minus to avoid mirroring
                    attrib.vertices[3 * idx.vertex_index + 1]
            };

            // Also apply the transform to normals!!
            vertexData[offset + i].normal = {
                    attrib.normals[3 * idx.normal_index + 0],
                    -attrib.normals[3 * idx.normal_index + 2],
                    attrib.normals[3 * idx.normal_index + 1]
            };

            vertexData[offset + i].color = {
                    attrib.colors[3 * idx.vertex_index + 0],
                    attrib.colors[3 * idx.vertex_index + 1],
                    attrib.colors[3 * idx.vertex_index + 2]
            };

            vertexData[offset + i].uv = {
                    attrib.texcoords[2 * idx.texcoord_index + 0],
                    1 - attrib.texcoords[2 * idx.texcoord_index + 1]
            };
        }
    }

    auto *data = reinterpret_cast<float *>(vertexData.data()); // Flatten
    int n_data = sizeof(UVTriangleVertexAttributes) * vertexData.size();
    std::vector<float> v(data, data + n_data);

    auto attrs = std::make_shared<engine::AttributeBuffer>(this->context, v, vertexData.size());
    this->resources = std::make_shared<ObjectResources>(this->context, this->scene, attrs, ColoredTriangle);
}

#include "Obj.hpp"


namespace {

    void
    load(tinyobj::ObjReader &reader, Polygons &p) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error();
            exit(1);
        }
        if (!reader.Warning().empty())
            std::cout << "TinyObjReader: " << reader.Warning();

        auto &attrib = reader.GetAttrib();
        auto &shapes = reader.GetShapes();
        //auto& materials = reader.GetMaterials();

        // Filling in vertexData:
        std::vector<UVTriangleVertexAttributes> vertexData;
        for (const auto &shape: shapes) {
            size_t offset = vertexData.size();
            vertexData.resize(offset + shape.mesh.indices.size());

            for (size_t i = 0; i < shape.mesh.indices.size(); ++i) {
                const tinyobj::index_t &idx = shape.mesh.indices[i];

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

        p.data = std::vector<float>(data, data + n_data);
        p.vertices = vertexData.size();
    }

}

void
loadFromObjFile(std::string path, Polygons &p) {
    tinyobj::ObjReader reader;
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./"; // Path to material files

    reader.ParseFromFile(path, reader_config);
    load(reader, p);
}

void
loadFromObj(std::string defn, Polygons &p) {
    tinyobj::ObjReader reader;

    reader.ParseFromString(defn, "");
    load(reader, p);
}
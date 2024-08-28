#include "Model.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <unordered_map>
#include <fstream>

Model::Model(const std::string& modelPath, const std::string& texturePath) {
    this->directory = "resources/textures"; // Update directory for textures
    loadModel(modelPath);
    loadTexture(texturePath);
}

void Model::draw(const Shader& shader) const {
    for (const auto& mesh : meshes)
        mesh.draw(shader);
}

void Model::loadModel(const std::string& path) {
    stbi_set_flip_vertically_on_load(true);

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str(), nullptr, true);

    if (!warn.empty()) {
        std::cout << "WARN: " << warn << '\n';
    }

    if (!err.empty()) {
        std::cerr << err << '\n';
    }

    if (!ret) {
        std::cerr << "Failed to load/parse .obj." << '\n';
        return;
    }

    for (const auto& shape : shapes) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        std::unordered_map<Vertex, uint32_t> uniqueVertices = {};

        for (const auto& index : shape.mesh.indices) {
            Vertex vertex = {};

            vertex.Position = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            if (index.normal_index >= 0) {
                vertex.Normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                };
            }

            if (index.texcoord_index >= 0) {
                vertex.TexCoords = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1]
                };
            }

            if (!uniqueVertices.contains(vertex)) {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(uniqueVertices[vertex]);
        }

        Mesh mesh(vertices, indices, textures);
        meshes.push_back(mesh);
    }
}

void Model::loadTexture(const std::string& path) {
    const std::string fullPath = directory + '/' + path;
    std::cout << "Loading texture: " << fullPath << '\n';

    Texture texture;
    texture.Id = textureFromFile(fullPath.c_str(), directory);
    texture.Type = "texture_diffuse";
    texture.Path = path;
    loadedTextures.push_back(texture);

    // Apply the texture to all meshes
    for (auto& mesh : meshes) {
        mesh.Textures.push_back(texture);
    }
}

unsigned int textureFromFile(const char* path, const std::string& directory, bool gamma) {
    const auto filename = std::string(path);

    unsigned int textureId;
    glGenTextures(1, &textureId);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format = 0;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else {
        std::cerr << "Texture failed to load at path: " << filename << '\n';
        stbi_image_free(data);
    }

    return textureId;
}

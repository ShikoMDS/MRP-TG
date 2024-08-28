#pragma once

#include "Shader.h"
#include <glew.h>
#include <glm.hpp>
#include <string>
#include <vector>
#include <functional> // Include for std::hash

struct Vertex {
    glm::vec3 Position;  // Vertex position
    glm::vec3 Normal;    // Vertex normal
    glm::vec2 TexCoords; // Texture coordinates

    bool operator==(const Vertex& other) const {
        return Position == other.Position && Normal == other.Normal && TexCoords == other.TexCoords;
    }
};

// Custom hash functions for glm::vec2 and glm::vec3
namespace std {
    template<>
    struct hash<glm::vec2> {
        size_t operator()(const glm::vec2& v) const noexcept {
            return hash<float>()(v.x) ^ (hash<float>()(v.y) << 1);
        }
    };

    template<>
    struct hash<glm::vec3> {
        size_t operator()(const glm::vec3& v) const noexcept {
            return hash<float>()(v.x) ^ (hash<float>()(v.y) << 1) ^ (hash<float>()(v.z) << 2);
        }
    };

    template<>
    struct hash<Vertex> {
        size_t operator()(const Vertex& vertex) const noexcept {
            return ((hash<glm::vec3>()(vertex.Position) ^
                (hash<glm::vec3>()(vertex.Normal) << 1)) >> 1) ^
                (hash<glm::vec2>()(vertex.TexCoords) << 1);
        }
    };
}

struct Texture {
    unsigned int Id;    // Texture ID
    std::string Type;   // Texture type (e.g., diffuse, specular)
    std::string Path;   // Texture file path
};

class Mesh {
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    void draw(const Shader& shader) const;

    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    std::vector<Texture> Textures;

private:
    void setupMesh();

    unsigned int VAO, VBO, EBO;
};


#pragma once

#include "Shader.h"
#include "Mesh.h"

#include <glew.h>
#include <glm.hpp>
#include <string>
#include <vector>

class Model {
public:
    Model(const std::string& modelPath, const std::string& texturePath);

    void draw(const Shader& shader) const;

private:
    void loadModel(const std::string& path);
    void loadTexture(const std::string& path);

    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> loadedTextures;
};

unsigned int textureFromFile(const char* path, const std::string& directory, bool gamma = false);

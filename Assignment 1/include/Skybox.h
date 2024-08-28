#pragma once

#include "Shader.h"

#include <glew.h>
#include <glm.hpp>
#include <vector>

class Skybox {
public:
    explicit Skybox(const std::vector<std::string>& faces);

    void draw(const Shader& shader) const;

private:
    void setupSkybox();
    static unsigned int loadCubeMap(const std::vector<std::string>& faces);

    unsigned int vao;
    unsigned int vbo;
    unsigned int cubeMapTexture;
};

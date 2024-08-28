#include "Skybox.h"
#include "stb_image.h"
#include <iostream>

Skybox::Skybox(const std::vector<std::string>& faces) {
    cubeMapTexture = loadCubeMap(faces);
    setupSkybox();
}

void Skybox::draw(const Shader& shader) const {
    glDepthFunc(GL_LEQUAL); // Ensure depth test passes when depth values are equal to set depth
    shader.use();
    glBindVertexArray(vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // Reset depth function to default
}

void Skybox::setupSkybox() {
    constexpr float skyboxVertices[] = {
        // Positions          
        -1.0f, 1.0f, -1.0f,  -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,  1.0f, 1.0f, -1.0f,  -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f,  -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,  -1.0f, 1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,  1.0f, -1.0f, 1.0f,  1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,  1.0f, 1.0f, -1.0f,  1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,  1.0f, 1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,  -1.0f, 1.0f, 1.0f,  -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,  -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,  -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr));
    glBindVertexArray(0);
}

unsigned int Skybox::loadCubeMap(const std::vector<std::string>& faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else {
            std::cerr << "Cubemap texture failed to load at path: " << faces[i] << '\n';
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

#pragma once

#include "Shader.h"

#include <glew.h>
#include <glm.hpp>
#include <vector>

class Skybox
{
public:
	explicit Skybox(const std::vector<std::string>& Faces);

	void draw(const Shader& Shader) const;

private:
	void setupSkybox();
	static unsigned int loadCubeMap(const std::vector<std::string>& Faces);

	unsigned int Vao;
	unsigned int Vbo;
	unsigned int CubeMapTexture;
};

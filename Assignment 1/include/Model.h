#pragma once

#include "Shader.h"
#include "Mesh.h"

#include <glew.h>
#include <glm.hpp>
#include <string>
#include <vector>

class Model
{
public:
	Model(const std::string& ModelPath, const std::string& TexturePath);

	void draw(const Shader& Shader) const;

private:
	void loadModel(const std::string& Path);
	void loadTexture(const std::string& Path);

	std::vector<Mesh> MMeshes;
	std::string MDirectory;
	std::vector<Texture> MLoadedTextures;
};

unsigned int textureFromFile(const char* Path, const std::string& Directory, bool Gamma = false);

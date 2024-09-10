#include "Skybox.h"
#include "stb_image.h"
#include <iostream>

Skybox::Skybox(const std::vector<std::string>& Faces)
{
	CubeMapTexture = loadCubeMap(Faces);
	setupSkybox();
}

void Skybox::draw(const Shader& Shader) const
{
	glDepthFunc(GL_LEQUAL); // Ensure depth test passes when depth values are equal to set depth
	Shader.use();
	glBindVertexArray(Vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // Reset depth function to default
}

void Skybox::setupSkybox()
{
	constexpr float SkyboxVertices[] = {
		// Positions          
		-1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f
	};

	glGenVertexArrays(1, &Vao);
	glGenBuffers(1, &Vbo);
	glBindVertexArray(Vao);
	glBindBuffer(GL_ARRAY_BUFFER, Vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SkyboxVertices), &SkyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr));
	glBindVertexArray(0);
}

unsigned int Skybox::loadCubeMap(const std::vector<std::string>& Faces)
{
	unsigned int TextureId;
	glGenTextures(1, &TextureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TextureId);

	int Width, Height, NrChannels;
	for (unsigned int I = 0; I < Faces.size(); I++)
	{
		unsigned char* Data = stbi_load(Faces[I].c_str(), &Width, &Height, &NrChannels, 0);
		if (Data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + I, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE,
			             Data);
			stbi_image_free(Data);
		}
		else
		{
			std::cerr << "Cubemap texture failed to load at path: " << Faces[I] << '\n';
			stbi_image_free(Data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return TextureId;
}

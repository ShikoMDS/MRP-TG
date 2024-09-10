#include "Model.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <unordered_map>
#include <fstream>

Model::Model(const std::string& ModelPath, const std::string& TexturePath)
{
	this->MDirectory = "resources/textures"; // Update directory for textures
	loadModel(ModelPath);
	loadTexture(TexturePath);
}

void Model::draw(const Shader& Shader) const
{
	for (const auto& Mesh : MMeshes)
		Mesh.draw(Shader);
}

void Model::loadModel(const std::string& Path)
{
	stbi_set_flip_vertically_on_load(true);

	tinyobj::attrib_t Attrib;
	std::vector<tinyobj::shape_t> Shapes;
	std::vector<tinyobj::material_t> Materials;
	std::string Warn, Err;

	bool Ret = LoadObj(&Attrib, &Shapes, &Materials, &Warn, &Err, Path.c_str(), nullptr, true);

	if (!Warn.empty())
	{
		std::cout << "WARN: " << Warn << '\n';
	}

	if (!Err.empty())
	{
		std::cerr << Err << '\n';
	}

	if (!Ret)
	{
		std::cerr << "Failed to load/parse .obj." << '\n';
		return;
	}

	for (const auto& Shape : Shapes)
	{
		std::vector<Vertex> Vertices;
		std::vector<unsigned int> Indices;
		std::vector<Texture> Textures;

		std::unordered_map<Vertex, uint32_t> UniqueVertices = {};

		for (const auto& Index : Shape.mesh.indices)
		{
			Vertex Vertex = {};

			Vertex.Position = {
				Attrib.vertices[3 * Index.vertex_index + 0],
				Attrib.vertices[3 * Index.vertex_index + 1],
				Attrib.vertices[3 * Index.vertex_index + 2]
			};

			if (Index.normal_index >= 0)
			{
				Vertex.Normal = {
					Attrib.normals[3 * Index.normal_index + 0],
					Attrib.normals[3 * Index.normal_index + 1],
					Attrib.normals[3 * Index.normal_index + 2]
				};
			}

			if (Index.texcoord_index >= 0)
			{
				Vertex.TexCoords = {
					Attrib.texcoords[2 * Index.texcoord_index + 0],
					Attrib.texcoords[2 * Index.texcoord_index + 1]
				};
			}

			if (!UniqueVertices.contains(Vertex))
			{
				UniqueVertices[Vertex] = static_cast<uint32_t>(Vertices.size());
				Vertices.push_back(Vertex);
			}

			Indices.push_back(UniqueVertices[Vertex]);
		}

		Mesh LMesh(Vertices, Indices, Textures);
		MMeshes.push_back(LMesh);
	}
}

void Model::loadTexture(const std::string& Path)
{
	const std::string FullPath = MDirectory + '/' + Path;
	std::cout << "Loading texture: " << FullPath << '\n';

	Texture LTexture;
	LTexture.Id = textureFromFile(FullPath.c_str(), MDirectory);
	LTexture.Type = "texture_diffuse";
	LTexture.Path = Path;
	MLoadedTextures.push_back(LTexture);

	// Apply the texture to all meshes
	for (auto& Mesh : MMeshes)
	{
		Mesh.Textures.push_back(LTexture);
	}
}

unsigned int textureFromFile(const char* Path, const std::string& Directory, bool Gamma)
{
	const auto Filename = std::string(Path);

	unsigned int TextureId;
	glGenTextures(1, &TextureId);

	int Width, Height, NrComponents;
	unsigned char* Data = stbi_load(Filename.c_str(), &Width, &Height, &NrComponents, 0);
	if (Data)
	{
		GLenum Format = 0;
		if (NrComponents == 1)
			Format = GL_RED;
		else if (NrComponents == 3)
			Format = GL_RGB;
		else if (NrComponents == 4)
			Format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, TextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, Format, Width, Height, 0, Format, GL_UNSIGNED_BYTE, Data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(Data);
	}
	else
	{
		std::cerr << "Texture failed to load at path: " << Filename << '\n';
		stbi_image_free(Data);
	}

	return TextureId;
}

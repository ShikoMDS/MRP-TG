#pragma once

#include "Shader.h"
#include <glew.h>
#include <glm.hpp>
#include <string>
#include <vector>
#include <functional>

struct Vertex
{
	glm::vec3 Position; // Vertex position
	glm::vec3 Normal; // Vertex normal
	glm::vec2 TexCoords; // Texture coordinates

	bool operator==(const Vertex& Other) const
	{
		return Position == Other.Position && Normal == Other.Normal && TexCoords == Other.TexCoords;
	}
};

// Custom hash functions for glm::vec2 and glm::vec3
namespace std
{
	template <>
	struct hash<glm::vec2>
	{
		size_t operator()(const glm::vec2& V) const noexcept
		{
			return hash<float>()(V.x) ^ (hash<float>()(V.y) << 1);
		}
	};

	template <>
	struct hash<glm::vec3>
	{
		size_t operator()(const glm::vec3& v) const noexcept
		{
			return hash<float>()(v.x) ^ (hash<float>()(v.y) << 1) ^ (hash<float>()(v.z) << 2);
		}
	};

	template <>
	struct hash<Vertex>
	{
		size_t operator()(const Vertex& Vertex) const noexcept
		{
			return ((hash<glm::vec3>()(Vertex.Position) ^
					(hash<glm::vec3>()(Vertex.Normal) << 1)) >> 1) ^
				(hash<glm::vec2>()(Vertex.TexCoords) << 1);
		}
	};
}

struct Texture
{
	unsigned int Id; // Texture ID
	std::string Type; // Texture type
	std::string Path; // Texture file path
};

class Mesh
{
public:
	Mesh(std::vector<Vertex> Vertices, std::vector<unsigned int> Indices, std::vector<Texture> Textures);

	void draw(const Shader& Shader) const;

	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;
	std::vector<Texture> Textures;

private:
	void setupMesh();

	unsigned int Vao, Vbo, Ebo;
};

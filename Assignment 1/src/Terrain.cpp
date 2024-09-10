#include "Terrain.h"
#include <stb_image.h>
#include <iostream>

// Load heightmap from file using STB image
std::vector<float> loadHeightmap(const std::string& Filepath, int& Width, int& Height)
{
	int Channels;
	unsigned char* Data = stbi_load(Filepath.c_str(), &Width, &Height, &Channels, 1); // Load as grayscale (1 channel)

	if (!Data)
	{
		std::cerr << "Failed to load heightmap: " << Filepath << std::endl;
		return {};
	}

	std::vector<float> Heightmap(Width * Height);

	// Normalize heightmap data between 0.0f and 1.0f
	for (int Y = 0; Y < Height; ++Y)
	{
		for (int X = 0; X < Width; ++X)
		{
			const int Index = Y * Width + X;
			Heightmap[Index] = Data[Index] / 255.0f; // Normalize the grayscale value
		}
	}

	stbi_image_free(Data);
	return Heightmap;
}

// Generate terrain vertices using the loaded heightmap data
std::vector<Vertex> generateTerrain(const std::vector<float>& Heightmap, const int Width, const int Height,
                                    const float TerrainScale, const float HeightScale)
{
	std::vector<Vertex> Vertices;

	for (int y = 0; y < Height; ++y)
	{
		for (int X = 0; X < Width; ++X)
		{
			Vertex Vertex;

			// Position of the vertex (X, Y from heightmap, Z)
			Vertex.VPosition = glm::vec3(
				static_cast<float>(X) * TerrainScale, // X position
				Heightmap[y * Width + X] * HeightScale, // Height (Y)
				static_cast<float>(y) * TerrainScale // Z position
			);

			// Texture coordinates
			Vertex.VTexCoords = glm::vec2(
				static_cast<float>(X) / static_cast<float>(Width), // U
				static_cast<float>(y) / static_cast<float>(Height // V
				) // V
			);

			// Set a placeholder normal (computed later)
			Vertex.VNormal = glm::vec3(0.0f, 1.0f, 0.0f); // Default Y-up normal

			Vertices.push_back(Vertex);
		}
	}

	return Vertices;
}

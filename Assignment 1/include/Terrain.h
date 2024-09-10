#pragma once

#include <vector>
#include <glm.hpp>
#include <string>

// Structure to hold the terrain vertex data
struct Vertex
{
	glm::vec3 VPosition;
	glm::vec3 VNormal;
	glm::vec2 VTexCoords;
};

// Function declarations for loading heightmap and generating terrain vertices
std::vector<float> loadHeightmap(const std::string& Filepath, int& Width, int& Height);
std::vector<Vertex> generateTerrain(const std::vector<float>& Heightmap, int Width, int Height, float TerrainScale,
                                    float HeightScale);

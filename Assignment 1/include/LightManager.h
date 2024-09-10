#pragma once

#include "Shader.h"
#include <glm.hpp>

struct DirectionalLight
{
	glm::vec3 Direction;
	glm::vec3 Colour;
	float AmbientStrength;
};

class LightManager
{
public:
	LightManager();
	void initialize();
	void updateLighting(const Shader& Shader) const;
	void setDirectionalLight(const glm::vec3& Direction, const glm::vec3& Color, float AmbientStrength);

private:
	DirectionalLight MDirectionalLight;
	bool MDirectionalLightOn;
};

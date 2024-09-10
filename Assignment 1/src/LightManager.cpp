#include "LightManager.h"

LightManager::LightManager() : MDirectionalLight(), MDirectionalLightOn(true)
{
}

void LightManager::initialize()
{
	MDirectionalLight = {glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(1.0f, 1.0f, 1.0f), 0.1f};
}

void LightManager::updateLighting(const Shader& Shader) const
{
	if (MDirectionalLightOn)
	{
		Shader.setVec3("directionalLight.direction", MDirectionalLight.Direction);
		Shader.setVec3("directionalLight.color", MDirectionalLight.Colour);
		Shader.setFloat("directionalLight.ambientStrength", MDirectionalLight.AmbientStrength);
	}
}

void LightManager::setDirectionalLight(const glm::vec3& Direction, const glm::vec3& Color, const float AmbientStrength)
{
	MDirectionalLight.Direction = Direction;
	MDirectionalLight.Colour = Color;
	MDirectionalLight.AmbientStrength = AmbientStrength;
}

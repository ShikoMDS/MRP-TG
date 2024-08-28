#include "LightManager.h"

LightManager::LightManager() : MDirectionalLightOn(true) {}

void LightManager::initialize() {
    MDirectionalLight = { glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(1.0f, 1.0f, 1.0f), 0.1f };
}

void LightManager::updateLighting(const Shader& Shader) const {
    if (MDirectionalLightOn) {
        Shader.setVec3("directionalLight.direction", MDirectionalLight.Direction);
        Shader.setVec3("directionalLight.color", MDirectionalLight.Colour);
        Shader.setFloat("directionalLight.ambientStrength", MDirectionalLight.AmbientStrength);
    }
}

void LightManager::setDirectionalLight(const glm::vec3& direction, const glm::vec3& color, float ambientStrength) {
    MDirectionalLight.Direction = direction;
    MDirectionalLight.Colour = color;
    MDirectionalLight.AmbientStrength = ambientStrength;
}

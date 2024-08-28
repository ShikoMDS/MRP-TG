#pragma once

#include "Shader.h"
#include <glm.hpp>

struct DirectionalLight {
    glm::vec3 Direction;
    glm::vec3 Colour;
    float AmbientStrength;
};

class LightManager {
public:
    LightManager();
    void initialize();
    void updateLighting(const Shader& Shader) const;
    void setDirectionalLight(const glm::vec3& direction, const glm::vec3& color, float ambientStrength);

private:
    DirectionalLight MDirectionalLight;
    bool MDirectionalLightOn;
};

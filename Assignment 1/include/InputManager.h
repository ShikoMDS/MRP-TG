#pragma once

#include "Camera.h"
#include "LightManager.h"
#include <glew.h>
#include <glfw3.h>
#include <unordered_map>
#include <iostream> // Include for std::cout

class InputManager {
public:
    InputManager(Camera& Camera, LightManager& LightManager);

    void processInput(GLFWwindow* Window, float DeltaTime);
    static void frameBufferSizeCallback(GLFWwindow* Window, int Width, int Height);
    static void mouseCallback(GLFWwindow* Window, double PosX, double PosY);
    static void scrollCallback(GLFWwindow* Window, double OffsetX, double OffsetY);

private:
    Camera& MCamera;
    LightManager& MLightManager;
    bool MFirstMouse;
    float MLastX;
    float MLastY;

    static InputManager* instance; // Static instance for callback access
};

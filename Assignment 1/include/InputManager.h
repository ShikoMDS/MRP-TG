#pragma once

#include "Camera.h"
#include "LightManager.h"
#include <glew.h>
#include <glfw3.h>
#include <unordered_map>
#include <iostream>

enum Scene
{
	SceneNone = 0,
	Scene1,
	Scene2,
	Scene3,
	Scene4
};

class InputManager
{
public:
	InputManager(Camera& Camera, LightManager& LightManager);

	void processInput(GLFWwindow* Window, float DeltaTime) const;
	static void frameBufferSizeCallback(GLFWwindow* Window, int Width, int Height);
	static void mouseCallback(GLFWwindow* Window, double PosX, double PosY);
	static void scrollCallback(GLFWwindow* Window, double OffsetX, double OffsetY);

private:
	Camera& MCamera;
	LightManager& MLightManager;
	bool MFirstMouse;
	float MLastX;
	float MLastY;

	static InputManager* MInstance; // Static instance for callback access
};

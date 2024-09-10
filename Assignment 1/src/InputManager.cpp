#include "InputManager.h"

InputManager* InputManager::MInstance = nullptr;

InputManager::InputManager(Camera& Camera, LightManager& LightManager)
	: MCamera(Camera), MLightManager(LightManager), MFirstMouse(true), MLastX(400), MLastY(300)
{
	MInstance = this; // Set static instance for callback access
}

void resetScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	// Reset states
}

void InputManager::processInput(GLFWwindow* Window, const float DeltaTime) const
{
	static Scene CurrentScene = SceneNone; // Track the active scene

	if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(Window, true);

	// Handle scene switching
	if (glfwGetKey(Window, GLFW_KEY_1) == GLFW_PRESS)
	{
		CurrentScene = Scene1;
		std::cout << "Switching to Scene 1: Stencil Test" << '\n';
		resetScene(); // Reset the scene before switching
	}
	if (glfwGetKey(Window, GLFW_KEY_2) == GLFW_PRESS)
	{
		CurrentScene = Scene2;
		std::cout << "Switching to Scene 2: Terrain Rendering" << '\n';
		resetScene();
	}
	if (glfwGetKey(Window, GLFW_KEY_3) == GLFW_PRESS)
	{
		CurrentScene = Scene3;
		std::cout << "Switching to Scene 3" << '\n';
		resetScene();
	}
	if (glfwGetKey(Window, GLFW_KEY_4) == GLFW_PRESS)
	{
		CurrentScene = Scene4;
		std::cout << "Switching to Scene 4" << '\n';
		resetScene();
	}

	// Camera movement handling
	if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
		MCamera.processKeyboard(Forward, DeltaTime);
	if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
		MCamera.processKeyboard(Backward, DeltaTime);
	if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
		MCamera.processKeyboard(Left, DeltaTime);
	if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
		MCamera.processKeyboard(Right, DeltaTime);
	if (glfwGetKey(Window, GLFW_KEY_E) == GLFW_PRESS)
		MCamera.processKeyboard(Up, DeltaTime);
	if (glfwGetKey(Window, GLFW_KEY_Q) == GLFW_PRESS)
		MCamera.processKeyboard(Down, DeltaTime);
}

void InputManager::frameBufferSizeCallback(GLFWwindow* Window, const int Width, const int Height)
{
	glViewport(0, 0, Width, Height);
}

void InputManager::mouseCallback(GLFWwindow* Window, const double PosX, const double PosY)
{
	if (MInstance)
	{
		if (MInstance->MFirstMouse)
		{
			MInstance->MLastX = static_cast<float>(PosX);
			MInstance->MLastY = static_cast<float>(PosY);
			MInstance->MFirstMouse = false;
		}

		const float OffsetX = static_cast<float>(PosX) - MInstance->MLastX;
		const float OffsetY = MInstance->MLastY - static_cast<float>(PosY);
		MInstance->MLastX = static_cast<float>(PosX);
		MInstance->MLastY = static_cast<float>(PosY);

		MInstance->MCamera.processMouseMovement(OffsetX, OffsetY);
	}
}

void InputManager::scrollCallback(GLFWwindow* Window, double OffsetX, const double OffsetY)
{
	if (MInstance)
	{
		MInstance->MCamera.processMouseScroll(static_cast<float>(OffsetY));
	}
}

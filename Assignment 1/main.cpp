#include <glew.h>
#include <glfw3.h>
#include <iostream>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Skybox.h"
#include "InputManager.h"
#include "LightManager.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

// Set up camera
Camera GCamera(glm::vec3(0.0f, 2.0f, 8.0f)); // Position the camera to view the scene

Shader SkyboxShader("resources/shaders/SkyboxVertexShader.vert", "resources/shaders/SkyboxFragmentShader.frag");

// Initialize Skybox
std::vector<std::string> SkyboxFaces = {
	"resources/skybox/Corona/right.png",
	"resources/skybox/Corona/left.png",
	"resources/skybox/Corona/top.png",
	"resources/skybox/Corona/bottom.png",
	"resources/skybox/Corona/back.png",
	"resources/skybox/Corona/front.png"
};

Skybox GSkybox(SkyboxFaces);

void renderSkybox()
{
	// Set up the skybox shader and render the skybox
	glDepthFunc(GL_LEQUAL); // Set depth function for skybox rendering
	SkyboxShader.use();
	const auto View = glm::mat4(glm::mat3(GCamera.getViewMatrix())); // Remove translation
	const glm::mat4 Projection = GCamera.getProjectionMatrix(800, 600);
	SkyboxShader.setMat4("view", View);
	SkyboxShader.setMat4("projection", Projection);
	GSkybox.draw(SkyboxShader);
	glDepthFunc(GL_LESS); // Reset depth function for normal rendering
}

int main()
{
	// Initialize GLFW
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << '\n';
		return -1;
	}

	// Configure OpenGL context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window
	GLFWwindow* Window = glfwCreateWindow(800, 600, "Stencil Test Scene", nullptr, nullptr);
	if (!Window)
	{
		std::cerr << "Failed to create GLFW window" << '\n';
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(Window);

	// Set the cursor to disabled mode (lock and hide)
	glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << '\n';
		return -1;
	}

	// Configure global OpenGL state
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	// Load shaders
	Shader ModelShader("resources/shaders/ModelShader.vert", "resources/shaders/ModelShader.frag");
	Shader OutlineShader("resources/shaders/OutlineShader.vert", "resources/shaders/OutlineShader.frag");

	const std::string PlaceholderTexture = "PolygonAncientWorlds_Texture_01_A.png";

	// Load models
	Model Tree("resources/models/DungeonPack/SM_Env_Tree_Twisted_01.obj", PlaceholderTexture);
	Model Crystal("resources/models/DungeonPack/SM_Prop_Tech_Crystal_01.obj", PlaceholderTexture);
	Model GemLarge("resources/models/DungeonPack/SM_Env_Gem_Large_01.obj", PlaceholderTexture);
	Model GemSpike("resources/models/DungeonPack/SM_Env_Gem_Spike_02.obj", PlaceholderTexture);

	LightManager GLightManager;
	GLightManager.initialize();
	InputManager GInputManager(GCamera, GLightManager);

	// Set up input callbacks
	glfwSetFramebufferSizeCallback(Window, InputManager::frameBufferSizeCallback);
	glfwSetCursorPosCallback(Window, InputManager::mouseCallback);
	glfwSetScrollCallback(Window, InputManager::scrollCallback);

	// Time tracking variables for rotation
	float LastFrame = 0.0f;
	float Angle = 0.0f; // Store the current rotation angle

	// Render loop
	while (!glfwWindowShouldClose(Window))
	{
		// Calculate deltaTime (time between frames)
		float CurrentFrame = static_cast<float>(glfwGetTime());
		float DeltaTime = CurrentFrame - LastFrame;
		LastFrame = CurrentFrame;

		// Update the rotation angle based on deltaTime
		Angle += 50.0f * DeltaTime; // Rotate 50 degrees per second
		if (Angle > 360.0f) Angle -= 360.0f;

		// Process input
		GInputManager.processInput(Window, CurrentFrame);

		// Clear buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// Render Skybox
		renderSkybox();

		// Render based on the active scene

		// Swap buffers and poll events
		glfwSwapBuffers(Window);
		glfwPollEvents();
	}

	// Cleanup
	glfwTerminate();
	return 0;
}

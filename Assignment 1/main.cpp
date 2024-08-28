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

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Configure OpenGL context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Stencil Test Scene", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Set the cursor to disabled mode (lock and hide)
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Configure global OpenGL state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    // Load shaders
    Shader modelShader("resources/shaders/ModelShader.vert", "resources/shaders/ModelShader.frag");
    Shader outlineShader("resources/shaders/OutlineShader.vert", "resources/shaders/OutlineShader.frag");
    Shader skyboxShader("resources/shaders/SkyboxVertexShader.vert", "resources/shaders/SkyboxFragmentShader.frag");

    const std::string placeholderTexture = "PolygonAncientWorlds_Texture_01_A.png";

    // Load models
    Model tree("resources/models/DungeonPack/SM_Env_Tree_Twisted_01.obj", placeholderTexture);
    Model crystal("resources/models/DungeonPack/SM_Prop_Tech_Crystal_01.obj", placeholderTexture);
    Model gemLarge("resources/models/DungeonPack/SM_Env_Gem_Large_01.obj", placeholderTexture);
    Model gemSpike("resources/models/DungeonPack/SM_Env_Gem_Spike_02.obj", placeholderTexture);

    // Initialize Skybox
    std::vector<std::string> skyboxFaces = {
        "resources/skybox/Corona/right.png",
        "resources/skybox/Corona/left.png",
        "resources/skybox/Corona/top.png",
        "resources/skybox/Corona/bottom.png",
        "resources/skybox/Corona/back.png",
        "resources/skybox/Corona/front.png"
    };
    Skybox skybox(skyboxFaces);

    // Set up camera
    Camera camera(glm::vec3(0.0f, 2.0f, 8.0f)); // Position the camera to view the scene
    LightManager lightManager;
    lightManager.initialize();
    InputManager inputManager(camera, lightManager);

    // Set up input callbacks
    glfwSetFramebufferSizeCallback(window, InputManager::frameBufferSizeCallback);
    glfwSetCursorPosCallback(window, InputManager::mouseCallback);
    glfwSetScrollCallback(window, InputManager::scrollCallback);

    // Declare and initialize the scale factor for outlines
	float scale = 1.1f;

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Input handling
        float currentFrame = static_cast<float>(glfwGetTime());
        inputManager.processInput(window, currentFrame);

        // Clear buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Render Skybox
        glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use();
        glm::mat4 view = glm::mat4(glm::mat3(camera.getViewMatrix())); // Remove translation from the view matrix
        glm::mat4 projection = camera.getProjectionMatrix(800, 600);
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        skybox.draw(skyboxShader);
        glDepthFunc(GL_LESS); // Reset depth function

        // Stencil test rendering
        glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set stencil buffer to always write 1
        glStencilMask(0xFF); // Enable writing to the stencil buffer

        // Render models normally
        modelShader.use();
        modelShader.setMat4("projection", projection);
        modelShader.setMat4("view", camera.getViewMatrix());

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        modelShader.setMat4("model", model);
        tree.draw(modelShader);

        // Rotate the tree model to satisfy the rotation requirement
        static float angle = 0.0f;
        angle += 50.0f * currentFrame; // Rotate 50 degrees per second
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
        modelShader.setMat4("model", model);
        tree.draw(modelShader);

        // Render other models
        model = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, 0.0f, -2.0f));
        modelShader.setMat4("model", model);
        crystal.draw(modelShader);

        model = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.0f, 2.0f));
        modelShader.setMat4("model", model);
        gemLarge.draw(modelShader);

        model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 4.0f));
        modelShader.setMat4("model", model);
        gemSpike.draw(modelShader);

        // Outline pass
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // Render where stencil buffer is not 1
        glStencilMask(0x00); // Disable writing to the stencil buffer
        glDisable(GL_DEPTH_TEST); // Disable depth test for the outline pass

        outlineShader.use();
        outlineShader.setMat4("projection", projection);
        outlineShader.setMat4("view", camera.getViewMatrix());

        // Draw the outline for the rotating tree
        scale = 1.1f;
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        outlineShader.setMat4("model", model);
        tree.draw(outlineShader);

        // Draw the outline for other models
        model = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, 0.0f, -2.0f));
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        outlineShader.setMat4("model", model);
        crystal.draw(outlineShader);

        model = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.0f, 2.0f));
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        outlineShader.setMat4("model", model);
        gemLarge.draw(outlineShader);

        model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 4.0f));
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        outlineShader.setMat4("model", model);
        gemSpike.draw(outlineShader);

        // Reset state for next pass
        glStencilMask(0xFF);
        glEnable(GL_DEPTH_TEST);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glfwTerminate();
    return 0;
}

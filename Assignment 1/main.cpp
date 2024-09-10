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

    // Time tracking variables for rotation
    float lastFrame = 0.0f;
    float angle = 0.0f;  // Store the current rotation angle

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Calculate deltaTime (time between frames)
        float currentFrame = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Update the rotation angle based on deltaTime
        angle += 50.0f * deltaTime;  // Rotate 50 degrees per second
        if (angle > 360.0f) angle -= 360.0f;

        // Process input...
        inputManager.processInput(window, currentFrame);

        // Clear buffers...
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Render Skybox...
        skyboxShader.use();
        glm::mat4 view = glm::mat4(glm::mat3(camera.getViewMatrix())); // Remove translation from the view matrix
        glm::mat4 projection = camera.getProjectionMatrix(800, 600);
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        skybox.draw(skyboxShader);
        glDepthFunc(GL_LESS); // Reset depth function

        // First pass: Render models normally with stencil buffer update
        glStencilFunc(GL_ALWAYS, 1, 0xFF);  // Set any fragment to 1 in the stencil buffer
        glStencilMask(0xFF);  // Enable writing to the stencil buffer

        modelShader.use();
        modelShader.setMat4("projection", projection);
        modelShader.setMat4("view", camera.getViewMatrix());

        // Tree (rotating)
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));  // Rotating object
        modelShader.setMat4("model", model);
        tree.draw(modelShader);

        // Other models...
        glm::mat4 crystalModel = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, 0.0f, -2.0f));
        modelShader.setMat4("model", crystalModel);
        crystal.draw(modelShader);

        // Second pass: Render the outlines using the stencil buffer
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);  // Render where stencil buffer is not 1
        glStencilMask(0x00);  // Disable writing to the stencil buffer
        glDisable(GL_DEPTH_TEST);  // Disable depth testing to ensure outline is drawn over everything

        outlineShader.use();
        outlineShader.setMat4("projection", projection);
        outlineShader.setMat4("view", camera.getViewMatrix());

        float outlineScale = 2.0f;  // Slightly increase the scale to make the outline more visible

        // Draw the outline for the rotating tree
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));  // Apply the same rotation
        model = glm::scale(model, glm::vec3(outlineScale, outlineScale, outlineScale));  // Scale the model to create the outline
        outlineShader.setMat4("model", model);
        tree.draw(outlineShader);

        // Draw the outline for the other models
        crystalModel = glm::scale(crystalModel, glm::vec3(outlineScale, outlineScale, outlineScale));
        outlineShader.setMat4("model", crystalModel);
        crystal.draw(outlineShader);

        // Reset stencil and depth testing for next frame
        glStencilMask(0xFF);  // Enable writing to the stencil buffer again
        glEnable(GL_DEPTH_TEST);  // Enable depth testing again

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // Cleanup
    glfwTerminate();
    return 0;
}

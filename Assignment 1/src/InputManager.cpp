#include "InputManager.h"

InputManager* InputManager::instance = nullptr;

InputManager::InputManager(Camera& Camera, LightManager& LightManager)
    : MCamera(Camera), MLightManager(LightManager), MFirstMouse(true), MLastX(400), MLastY(300) {
    instance = this; // Set static instance
}

void InputManager::processInput(GLFWwindow* Window, float DeltaTime) {
    if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(Window, true);

    // Camera movement handling
    if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
        MCamera.processKeyboard(FORWARD, DeltaTime);
    if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
        MCamera.processKeyboard(BACKWARD, DeltaTime);
    if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
        MCamera.processKeyboard(LEFT, DeltaTime);
    if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
        MCamera.processKeyboard(RIGHT, DeltaTime);
    if (glfwGetKey(Window, GLFW_KEY_E) == GLFW_PRESS)
        MCamera.processKeyboard(UP, DeltaTime); // Move up
    if (glfwGetKey(Window, GLFW_KEY_Q) == GLFW_PRESS)
        MCamera.processKeyboard(DOWN, DeltaTime); // Move down

    // Scene switching logic using number keys
    if (glfwGetKey(Window, GLFW_KEY_1) == GLFW_PRESS) {
        std::cout << "Switching to Scene 1: Stencil Test" << std::endl;
        // Logic to switch to Scene 1
    }
    if (glfwGetKey(Window, GLFW_KEY_2) == GLFW_PRESS) {
        std::cout << "Switching to Scene 2: Terrain Rendering" << std::endl;
        // Logic to switch to Scene 2
    }
    if (glfwGetKey(Window, GLFW_KEY_3) == GLFW_PRESS) {
        std::cout << "Switching to Scene 3: Perlin Noise Generation" << std::endl;
        // Logic to switch to Scene 3
    }
    if (glfwGetKey(Window, GLFW_KEY_4) == GLFW_PRESS) {
        std::cout << "Switching to Scene 4: Post-Processing" << std::endl;
        // Logic to switch to Scene 4
    }
}


void InputManager::frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void InputManager::mouseCallback(GLFWwindow* Window, double PosX, double PosY) {
    if (instance) {
        if (instance->MFirstMouse) {
            instance->MLastX = static_cast<float>(PosX);
            instance->MLastY = static_cast<float>(PosY);
            instance->MFirstMouse = false;
        }

        float OffsetX = static_cast<float>(PosX) - instance->MLastX;
        float OffsetY = instance->MLastY - static_cast<float>(PosY);
        instance->MLastX = static_cast<float>(PosX);
        instance->MLastY = static_cast<float>(PosY);

        instance->MCamera.processMouseMovement(OffsetX, OffsetY);
    }
}

void InputManager::scrollCallback(GLFWwindow* Window, double OffsetX, double OffsetY) {
    if (instance) {
        instance->MCamera.processMouseScroll(static_cast<float>(OffsetY));
    }
}

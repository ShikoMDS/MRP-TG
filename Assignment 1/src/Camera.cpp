#include "Camera.h"

Camera::Camera(float PosX, float PosY, float PosZ, float UpX, float UpY, float UpZ, float Yaw, float Pitch)
    : VFront(glm::vec3(0.0f, 0.0f, -1.0f)), FZoom(45.0f) {
    VPosition = glm::vec3(PosX, PosY, PosZ);
    VWorldUp = glm::vec3(UpX, UpY, UpZ);
    FYaw = Yaw;
    FPitch = Pitch;
    updateCameraVectors();
}

Camera::Camera(glm::vec3 Pos, glm::vec3 Up, float Yaw, float Pitch)
    : VFront(glm::vec3(0.0f, 0.0f, -1.0f)), FZoom(45.0f) {
    VPosition = Pos;
    VWorldUp = Up;
    FYaw = Yaw;
    FPitch = Pitch;
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(VPosition, VPosition + VFront, VUp);
}

glm::mat4 Camera::getProjectionMatrix(float Width, float Height) const {
    return glm::perspective(glm::radians(FZoom), Width / Height, 0.1f, 100.0f);
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        VPosition += VFront * velocity;
    if (direction == BACKWARD)
        VPosition -= VFront * velocity;
    if (direction == LEFT)
        VPosition -= VRight * velocity;
    if (direction == RIGHT)
        VPosition += VRight * velocity;
    if (direction == UP)
        VPosition += VWorldUp * velocity; // Move up
    if (direction == DOWN)
        VPosition -= VWorldUp * velocity; // Move down
}

void Camera::processMouseMovement(float OffsetX, float OffsetY, GLboolean ConstrainPitch) {
    OffsetX *= MouseSensitivity;
    OffsetY *= MouseSensitivity;

    FYaw += OffsetX;
    FPitch += OffsetY;

    if (ConstrainPitch) {
        if (FPitch > 89.0f)
            FPitch = 89.0f;
        if (FPitch < -89.0f)
            FPitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::processMouseScroll(float OffsetY) {
    FZoom -= OffsetY;
    if (FZoom < 1.0f)
        FZoom = 1.0f;
    if (FZoom > 90.0f)
        FZoom = 90.0f;
}

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(FYaw)) * cos(glm::radians(FPitch));
    front.y = sin(glm::radians(FPitch));
    front.z = sin(glm::radians(FYaw)) * cos(glm::radians(FPitch));
    VFront = glm::normalize(front);
    VRight = glm::normalize(glm::cross(VFront, VWorldUp));
    VUp = glm::normalize(glm::cross(VRight, VFront));
}

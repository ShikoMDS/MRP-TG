#include "Camera.h"

Camera::Camera(float PosX, float PosY, float PosZ, float UpX, float UpY, float UpZ, float Yaw, float Pitch)
	: VFront(glm::vec3(0.0f, 0.0f, -1.0f)), FZoom(45.0f)
{
	VPosition = glm::vec3(PosX, PosY, PosZ);
	VWorldUp = glm::vec3(UpX, UpY, UpZ);
	FYaw = Yaw;
	FPitch = Pitch;
	updateCameraVectors();
}

Camera::Camera(glm::vec3 Pos, glm::vec3 Up, float Yaw, float Pitch)
	: VFront(glm::vec3(0.0f, 0.0f, -1.0f)), FZoom(45.0f)
{
	VPosition = Pos;
	VWorldUp = Up;
	FYaw = Yaw;
	FPitch = Pitch;
	updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
	return lookAt(VPosition, VPosition + VFront, VUp);
}

glm::mat4 Camera::getProjectionMatrix(const float Width, const float Height) const
{
	return glm::perspective(glm::radians(FZoom), Width / Height, 0.1f, 100.0f);
}

void Camera::processKeyboard(const CameraMovement Direction, const float DeltaTime)
{
	const float Velocity = MovementSpeed * DeltaTime;
	if (Direction == Forward)
		VPosition += VFront * Velocity;
	if (Direction == Backward)
		VPosition -= VFront * Velocity;
	if (Direction == Left)
		VPosition -= VRight * Velocity;
	if (Direction == Right)
		VPosition += VRight * Velocity;
	if (Direction == Up)
		VPosition += VWorldUp * Velocity;
	if (Direction == Down)
		VPosition -= VWorldUp * Velocity;
}

void Camera::processMouseMovement(float OffsetX, float OffsetY, const GLboolean ConstrainPitch)
{
	OffsetX *= MouseSensitivity;
	OffsetY *= MouseSensitivity;

	FYaw += OffsetX;
	FPitch += OffsetY;

	if (ConstrainPitch)
	{
		if (FPitch > 89.0f)
			FPitch = 89.0f;
		if (FPitch < -89.0f)
			FPitch = -89.0f;
	}

	updateCameraVectors();
}

void Camera::processMouseScroll(const float OffsetY)
{
	FZoom -= OffsetY;
	if (FZoom < 1.0f)
		FZoom = 1.0f;
	if (FZoom > 90.0f)
		FZoom = 90.0f;
}

void Camera::updateCameraVectors()
{
	glm::vec3 Front;
	Front.x = cos(glm::radians(FYaw)) * cos(glm::radians(FPitch));
	Front.y = sin(glm::radians(FPitch));
	Front.z = sin(glm::radians(FYaw)) * cos(glm::radians(FPitch));
	VFront = normalize(Front);
	VRight = normalize(cross(VFront, VWorldUp));
	VUp = normalize(cross(VRight, VFront));
}

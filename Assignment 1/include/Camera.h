#pragma once

#include <glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

enum CameraMovement
{
	Forward,
	Backward,
	Left,
	Right,
	Up,
	Down
};

class Camera
{
public:
	Camera(float PosX, float PosY, float PosZ, float UpX, float UpY, float UpZ, float Yaw, float Pitch);
	explicit Camera(glm::vec3 Pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f),
	                float Yaw = -90.0f, float Pitch = 0.0f);

	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjectionMatrix(float Width, float Height) const;

	void processKeyboard(CameraMovement Direction, float DeltaTime);
	void processMouseMovement(float OffsetX, float OffsetY, GLboolean ConstrainPitch = true);
	void processMouseScroll(float OffsetY);

	glm::vec3 VPosition;
	glm::vec3 VFront;
	glm::vec3 VUp;
	glm::vec3 VRight;
	glm::vec3 VWorldUp;
	float FYaw;
	float FPitch;
	float FZoom;

private:
	float MovementSpeed = 0.005f;
	float MouseSensitivity = 0.1f;

	void updateCameraVectors();
};

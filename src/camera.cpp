#include "camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

Camera::Camera(int viewWidth, int viewHeight, float fov, float near, float far)
{
	this->viewHeight = viewHeight;
	this->viewWidth = viewWidth;
	this->near = near;
	this->fov = fov;
	this->far = far;

	position = glm::vec3(0.5f, 0.5f, 3.0f);
	front = glm::vec3(0, 0, -1);
	right = glm::vec3(0, 0, 0);
	up = glm::vec3(0, 1, 0);

	glm::vec3 camFront;

	camFront.x = static_cast<float>(cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
	camFront.y = static_cast<float>(sin(glm::radians(pitch)));
	camFront.z = static_cast<float>(sin(glm::radians(yaw)) * cos(glm::radians(pitch)));

	front = glm::normalize(camFront);
}

Camera::Camera(glm::vec3 & position, glm::vec3 & front, glm::vec3 & right, glm::vec3 & up, int viewWidth, int viewHeight, float fov, float near, float far)
{
	this->viewHeight = viewHeight;
	this->viewWidth = viewWidth;
	this->near = near;
	this->fov = fov;
	this->far = far;

	this->position = position;
	this->front = front;
	this->right = right;
	this->up = up;

	glm::vec3 camFront;

	camFront.x = static_cast<float>(cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
	camFront.y = static_cast<float>(sin(glm::radians(pitch)));
	camFront.z = static_cast<float>(sin(glm::radians(yaw)) * cos(glm::radians(pitch)));

	front = glm::normalize(camFront);
}

void Camera::trackMouse(const float mx, const float my)
{
	if (mouseStart)
	{
		lastX = mx;
		lastY = my;
		mouseStart = false;
	}

	float xoffset = mx - lastX;
	float yoffset = lastY - my;

	lastX = mx;
	lastY = my;

	xoffset *= sensivity;
	yoffset *= sensivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 camFront;

	camFront.x = static_cast<float>(cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
	camFront.y = static_cast<float>(sin(glm::radians(pitch)));
	camFront.z = static_cast<float>(sin(glm::radians(yaw)) * cos(glm::radians(pitch)));

	front = glm::normalize(camFront);
}

void Camera::moveForward()
{
	this->position += speed * front;
}

void Camera::moveBackwards()
{
	this->position -= speed * front;
}

void Camera::moveLeft()
{
	this->position -= glm::cross(front, up) * speed;
}

void Camera::moveRight()
{
	this->position += glm::cross(front, up) * speed;
}

void Camera::resetMouse()
{
	mouseStart = true;
}

void Camera::focusAt(const glm::vec3 position)
{
	this->position = position - (front * glm::vec3(5, 5, 5));
}

glm::mat4 Camera::getProjection() const {
	return glm::perspective(fov, static_cast<float>(viewWidth) / static_cast<float>(viewHeight), near, far);
}

glm::mat4 Camera::getView() const {
	return glm::lookAt(position, position + front, up);
}

void Camera::screenToWorld(const float mx, const float my) {
	const float mouseX = mx / (static_cast<float>(viewWidth)  * 0.5f) - 1.0f;
	const float mouseY = my / (static_cast<float>(viewHeight) * 0.5f) - 1.0f;

	const glm::mat4 proj = glm::perspective(fov, static_cast<float>(viewWidth) / static_cast<float>(viewHeight), near, far);
	const glm::mat4 view = glm::lookAt(glm::vec3(0.0f), front, up);

	const glm::mat4 invVP = glm::inverse(proj * view);
	const glm::vec4 screenPos = glm::vec4(mouseX, -mouseY, 1.0f, 1.0f);
	const glm::vec4 worldPos = invVP * screenPos;

	rayOrigin = position;
	rayDirection = glm::normalize(glm::vec3(worldPos));
}

glm::mat4 Camera::getTransform() const {
	const glm::mat4 rotation = glm::toMat4(glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)));
	const glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
	const glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	return  translation * rotation * scale;
}

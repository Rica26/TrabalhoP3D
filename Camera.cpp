#include "Camera.h"
#include <iostream>

Camera::Camera() : position(2.0f, 3.0f, 5.0f), target(0.0f, 0.0f, 0.0f), up(0.0f, 1.0f, 0.0f), fov(45.0f) {
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, target, up);
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio) const {
    return glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f);
}

void Camera::setPosition(const glm::vec3& position) {
    this->position = position;
}

void Camera::setTarget(const glm::vec3& target) {
    this->target = target;
}

void Camera::setUp(const glm::vec3& up) {
    this->up = up;
}

void Camera::setFOV(float fov) {
    this->fov = fov;
}

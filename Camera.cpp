#include "Camera.h"
#include <iostream>

Camera::Camera() : position(2.0f, 3.0f, 5.0f), target(0.0f, 0.0f, -0.1f), up(0.0f, 1.0f, 0.0f), fov(45.0f) {
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, target, up);
}

glm::vec3 Camera::getPosition() const {
    return position;
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio) const {
    return glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f);
}

void Camera::ZOOM(double yoffset) {
    if (yoffset == -1) {
        fov += fabs(fov) * 0.1f;
    }
    else if (yoffset == 1) {
        fov -= fabs(fov) * 0.1f;
    }
}

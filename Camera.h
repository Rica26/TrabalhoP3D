#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

class Camera {
public:
    Camera();
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(float aspectRatio) const;
    glm::vec3 getPosition() const;
    void ZOOM(double yoffset);

private:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    float fov;
};


#endif // CAMERA_H


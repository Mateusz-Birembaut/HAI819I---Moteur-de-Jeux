#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

class Transform {
public:
    float rotationSpeed;
    bool isDirty= true;
    
    glm::bvec3 continuouslyRotate = { false, false, false };
    glm::vec3 translation = { 0.0f, 0.0f, 0.0f };
    glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
    glm::vec3 eulerRot = { 0.0f, 0.0f, 0.0f };
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    void resetTransform();
    glm::mat3 getRotation3x3() const;
    glm::mat4 getLocalModelMatrix(float deltaTime);
    bool needsUpdate();
};

#endif

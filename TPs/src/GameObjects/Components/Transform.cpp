#include "Transform.hpp"

void Transform::resetTransform() {
    rotationSpeed = 0.0f;
    continuouslyRotate = { false, false, false };
    eulerRot = { 0.0f, 0.0f, 0.0f };
    scale = { 1.0f, 1.0f, 1.0f };
    modelMatrix = glm::mat4(1.0f);
    isDirty = true;
}


glm::mat4 Transform::getLocalModelMatrix(float deltaTime) {
    if (continuouslyRotate.x){
        eulerRot.x += rotationSpeed * deltaTime; 
        isDirty = true;
    } 
    if (continuouslyRotate.y){
        eulerRot.y += rotationSpeed * deltaTime; 
        isDirty = true;
    } 
    if (continuouslyRotate.z){
        eulerRot.z += rotationSpeed * deltaTime; 
        isDirty = true;
    } 

    if (!isDirty){
        return modelMatrix;
    } 

    eulerRot.x = fmodf(eulerRot.x, 360.0f);
    eulerRot.y = fmodf(eulerRot.y, 360.0f);
    eulerRot.z = fmodf(eulerRot.z, 360.0f);

    if (eulerRot.x > 180.0f) eulerRot.x -= 360.0f;
    if (eulerRot.y > 180.0f) eulerRot.y -= 360.0f;
    if (eulerRot.z > 180.0f) eulerRot.z -= 360.0f;

    const glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(eulerRot.x), glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), glm::radians(eulerRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), glm::radians(eulerRot.z),glm::vec3(0.0f, 0.0f, 1.0f));

    // Y * X * Z
    const glm::mat4 rotationMatrix = rotationZ * rotationY * rotationX;

    modelMatrix = glm::translate(glm::mat4(1.0f), translation) * rotationMatrix * glm::scale(glm::mat4(1.0f), scale);
    
    return modelMatrix;
}

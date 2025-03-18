#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

class Transform{

    public:
        float rotationSpeed;

        glm::bvec3 continuouslyRotate = { false, false, false };
    
        glm::vec3 translation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 eulerRot = { 0.0f, 0.0f, 0.0f };
        glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

        glm::mat4 modelMatrix = glm::mat4(1.0f);

        void resetTransform(){
            rotationSpeed= 0.0f;
            continuouslyRotate = { false, false, false };
            eulerRot = { 0.0f, 0.0f, 0.0f };
            scale = { 1.0f, 1.0f, 1.0f };
            modelMatrix = glm::mat4(1.0f);
        }

        glm::mat4 getLocalModelMatrix(float deltaTime){
            if (continuouslyRotate.x) eulerRot.x += rotationSpeed * deltaTime;
            if (continuouslyRotate.y) eulerRot.y += rotationSpeed * deltaTime;
            if (continuouslyRotate.z) eulerRot.z += rotationSpeed * deltaTime;
            
            eulerRot.x = fmodf(eulerRot.x, 360.0f);
            eulerRot.y = fmodf(eulerRot.y, 360.0f);
            eulerRot.z = fmodf(eulerRot.z, 360.0f);
            
            if (eulerRot.x > 180.0f) eulerRot.x -= 360.0f;
            if (eulerRot.y > 180.0f) eulerRot.y -= 360.0f;
            if (eulerRot.z > 180.0f) eulerRot.z -= 360.0f;

            const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f),
                                glm::radians(eulerRot.x),
                                glm::vec3(1.0f, 0.0f, 0.0f));
            const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f),
                                glm::radians(eulerRot.y),
                                glm::vec3(0.0f, 1.0f, 0.0f));
            const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f),
                                glm::radians(eulerRot.z),
                                glm::vec3(0.0f, 0.0f, 1.0f));

            // Y * X * Z
            const glm::mat4 roationMatrix = transformY * transformX * transformZ;

            // translation * rotation * scale (also know as TRS matrix)
            return glm::translate(glm::mat4(1.0f), translation) * roationMatrix * glm::scale(glm::mat4(1.0f), scale);
        }

};


#endif
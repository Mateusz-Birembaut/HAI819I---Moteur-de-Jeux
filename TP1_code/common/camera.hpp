#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

enum CAMERA_CONTROL { FREE, ROTATE };

class Camera
{
private:

    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    float rotation_angle;
    float speed;
    CAMERA_CONTROL camera_control;
    bool cpressed;


public:
    Camera( glm::vec3 position = glm::vec3(0.0f, 1.0f, 2.0f),
            glm::vec3 target = glm::vec3(0.0f, 0.0f, -1.0f),
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
            float angle = 0.0f,
            float speed = 2.5f):   

            position(position),
            target(target),
            up(up),
            rotation_angle(angle),
            speed(speed),
            camera_control(FREE)
    {}

    glm::vec3 getPosition() const { return position; }
    glm::vec3 getTarget() const { return target; }
    glm::vec3 getUp() const { return up; }
    float getRotationAngle() const { return rotation_angle; }
    float getSpeed() const { return speed; }
    CAMERA_CONTROL getCameraControl() const { return camera_control; }

    void handleCameraInputs(float deltaTime, GLFWwindow* window){
        float cameraSpeed = speed * deltaTime;
        if (camera_control == FREE){

            if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
                if (!cpressed) {
                    camera_control = ROTATE;
                    cpressed = true;
                }
            } else {
                cpressed = false;
            }

            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                position += cameraSpeed * target;
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                position -= cameraSpeed * target;
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
                glm::vec3 camera_right = glm::cross(up, target);
                position += cameraSpeed * camera_right;
            }
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
                glm::vec3 camera_right = glm::cross( up, target);
                position -= cameraSpeed * camera_right;
            }

            if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
                rotation_angle += cameraSpeed;
                if (rotation_angle > glm::radians(360.0f)) {
                    rotation_angle -= glm::radians(360.0f);
                }
                glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), rotation_angle, up);
                target = glm::vec3(rotation * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
            }
            if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
                rotation_angle -= cameraSpeed;
                if (rotation_angle < glm::radians(-360.0f)) {
                    rotation_angle += glm::radians(360.0f);
                }
                glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), rotation_angle, up);
                target = glm::vec3(rotation * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
            }
    
            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
                target += cameraSpeed * up; // pas ouf mais ok pour test texture
            }
            if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
                target -= cameraSpeed * up; // pas ouf mais ok pour test texture
            }

        }else if (camera_control == ROTATE){
            if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
                if (!cpressed) {
                    camera_control = FREE;
                    cpressed = true;
                }
            } else {
                cpressed = false;
            }

            
        }
    }

};


CAMERA_CONTROL camera_control = FREE;


#endif
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#define DEBUG_CAMERA false

enum CAMERA_CONTROL { FREE, ORBIT };

const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

class Camera
{
private:

    // general
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    CAMERA_CONTROL camera_control;

    const float MAX_VERTICAL_ANGLE = glm::radians(85.0f);
    const float MIN_VERTICAL_ANGLE = glm::radians(5.0f);

    bool cpressed;
 
    // free
    float vertical_angle = glm::radians(0.0f);
    float rotation_angle;
    float speed;

    // orbit
    float orbit_distance;
    float orbit_speed;


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
            orbit_speed(1.0f),
            orbit_distance(5.0f),
            camera_control(FREE)
    {}

    glm::vec3 getPosition() const { return position; }
    glm::vec3 getTarget() const { return target; }
    glm::vec3 getUp() const { return up; }
    float getRotationAngle() const { return rotation_angle; }
    float getSpeed() const { return speed; }
    CAMERA_CONTROL getCameraControl() const { return camera_control; }

    void resetCamera(){
        position = glm::vec3(0.0f, 1.0f, 2.0f);
        target = glm::vec3(0.0f, 0.0f, -1.0f);
        up = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    void orbit(float deltaTime, glm::vec3 orbit_center){
        float angle = orbit_speed * deltaTime;
        rotation_angle += angle;
        if (rotation_angle > glm::radians(360.0f)) {
            rotation_angle -= glm::radians(360.0f);
        }
        
        position.x = orbit_center.x + orbit_distance * cos(rotation_angle);
        position.z = orbit_center.z + orbit_distance * sin(rotation_angle);
        
        target = orbit_center - position;
        target = glm::normalize(target);

        glm::vec3 camera_right = glm::normalize(glm::cross(target, WORLD_UP));
        up = glm::cross(camera_right, target );
        up = glm::normalize(up);
    }

    void handleCameraInputs(float deltaTime, GLFWwindow* window){
        float cameraSpeed = speed * deltaTime;
        if (camera_control == FREE){

            if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
                if (!cpressed) {
                    camera_control = ORBIT;
                    cpressed = true;
                    resetCamera();
                }
            } else {
                cpressed = false;
            }

            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                position += cameraSpeed * target;
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                position -= cameraSpeed * target;
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
                glm::vec3 camera_right = glm::normalize(glm::cross(target, WORLD_UP));
                position -= cameraSpeed * camera_right;
            }
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
                glm::vec3 camera_right = glm::normalize(glm::cross(target, WORLD_UP));
                position += cameraSpeed * camera_right;
            }

            if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
                rotation_angle += cameraSpeed;
                if (rotation_angle > glm::radians(360.0f)) {
                    rotation_angle -= glm::radians(360.0f);
                }
                glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), cameraSpeed, WORLD_UP);
                target = glm::normalize(glm::vec3(rotation * glm::vec4(target, 0.0f)));

                glm::vec3 camera_right = glm::normalize(glm::cross(target, WORLD_UP));
                up = glm::normalize(glm::cross(camera_right, target));
            }
            
            if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
                rotation_angle -= cameraSpeed;
                if (rotation_angle < glm::radians(-360.0f)) {
                    rotation_angle += glm::radians(360.0f);
                }
                glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), -cameraSpeed, WORLD_UP);
                target = glm::normalize(glm::vec3(rotation * glm::vec4(target, 0.0f)));

                glm::vec3 camera_right = glm::normalize(glm::cross(target, WORLD_UP));
                up = glm::normalize(glm::cross(camera_right, target));
            }
    
            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
                if (vertical_angle < MAX_VERTICAL_ANGLE) {
                    vertical_angle += cameraSpeed;
                    
                    glm::vec3 camera_right = glm::normalize(glm::cross(target, WORLD_UP));
                    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), cameraSpeed, camera_right);
                    target = glm::normalize(glm::vec3(rotation * glm::vec4(target, 0.0f)));
                    up = glm::normalize(glm::cross(camera_right, target));
                }
            }
            
            if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
                if (vertical_angle > -MAX_VERTICAL_ANGLE) {
                    vertical_angle -= cameraSpeed;
                    
                    glm::vec3 camera_right = glm::normalize(glm::cross(target, WORLD_UP));
                    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), -cameraSpeed, camera_right);
                    target = glm::normalize(glm::vec3(rotation * glm::vec4(target, 0.0f)));
                    up = glm::normalize(glm::cross(camera_right, target));
                }
            }

            if (DEBUG_CAMERA){
                std::cout << "Position: " << position.x << " " << position.y << " " << position.z << std::endl;
                std::cout << "Target: " << target.x << " " << target.y << " " << target.z << std::endl;
                std::cout << "Up: " << up.x << " " << up.y << " " << up.z << std::endl;
                std::cout << "Vertical angle: " << vertical_angle << std::endl;
                std::cout << "Rotation angle: " << rotation_angle << std::endl;
            }

        }else if (camera_control == ORBIT){
            if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
                if (!cpressed) {
                    camera_control = FREE;
                    cpressed = true;
                }
            } else {
                cpressed = false;
            }

            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
                orbit_speed += 0.1f;
            }
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
                orbit_speed -= 0.1f;
            }
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
                orbit_distance += 0.1f;
            }
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
                orbit_distance -= 0.1f;
            }

            orbit(deltaTime, glm::vec3(0.0f, 0.0f, 0.0f));

            if (DEBUG_CAMERA){
                std::cout << "Position: " << position.x << " " << position.y << " " << position.z << std::endl;
                std::cout << "Target: " << target.x << " " << target.y << " " << target.z << std::endl;
                std::cout << "Up: " << up.x << " " << up.y << " " << up.z << std::endl;
                std::cout << "Orbit distance: " << orbit_distance << std::endl;
                std::cout << "Orbit speed: " << orbit_speed << std::endl;
            }

        }
    }

};



#endif
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "../GameObjects/GameObject.hpp"
#include "../GameObjects/Components/Physics/AABB.hpp"
#include "../Ressources/Globals.hpp"

#define DEBUG_CAMERA false

enum CAMERA_CONTROL { FREE, ORBIT };

const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

class Camera
{
private:

    Camera(){}

    float FOV = 45.0f;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 viewProjectionMatrix;
    int lastWindowWidth = 0;
    int lastWindowHeight = 0;
    bool matricesDirty = true;

    // general
    glm::vec3 position = glm::vec3(0.0f, 1.0f, 2.0f);
    glm::vec3 target = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    CAMERA_CONTROL camera_control = FREE;

    const float MAX_VERTICAL_ANGLE = glm::radians(85.0f);
    const float MIN_VERTICAL_ANGLE = glm::radians(5.0f);

    bool cpressed;
 
    // free
    float vertical_angle = glm::radians(0.0f);
    float rotation_angle = 0.0f;
    float speed = 2.5f;

    // orbit
    float orbit_distance = 5.0f;
    float orbit_speed = 1.0f;

    float nearDistance = 0.1f;
    float farDistance = 100.0f;


public:

    static Camera& getInstance(){
        static Camera camera;
        return camera;
    }

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
                    matricesDirty = true;
                }
            } else {
                cpressed = false;
            }

            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
                position += cameraSpeed * target;
                matricesDirty = true;
            }

            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
                position -= cameraSpeed * target;
                matricesDirty = true;
            }

            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
                glm::vec3 camera_right = glm::normalize(glm::cross(target, WORLD_UP));
                position -= cameraSpeed * camera_right;
                matricesDirty = true;
            }
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
                glm::vec3 camera_right = glm::normalize(glm::cross(target, WORLD_UP));
                position += cameraSpeed * camera_right;
                matricesDirty = true;
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
                matricesDirty = true;
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
                matricesDirty = true;
            }
    
            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
                if (vertical_angle < MAX_VERTICAL_ANGLE) {
                    vertical_angle += cameraSpeed;
                    
                    glm::vec3 camera_right = glm::normalize(glm::cross(target, WORLD_UP));
                    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), cameraSpeed, camera_right);
                    target = glm::normalize(glm::vec3(rotation * glm::vec4(target, 0.0f)));
                    up = glm::normalize(glm::cross(camera_right, target));
                    matricesDirty = true;
                }
            }
            
            if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
                if (vertical_angle > -MAX_VERTICAL_ANGLE) {
                    vertical_angle -= cameraSpeed;
                    
                    glm::vec3 camera_right = glm::normalize(glm::cross(target, WORLD_UP));
                    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), -cameraSpeed, camera_right);
                    target = glm::normalize(glm::vec3(rotation * glm::vec4(target, 0.0f)));
                    up = glm::normalize(glm::cross(camera_right, target));
                    matricesDirty = true;
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
            matricesDirty = true;
        }
    }

    void sendMatricesToShader(GLuint programID, int windowWidth, int windowHeight){
        updateMatrices(windowWidth, windowHeight);

        // Model matrix : an identity matrix (model will be at the origin) then change
        glm::mat4 modelMatrix = glm::mat4(1.0);
        // View matrix : camera/view transformation lookat() utiliser camera_position camera_target camera_up
        viewMatrix = glm::lookAt(position, target + position, up);
        // Projection matrix : 45 Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        projectionMatrix = glm::perspective(FOV, (float)windowWidth / (float)windowHeight, nearDistance, farDistance);

        // Send our transformation to the currently bound shader,
        // in the "Model View Projection" to the shader uniforms
        GLint loc_transformations = glGetUniformLocation(programID, "u_model");
        GLint loc_ViewMatrix = glGetUniformLocation(programID, "u_view");
        GLint lov_ProjectionMatrix = glGetUniformLocation(programID, "u_projection");

        glUniformMatrix4fv(loc_ViewMatrix, 1, GL_FALSE, &viewMatrix[0][0]);
        glUniformMatrix4fv(lov_ProjectionMatrix, 1, GL_FALSE, &projectionMatrix[0][0]);
        glUniformMatrix4fv(loc_transformations, 1, GL_FALSE, &modelMatrix[0][0]);
    
    }

    bool isInCameraView(GameObject * gameObject){
        if (gameObject->collider == nullptr){
            return false;
        }

        std::vector<glm::vec4> planes = getCameraPlanes();

        AABB bb = gameObject->collider->aabb;

        for(auto& plane : planes){
            unsigned short index = -1;
            float distToPlane = -INFINITY;

            // normal goes towards the view frustum
            glm::vec3 normal(plane.x, plane.y, plane.z);

            // distance to plane 
            glm::vec3 pVertex;
            // if we are in 1d, min ------- max  , if normal.x > 0 means goes towards the right IE max
            // we do this for every coordinates, we get the farest vertex
            pVertex.x = (normal.x >= 0) ? bb.worldMax.x : bb.worldMin.x;
            pVertex.y = (normal.y >= 0) ? bb.worldMax.y : bb.worldMin.y;
            pVertex.z = (normal.z >= 0) ? bb.worldMax.z : bb.worldMin.z;

            // Calculate distance from p-vertex to plane
            // plane equation: Ax + By + Cz + D = 0
            // distance = (Ax + By + Cz + D) / sqrt(A² + B² + C²)
            if (glm::dot(normal, pVertex) + plane.w < 0 ){
                return false;
            }


        }


        return true;

    }

    std::vector<glm::vec4> getCameraPlanes(){
        glm::vec4 planes[6];
        
        //extrac rows from projection matrix
        glm::vec4 row1 = glm::vec4(viewProjectionMatrix[0][0], viewProjectionMatrix[0][1], viewProjectionMatrix[0][2], viewProjectionMatrix[0][3]);
        glm::vec4 row2 = glm::vec4(viewProjectionMatrix[1][0], viewProjectionMatrix[1][1], viewProjectionMatrix[1][2], viewProjectionMatrix[1][3]);
        glm::vec4 row3 = glm::vec4(viewProjectionMatrix[2][0], viewProjectionMatrix[2][1], viewProjectionMatrix[2][2], viewProjectionMatrix[2][3]);
        glm::vec4 row4 = glm::vec4(viewProjectionMatrix[3][0], viewProjectionMatrix[3][1], viewProjectionMatrix[3][2], viewProjectionMatrix[3][3]);
        //gives coordinates in world coordinates

        planes[0] = row1 + row4; // left plane
        planes[1] = row1 - row4; // right 
        planes[2] = row2 + row4; // bottom
        planes[3] = row2 - row4; // top
        planes[4] = row3 + row4; // near
        planes[5] = row3 - row4; // far


        return std::vector<glm::vec4>(planes, planes + 6);
    }

    void updateMatrices(int windowWidth, int windowHeight) {
        if (!matricesDirty && lastWindowWidth == windowWidth && lastWindowHeight == windowHeight) {
            return;
        }
        
        lastWindowWidth = windowWidth;
        lastWindowHeight = windowHeight;
        
        viewMatrix = glm::lookAt(position, target + position, up);
        projectionMatrix = glm::perspective(FOV, (float)windowWidth / (float)windowHeight, nearDistance, farDistance);
        viewProjectionMatrix = projectionMatrix * viewMatrix;
        
        matricesDirty = false;
    }


};



#endif
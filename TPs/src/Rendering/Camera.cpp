
#include <GL/glew.h>
#include <iostream>

#include "Camera.hpp"

Camera::Camera() {}

Camera& Camera::getInstance() {
    static Camera camera;
    return camera;
}

glm::vec3 Camera::getPosition() const {
    return position;
}

glm::vec3 Camera::getTarget() const {
    return target;
}

glm::vec3 Camera::getUp() const {
    return up;
}

float Camera::getRotationAngle() const {
    return rotation_angle;
}

float Camera::getSpeed() const {
    return speed;
}

CAMERA_CONTROL Camera::getCameraControl() const {
    return camera_control;
}

void Camera::resetCamera() {
    position = glm::vec3(0.0f, 1.0f, 2.0f);
    target = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::orbit(float deltaTime, glm::vec3 orbit_center) {
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

void Camera::handleCameraInputs(float deltaTime, GLFWwindow* window) {
    float cameraSpeed = speed * deltaTime;
    if (camera_control == FREE) {
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

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            position += cameraSpeed * target;
            matricesDirty = true;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            position -= cameraSpeed * target;
            matricesDirty = true;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            glm::vec3 camera_right = glm::normalize(glm::cross(target, WORLD_UP));
            position -= cameraSpeed * camera_right;
            matricesDirty = true;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
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

        if (DEBUG_CAMERA) {
            std::cout << "Position: " << position.x << " " << position.y << " " << position.z << std::endl;
            std::cout << "Target: " << target.x << " " << target.y << " " << target.z << std::endl;
            std::cout << "Up: " << up.x << " " << up.y << " " << up.z << std::endl;
            std::cout << "Vertical angle: " << vertical_angle << std::endl;
            std::cout << "Rotation angle: " << rotation_angle << std::endl;
        }

    } else if (camera_control == ORBIT) {
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
            if (!cpressed) {
                camera_control = FREE;
                cpressed = true;
            }
        } else {
            cpressed = false;
        }

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            orbit_speed += 0.1f;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            orbit_speed -= 0.1f;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            orbit_distance += 0.1f;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            orbit_distance -= 0.1f;
        }

        orbit(deltaTime, glm::vec3(0.0f, 0.0f, 0.0f));

        if (DEBUG_CAMERA) {
            std::cout << "Position: " << position.x << " " << position.y << " " << position.z << std::endl;
            std::cout << "Target: " << target.x << " " << target.y << " " << target.z << std::endl;
            std::cout << "Up: " << up.x << " " << up.y << " " << up.z << std::endl;
            std::cout << "Orbit distance: " << orbit_distance << std::endl;
            std::cout << "Orbit speed: " << orbit_speed << std::endl;
        }
        matricesDirty = true;
    }
}

void Camera::sendMatricesToShader(GLuint programID, int windowWidth, int windowHeight) {
    updateMatrices(windowWidth, windowHeight);

    // Model matrix : an identity matrix (model will be at the origin) then change
    glm::mat4 modelMatrix = glm::mat4(1.0);

    // Send our transformation to the currently bound shader,
    // in the "Model View Projection" to the shader uniforms
    GLint loc_transformations = glGetUniformLocation(programID, "u_model");
    GLint loc_ViewMatrix = glGetUniformLocation(programID, "u_view");
    GLint lov_ProjectionMatrix = glGetUniformLocation(programID, "u_projection");

    glUniformMatrix4fv(loc_ViewMatrix, 1, GL_FALSE, &viewMatrix[0][0]);
    glUniformMatrix4fv(lov_ProjectionMatrix, 1, GL_FALSE, &projectionMatrix[0][0]);
    glUniformMatrix4fv(loc_transformations, 1, GL_FALSE, &modelMatrix[0][0]);
}

bool Camera::isInCameraView(GameObject * gameObject) {
    if (gameObject->collider == nullptr) {
        return false;
    }

    updateMatrices(lastWindowWidth, lastWindowHeight);

    glm::vec4 planes[6];
    getCameraPlanes(planes);

    AABB bb = gameObject->collider->aabb;

    for(auto& plane : planes) {
        glm::vec3 normal(plane.x, plane.y, plane.z);

        // closest point of aabb to plane
        glm::vec3 farthestPoint;
        farthestPoint.x = (normal.x <= 0) ? bb.worldMin.x : bb.worldMax.x;
        farthestPoint.y = (normal.y <= 0) ? bb.worldMin.y : bb.worldMax.y;
        farthestPoint.z = (normal.z <= 0) ? bb.worldMin.z : bb.worldMax.z;

        // we project the point on the normal, if the dot < 0, means is outside the frustum, we can return false
        if (glm::dot(farthestPoint, normal ) + plane.w <= 0) {
            return false;
        }
    }
    return true;
}

bool Camera::isInCameraView(AABB * aabb) {
    updateMatrices(lastWindowWidth, lastWindowHeight);

    glm::vec4 planes[6];
    getCameraPlanes(planes);


    for(auto& plane : planes) {
        glm::vec3 normal(plane.x, plane.y, plane.z);

        // closest point of aabb to plane
        glm::vec3 farthestPoint;
        farthestPoint.x = (normal.x <= 0) ? aabb->worldMin.x : aabb->worldMax.x;
        farthestPoint.y = (normal.y <= 0) ? aabb->worldMin.y : aabb->worldMax.y;
        farthestPoint.z = (normal.z <= 0) ? aabb->worldMin.z : aabb->worldMax.z;

        // we project the point on the normal, if the dot < 0, means is outside the frustum, we can return false
        if (glm::dot(farthestPoint, normal ) + plane.w <= 0) {
            return false;
        }
    }
    return true;
}


void Camera::getCameraPlanes(glm::vec4 * planes){
 
    glm::vec4 col1(viewProjectionMatrix[0][0], viewProjectionMatrix[1][0], viewProjectionMatrix[2][0], viewProjectionMatrix[3][0]);
    glm::vec4 col2(viewProjectionMatrix[0][1], viewProjectionMatrix[1][1], viewProjectionMatrix[2][1], viewProjectionMatrix[3][1]);
    glm::vec4 col3(viewProjectionMatrix[0][2], viewProjectionMatrix[1][2], viewProjectionMatrix[2][2], viewProjectionMatrix[3][2]);
    glm::vec4 col4(viewProjectionMatrix[0][3], viewProjectionMatrix[1][3], viewProjectionMatrix[2][3], viewProjectionMatrix[3][3]);

    planes[0] = col4 + col1; // left
    planes[1] = col4 - col1; // right
    planes[2] = col4 + col2; // bottom
    planes[3] = col4 - col2; // top
    planes[4] = col4 + col3; // near
    planes[5] = col4 - col3; // far
}



void Camera::updateMatrices(int windowWidth, int windowHeight) {
    if (!matricesDirty && lastWindowWidth == windowWidth && lastWindowHeight == windowHeight) {
        return;
    }

    lastWindowWidth = windowWidth;
    lastWindowHeight = windowHeight;

    viewMatrix = glm::lookAt(position, position + target , up);
    projectionMatrix = glm::perspective(FOV, (float)windowWidth / (float)windowHeight, nearDistance, farDistance);
    viewProjectionMatrix = projectionMatrix * viewMatrix;

    matricesDirty = false;
}

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../GameObjects/GameObject.hpp"

#define DEBUG_CAMERA false

enum CAMERA_CONTROL { FREE, ORBIT };

const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

class Camera
{
private:
    Camera();

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
    static Camera& getInstance();

    glm::vec3 getPosition() const;
    glm::vec3 getTarget() const;
    glm::vec3 getUp() const;
    float getRotationAngle() const;
    float getSpeed() const;
    CAMERA_CONTROL getCameraControl() const;

    void resetCamera();
    void orbit(float deltaTime, glm::vec3 orbit_center);
    void handleCameraInputs(float deltaTime, GLFWwindow* window);
    void sendMatricesToShader(GLuint programID, int windowWidth, int windowHeight);
    bool isInCameraView(GameObject * gameObject);
    bool isInCameraView(AABB * aabb);
    void getCameraPlanes(glm::vec4 * planes);
    void updateMatrices(int windowWidth, int windowHeight);
    void drawFrustumPlanes();
};

#endif

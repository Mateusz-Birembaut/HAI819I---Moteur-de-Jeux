#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <GLFW/glfw3.h>
#include <iostream>

class GameObject;

struct Controller {

    float movementSpeed = 1.0;
    float offsetY = 1.f;

    void rayCastDown(GameObject * go);
    void handleInputs(float deltaTime, GLFWwindow* window, GameObject * go);
};

#endif
#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "Transform.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

struct Controller {

    float movementSpeed = 1.0;

    void handleInputs(float deltaTime, GLFWwindow* window, Transform * transform);
};

#endif
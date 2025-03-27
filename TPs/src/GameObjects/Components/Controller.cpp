#include "Controller.hpp"
#include <GLFW/glfw3.h>

    void Controller::handleInputs(float deltaTime, GLFWwindow* window, Transform * transform){
        float displacement = movementSpeed * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            transform->translation[2] += displacement;
            transform->isDirty = true;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            transform->translation[2] -= displacement;
            transform->isDirty = true;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            transform->translation[0] -= displacement;
            transform->isDirty = true;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            transform->translation[0] += displacement;
            transform->isDirty = true;
        }
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
            transform->translation[1] -= displacement;
            transform->isDirty = true;
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
            transform->translation[1] += displacement;
            transform->isDirty = true;
        }
    }
#ifndef COLLIDER_HPP
#define COLLIDER_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "AABB.hpp"

class Collider {
public:
    AABB aabb;
    bool isTrigger = false;

    Collider();
    
    /// @brief Draws collider, only calls
    /// @param programID 
    /// @param modelMatrix 
    void drawCollider(GLuint programID, const glm::mat4& modelMatrix);
};

#endif // COLLIDER_HPP

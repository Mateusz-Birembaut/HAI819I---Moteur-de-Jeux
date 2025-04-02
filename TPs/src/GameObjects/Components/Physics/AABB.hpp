#ifndef AABB_HPP
#define AABB_HPP
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <limits>
#include <vector>

#include "../Controller.hpp"
#include "../../../UI/ImGuiConsole.hpp"

class Mesh;

struct AABB {
    glm::vec3 min;
    glm::vec3 max;
    glm::vec3 worldMin;
    glm::vec3 worldMax;
    bool show = false;

    AABB();

    /// @brief Checks if the other AABB is contained 
    /// @param other AABB
    /// @return True if is contained, False otherwise
    bool contains(const AABB& other);

    /// @brief Checks if the point is contained in the AABB
    /// @param point The point to check
    /// @return True if the point is contained, False otherwise
    bool containsOrBelow(const glm::vec3& point);

    /// @brief Compute the overlap value of the other AABB.
    /// @param other AABB
    /// @return The overlap volume
    float overlapValue(AABB * other);

    /// @brief Does what it says
    /// @param other 
    /// @return True if collision, False otherwise
    bool checkCollision(const AABB &other);

    /// @brief Update the local min and max to the mesh min and max
    /// @param mesh 
    void fitToMesh(Mesh* mesh);


    /// @brief Draw the AABB
    /// @param programID 
    /// @param modelMatrix 
    void draw(GLuint programID, const glm::mat4& modelMatrix);

    /// @brief Loops throw every corner, updates their position using the modelMatrix.
    /// Then sets the world min max using the min and max value of all the transformer corners 
    /// @param modelMatrix 
    void updateWorldMinMax(const glm::mat4 & modelMatrix);
};

#endif

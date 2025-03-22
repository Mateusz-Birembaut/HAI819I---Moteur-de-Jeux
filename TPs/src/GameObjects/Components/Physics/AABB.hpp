#ifndef AABB_HPP
#define AABB_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <limits>
#include <vector>

#include "../Mesh.hpp"
#include "../../../UI/ImGuiConsole.hpp"

struct AABB {
    glm::vec3 min;
    glm::vec3 max;
    glm::vec3 worldMin;
    glm::vec3 worldMax;
    glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

    AABB();

    bool contains(const AABB& other);
    float overlapValue(AABB * other);
    bool checkCollision(const AABB &other);
    void fitToMesh(Mesh* mesh);
    void draw(GLuint programID, const glm::mat4& modelMatrix);
    void updateWorldMinMax(const glm::mat4 & modelMatrix);
};

#endif

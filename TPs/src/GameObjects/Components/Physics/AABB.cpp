#include "AABB.hpp"
#include <glm/gtc/type_ptr.hpp>


AABB::AABB() : min(-0.5f), max(0.5f) {}


bool AABB::checkCollision(const AABB& other) {
    if (worldMax.x < other.worldMin.x || worldMin.x > other.worldMax.x) return false;
    if (worldMax.y < other.worldMin.y || worldMin.y > other.worldMax.y) return false;
    if (worldMax.z < other.worldMin.z || worldMin.z > other.worldMax.z) return false;
    return true;
}

void AABB::fitToMesh(Mesh* mesh) {
    min = glm::vec3(std::numeric_limits<float>::max());
    max = glm::vec3(std::numeric_limits<float>::lowest());

    for (const auto& vertex : mesh->getVertices()) {
        min = glm::min(min, vertex.position);
        max = glm::max(max, vertex.position);
    }
}


void AABB::draw(GLuint programID, const glm::mat4& modelMatrix) {
    // All the corners :
    std::vector<glm::vec3> corners = {
        {worldMin.x, worldMin.y, worldMin.z},
        {worldMax.x, worldMin.y, worldMin.z},
        {worldMin.x, worldMax.y, worldMin.z},
        {worldMax.x, worldMax.y, worldMin.z},
        {worldMin.x, worldMin.y, worldMax.z},
        {worldMax.x, worldMin.y, worldMax.z},
        {worldMin.x, worldMax.y, worldMax.z},
        {worldMax.x, worldMax.y, worldMax.z}
    };

    std::vector<std::pair<int, int>> edges = {
        {0, 1}, {1, 3}, {3, 2}, {2, 0}, // Down face
        {4, 5}, {5, 7}, {7, 6}, {6, 4}, // Up Face
        {0, 4}, {1, 5}, {2, 6}, {3, 7}  // Vertical faces
    };

    std::vector<glm::vec3> positions;
    for (const auto& edge : edges) {
        positions.push_back(corners[edge.first]);
        positions.push_back(corners[edge.second]);
    }


    // Create VAO and VBO
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);


    // Setring default values for the shader

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    glDisableVertexAttribArray(1);
    glVertexAttrib3f(1, 1, 0, 0);

    glDisableVertexAttribArray(2);
    glVertexAttrib3f(2, 0.0f, 1.0f, 0.0f); 

    glDisableVertexAttribArray(3);
    glVertexAttrib2f(3, 0.0f, 0.0f); 

    GLint modelLoc = glGetUniformLocation(programID, "u_model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

    GLint hasTextureLoc = glGetUniformLocation(programID, "u_hasTexture");
    if (hasTextureLoc != -1) {
        glUniform1i(hasTextureLoc, 0); 
    }

    glDrawArrays(GL_LINES, 0, positions.size());

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}


void AABB::updateWorldMinMax(const glm::mat4 & modelMatrix) {  
    worldMin = glm::vec3(std::numeric_limits<float>::max());
    worldMax = glm::vec3(std::numeric_limits<float>::lowest());

    std::vector<glm::vec3> corners = {
        {min.x, min.y, min.z},
        {max.x, min.y, min.z},
        {min.x, max.y, min.z},
        {max.x, max.y, min.z},
        {min.x, min.y, max.z},
        {max.x, min.y, max.z},
        {min.x, max.y, max.z},
        {max.x, max.y, max.z}
    };

    glm::vec4 worldCorner;
    for (const auto& corner : corners) {
        worldCorner = modelMatrix * glm::vec4(corner, 1.0f);

        worldMin.x = std::min(worldMin.x, worldCorner.x);
        worldMin.y = std::min(worldMin.y, worldCorner.y);
        worldMin.z = std::min(worldMin.z, worldCorner.z);

        worldMax.x = std::max(worldMax.x, worldCorner.x);
        worldMax.y = std::max(worldMax.y, worldCorner.y);
        worldMax.z = std::max(worldMax.z, worldCorner.z);
    } 
}


bool AABB::contains(const AABB& other){
    if(worldMax.x < other.worldMax.x) return false;
    if(worldMax.y < other.worldMax.y) return false;
    if(worldMax.z < other.worldMax.z) return false;

    if(worldMin.x > other.worldMin.x) return false;
    if(worldMin.y > other.worldMin.y) return false;
    if(worldMin.z > other.worldMin.z) return false;

    return true;
}


float AABB::overlapValue(AABB * other){
    if (!checkCollision(*other)){
        return 0.0f;
    }

    float xOverlap = std::max(worldMax.x, other->worldMax.x) - std::max(worldMin.x, other->worldMin.x);
    float yOverlap = std::max(worldMax.y, other->worldMax.y) - std::max(worldMin.y, other->worldMin.y);
    float zOverlap = std::max(worldMax.z, other->worldMax.z) - std::max(worldMin.z, other->worldMin.z);

    float overlapVolume = xOverlap * yOverlap * zOverlap;
    
    return overlapVolume;

}
#ifndef AABB_HPP
#define AABB_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <limits>

#include "../Mesh.hpp"
#include "../../../Ressources/Structs.hpp"
#include "../../../UI/ImGuiConsole.hpp"
#include <vector>
#include "Collider.hpp"
#include <glm/gtc/type_ptr.hpp>

struct AABB {
    glm::vec3 min;
    glm::vec3 max;
 
    glm::vec3 worldMin;
    glm::vec3 worldMax;
 
    glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

    AABB() : min(0.0f), max(1.0f) {}
    
    
    void fitToMesh(Mesh * mesh) {
        min = glm::vec3(std::numeric_limits<float>::max());
        max = glm::vec3(std::numeric_limits<float>::lowest());
        
        for (const auto& vertex : mesh->getVertices()) {
            min = glm::min(min, vertex.position);
            max = glm::max(max, vertex.position);
        }
    }


    //TODO : modif pour avoir qu'un seul vbo ect ?
    //TODO : pour avoir la bouding box qui reste axis aligned
    void draw(GLuint programID, const glm::mat4& modelMatrix) {
        updateWorldMinMax(modelMatrix);

        // Créer les 8 sommets de la boîte
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
        
        // Définir les arêtes
        std::vector<std::pair<int, int>> edges = {
            {0, 1}, {1, 3}, {3, 2}, {2, 0}, // Face du bas
            {4, 5}, {5, 7}, {7, 6}, {6, 4}, // Face du haut
            {0, 4}, {1, 5}, {2, 6}, {3, 7}  // Arêtes verticales
        };
        
        // Create simple vertices with only position
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
        
        // Position attribute (0)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
        
        // Set default color attribute (1) to your AABB color
        glDisableVertexAttribArray(1);
        glVertexAttrib3f(1, color.x, color.y, color.z);
        
        // Set default values for normal and UV attributes
        glDisableVertexAttribArray(2);
        glVertexAttrib3f(2, 0.0f, 1.0f, 0.0f); // Default normal
        
        glDisableVertexAttribArray(3);
        glVertexAttrib2f(3, 0.0f, 0.0f); // Default UV
        
        // Set model matrix
        GLint modelLoc = glGetUniformLocation(programID, "u_model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
        
        // Disable textures for this draw call
        GLint hasTextureLoc = glGetUniformLocation(programID, "u_hasTexture");
        if (hasTextureLoc != -1) {
            glUniform1i(hasTextureLoc, 0); // No texture
        }
        
        // Draw lines
        glDrawArrays(GL_LINES, 0, positions.size());
        
        // Clean up
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }

    void updateWorldMinMax(const glm::mat4& modelMatrix) {
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
        
        for (const auto& corner : corners) {
            glm::vec4 worldCorner = modelMatrix * glm::vec4(corner, 1.0f);

            worldMin.x = std::min(worldMin.x, worldCorner.x);
            worldMin.y = std::min(worldMin.y, worldCorner.y);
            worldMin.z = std::min(worldMin.z, worldCorner.z);
            
            worldMax.x = std::max(worldMax.x, worldCorner.x);
            worldMax.y = std::max(worldMax.y, worldCorner.y);
            worldMax.z = std::max(worldMax.z, worldCorner.z);
        }
    }


        

};

#endif
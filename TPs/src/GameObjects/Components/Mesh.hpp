#ifndef MESH_HPP
#define MESH_HPP

#include "../../Ressources/Structs.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <GL/glew.h>


class Mesh {
    protected:
        GLuint VAO, VBO, EBO;
        std::vector<Vertex> vertices;
        std::vector<unsigned short> indexes;

    public:
        Mesh(){}

        ~Mesh() {
        }

        std::vector<Vertex> getVertices(){
            return vertices;
        }

        void setVertices(std::vector<Vertex> _vertices){
            vertices = _vertices;
        }

        void setIndexes(std::vector<unsigned short> _indexes){
            indexes = _indexes;
        }
        
        void draw(GLuint shaderProgram) {
            glUseProgram(shaderProgram);
                
            // Set uniforms
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, indexes.size(), GL_UNSIGNED_SHORT, 0);
            glBindVertexArray(0);
        }

        // Buffer creation
        void createBuffers() {
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);

            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(unsigned short), &indexes[0], GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); // position
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec3)); // color
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3) * 2)); // normal
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3) * 3)); // uv
            glEnableVertexAttribArray(3);

            glBindVertexArray(0);
        }
};

#endif
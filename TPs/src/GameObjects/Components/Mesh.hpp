#ifndef MESH_HPP
#define MESH_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "../../Ressources/Structs.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>


class Mesh {
protected:
    GLuint VAO, VBO, EBO;
    std::vector<Vertex> vertices;
    std::vector<unsigned short> indexes;
    glm::vec3 minBounds;
    glm::vec3 maxBounds;


public:
    Mesh();
    ~Mesh();


    void setMinMaxBounds(glm::vec3 _minBounds, glm::vec3 _maxBounds);

    glm::vec3 & getMinBounds(){ return minBounds; };
    glm::vec3 & getMaxBounds(){ return maxBounds; };


    std::vector<Vertex> getVertices();
    void setVertices(std::vector<Vertex> _vertices);

    std::vector<unsigned short> getIndexes();
    void setIndexes(std::vector<unsigned short> _indexes);

    void draw(GLuint shaderProgram);

    /// @brief create the VAO, VBO and EBO for the mesh using Vertex structure ()
    void createBuffers();
};

#endif

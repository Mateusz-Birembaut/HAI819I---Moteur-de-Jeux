#ifndef MESH_HPP
#define MESH_HPP

#include <GL/glew.h>
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

public:
    Mesh();
    ~Mesh();

    std::vector<Vertex> getVertices();
    std::vector<unsigned short> getIndexes();
    void setVertices(std::vector<Vertex> _vertices);
    void setIndexes(std::vector<unsigned short> _indexes);
    void draw(GLuint shaderProgram);
    void createBuffers();
};

#endif

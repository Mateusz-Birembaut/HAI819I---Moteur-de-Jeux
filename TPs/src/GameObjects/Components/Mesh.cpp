#include "Mesh.hpp"

Mesh::Mesh() {}

Mesh::~Mesh() {}

std::vector<Vertex> Mesh::getVertices() {
    return vertices;
}

std::vector<unsigned short> Mesh::getIndexes() {
    return indexes;
}

void Mesh::setVertices(std::vector<Vertex> _vertices) {
    vertices = _vertices;
}

void Mesh::setIndexes(std::vector<unsigned short> _indexes) {
    indexes = _indexes;
}

void Mesh::draw(GLuint shaderProgram) {
    glUseProgram(shaderProgram);

    // Set uniforms
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexes.size(), GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
}

 void Mesh::createBuffers() {
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
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3))); // uv
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2))); // normal (pitch Yaw)
    glEnableVertexAttribArray(2);


    glBindVertexArray(0);
}

void Mesh::setMinMaxBounds(glm::vec3 _minBounds, glm::vec3 _maxBounds) {
    minBounds = _minBounds;
    maxBounds = _maxBounds;
}
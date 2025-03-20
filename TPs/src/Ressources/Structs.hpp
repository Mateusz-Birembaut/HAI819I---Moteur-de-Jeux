#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <glm/glm.hpp>

// data sent to gpu
struct Vertex{ 
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 normal;
    glm::vec2 uv;
};


#endif
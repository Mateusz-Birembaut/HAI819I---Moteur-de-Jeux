#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <glm/glm.hpp>

// data sent to gpu
struct Vertex{ 
    glm::vec3 position;
    glm::vec2 uv;
    unsigned short pitchYaw[2];

};


#endif
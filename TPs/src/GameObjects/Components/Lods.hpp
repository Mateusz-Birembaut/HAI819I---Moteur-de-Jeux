#ifndef LODS_HPP
#define LODS_HPP

#include <GL/glew.h>
#include <algorithm>
#include <glm/glm.hpp>

#include "Mesh.hpp"

struct Lods
{

    Lods() {};

    std::vector<std::pair<unsigned short, Mesh *>> meshes;

    Mesh *getMesh(float distance);
    void addMesh(unsigned short lod, Mesh *mesh);
    void sortLods();
};

#endif // RESSOURCES_MANAGER_HPP
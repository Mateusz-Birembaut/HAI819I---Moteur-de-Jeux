#include "Lods.hpp"

void Lods::sortLods()
{
    std::sort(meshes.begin(), meshes.end(),
              [](const std::pair<unsigned short, Mesh *> &a, const std::pair<unsigned short, Mesh *> &b)
              {
                  return a.first < b.first;
              });
}

void Lods::addMesh(unsigned short lod, Mesh *mesh)
{
    meshes.push_back(std::make_pair(lod, mesh));
    sortLods();
}

Mesh *Lods::getMesh(float distance)
{
    for (auto &pair : meshes)
    {
        if (distance < pair.first)
        {
            return pair.second;
        }
    }
    return meshes.back().second;
}
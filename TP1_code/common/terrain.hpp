//// filepath: /home/mat/HAI819I---Moteur-de-Jeux/TP1_code/common/terrain.hpp
#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include "structs.hpp"

class Terrain {
private:
    int VertexCountX;
    int VertexCountY;
    float sizeX;
    float sizeY; 

public:
    Terrain() 
      : VertexCountX(10), VertexCountY(10), sizeX(1.0f), sizeY(1.0f) {}

    Terrain(int nX, int nY, float sX = 2.0f, float sY = 2.0f)
      : VertexCountX(nX), VertexCountY(nY), sizeX(sX), sizeY(sY) {}

    void create(std::vector<unsigned short> & indices, 
                std::vector<std::vector<unsigned short> > & triangles,
                std::vector<Vertex> & indexed_vertices)
    {
        indices.clear();
        triangles.clear();
        indexed_vertices.clear();

        for (int i = 0; i < VertexCountX; i++) {
            for (int j = 0; j < VertexCountY; j++) {
                float t_x = float(i) / float(VertexCountX - 1);
                float t_z = float(j) / float(VertexCountY - 1);

                float x = t_x * sizeX - (sizeX * 0.5f);
                float z = t_z * sizeY - (sizeY * 0.5f);

                float y = 0.0f;

                Vertex v;
                v.position = glm::vec3(x, y, z);
                //v.color = ...
                //v.normal = ...
                v.uv = glm::vec2( t_x, t_z );

                indexed_vertices.push_back(v);
            }
        }

        for (int i = 0; i < VertexCountX; i ++) {
            for (int j = 0; j < VertexCountY; j++) {
                if(i != VertexCountX - 1 && j != VertexCountY - 1){
                    unsigned short baseIndex       = i * VertexCountX + j;
                    unsigned short rightNeighbor   = i * VertexCountX + (j + 1);
                    unsigned short topNeighbor     = (i + 1) * VertexCountX + j;
                    unsigned short topRightNeighbor = (i + 1) * VertexCountX + (j + 1);

                    indices.push_back(baseIndex);
                    indices.push_back(topNeighbor);
                    indices.push_back(rightNeighbor);
                    triangles.push_back({ baseIndex, topNeighbor, rightNeighbor });

                    indices.push_back(topRightNeighbor);
                    indices.push_back(rightNeighbor);
                    indices.push_back(topNeighbor);
                    triangles.push_back({ topRightNeighbor, rightNeighbor, topNeighbor });
                }
            }
        }
    }
};

#endif
#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <glm/fwd.hpp>
#include <glm/glm.hpp>

#include "../Ressources/Structs.hpp"
#include "../GameObjects/Components/Mesh.hpp"

class Sphere {


    public:

        static void create(Mesh & mesh, int VertexCountX, int VertexCountY) {
            std::vector<Vertex> vertices;
            std::vector<unsigned short> indexes;

            float x, y, z, xy;                          // vertex position
            float sectorStep = 2 * M_PI / VertexCountX;   // angle increment
            float stackStep = M_PI / VertexCountY;         // angle increment
            float sectorAngle, stackAngle;

            for (int i = 0; i <= VertexCountY; ++i) {
                stackAngle = M_PI / 2 - i * stackStep;  // starting at pi/2, going to -pi/2
                xy = cosf(stackAngle);         // radius at stack
                z = sinf(stackAngle);          // z coordinate

                for (int j = 0; j <= VertexCountX; ++j) {
                    sectorAngle = j * sectorStep;      // sector angle

                    x = xy * cosf(sectorAngle);         // x coordinate
                    y = xy * sinf(sectorAngle);         // y coordinate

                    Vertex v;
                    // Modifier l'orientation des coordonnées pour que Z soit l'axe vertical (haut-bas)
                    v.position = glm::vec3(x, z, y);  // Modifié: permutation des coordonnées y et z
                    v.color = glm::vec3(0.0f, 1.0f, 0.0f); // green color for sphere
                    
                    // Normaliser correctement la normale en fonction des nouvelles coordonnées
                    v.normal = glm::normalize(glm::vec3(x, z, y));
                    
                    // Coordonnées UV ajustées pour les mappings de textures terrestres
                    // Longitude (j) -> U, avec décalage pour avoir Greenwich au bon endroit
                    // Latitude (i) -> V inversé pour que le pôle nord soit en haut
                    float u = float(j) / VertexCountX;
                    u = fmod(u - 0.25f, 1.0f); // Décalage pour aligner les continents
                    v.uv = glm::vec2(u, 1.0f - (float(i) / VertexCountY));

                    vertices.push_back(v);
                }
            }
            // indices for drawing triangles
            unsigned short k1, k2;
            for (int i = 0; i < VertexCountY; ++i) {
                k1 = i * (VertexCountX + 1);     // current stack
                k2 = k1 + VertexCountX + 1;      // next stack

                for (int j = 0; j < VertexCountX; ++j, ++k1, ++k2) {
                    if (i != 0) {
                        indexes.push_back(k1);
                        indexes.push_back(k1 + 1);
                        indexes.push_back(k2);
                    }
                    if (i != (VertexCountY - 1)) {
                        indexes.push_back(k1 + 1);
                        indexes.push_back(k2 + 1);
                        indexes.push_back(k2);
                    }
                }
            }


            mesh.setVertices(vertices);
            mesh.setIndexes(indexes);
            mesh.createBuffers();
        }

};
    



#endif
#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "Mesh.hpp"
#include <glm/fwd.hpp>
#include <glm/glm.hpp>

class Sphere : public Mesh {
    private:
        int VertexCountX;
        int VertexCountY;
        float radius;
        glm::vec3 origin;

    public:

        Sphere() : VertexCountX(10), VertexCountY(10), radius(1.0f), origin({0.0f,0.0f, 0.0f }) {}

        Sphere(int nX, int nY, float r = 1.0f, glm::vec3 o = {0.0f,0.0f, 0.0f})
        : VertexCountX(nX), VertexCountY(nY), radius(r), origin(o) {
            create(); 
        }

        void create() override {
            vertices.clear();
            indexes.clear();

            float x, y, z, xy;                          // vertex position
            float sectorStep = 2 * M_PI / VertexCountX;   // angle increment
            float stackStep = M_PI / VertexCountY;         // angle increment
            float sectorAngle, stackAngle;

            for (int i = 0; i <= VertexCountY; ++i) {
                stackAngle = M_PI / 2 - i * stackStep;  // starting at pi/2, going to -pi/2
                xy = radius * cosf(stackAngle);         // radius at stack
                z = radius * sinf(stackAngle);          // z coordinate

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
                        indexes.push_back(k2);
                        indexes.push_back(k1 + 1);
                    }
                    if (i != (VertexCountY - 1)) {
                        indexes.push_back(k1 + 1);
                        indexes.push_back(k2);
                        indexes.push_back(k2 + 1);
                    }
                }
            }

            createBuffers();
        }


        void draw(GLuint shaderProgram) override {
            glUseProgram(shaderProgram);
            
            // Set uniforms
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, indexes.size(), GL_UNSIGNED_SHORT, 0);
            glBindVertexArray(0);
        }
};
    



#endif
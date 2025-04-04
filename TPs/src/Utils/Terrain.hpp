#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include "../Ressources/Structs.hpp"
#include "../GameObjects/Components/Mesh.hpp"
#include "../../common/Conversion.hpp"

class Terrain {

    public:

        static void create(Mesh & mesh, int VertexCountX, int VertexCountZ) {
            std::vector<Vertex> vertices;
            std::vector<unsigned short> indexes;

            glm::vec3 minBounds(std::numeric_limits<float>::max());
            glm::vec3 maxBounds(std::numeric_limits<float>::lowest());

            for (int i = 0; i < VertexCountX; i++) {
                for (int j = 0; j < VertexCountZ; j++) {
                    float t_x = float(i) / float(VertexCountX - 1);
                    float t_z = float(j) / float(VertexCountZ - 1);

                    float x = t_x - 0.5f;
                    float z = t_z - 0.5f;

                    float y = 0.0f;

                    Vertex v;
                    v.position = glm::vec3(x, y, z);

                    minBounds.x = std::min(minBounds.x, x);
                    minBounds.y = std::min(minBounds.y, y);
                    minBounds.z = std::min(minBounds.z, z);
                    
                    maxBounds.x = std::max(maxBounds.x, x);
                    maxBounds.y = std::max(maxBounds.y, y);
                    maxBounds.z = std::max(maxBounds.z, z);
                    

                    normalToUnsignedShort(glm::vec3(0.0f,1.0f,0.0f) , v.pitchYaw[0], v.pitchYaw[1]);
                    v.uv = glm::vec2(t_x , t_z);

                    vertices.push_back(v);
                }
            }

            for (int i = 0; i < VertexCountX; i ++) {
                for (int j = 0; j < VertexCountZ; j++) {
                    if(i != VertexCountX - 1 && j != VertexCountZ - 1){
                        unsigned short baseIndex       = i * VertexCountX + j;
                        unsigned short rightNeighbor   = i * VertexCountX + (j + 1);
                        unsigned short topNeighbor     = (i + 1) * VertexCountX + j;
                        unsigned short topRightNeighbor = (i + 1) * VertexCountX + (j + 1);

                        indexes.push_back(baseIndex);
                        indexes.push_back(rightNeighbor);
                        indexes.push_back(topNeighbor);

                        indexes.push_back(topRightNeighbor);
                        indexes.push_back(topNeighbor);
                        indexes.push_back(rightNeighbor);
                    }
                }
            }

            mesh.setMinMaxBounds(minBounds, maxBounds);
            mesh.setVertices(vertices);
            mesh.setIndexes(indexes);
            mesh.createBuffers();
        }
        
/* 
        void handleInputs(GLFWwindow *window) {
            static bool zPressed = false;
            static bool xPressed = false;

            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);

            if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && VertexCountZ < 256) {
                if (!zPressed) {
                    VertexCountX++;
                    VertexCountZ++;
                    std::cout << VertexCountX << " " << VertexCountZ << std::endl;
                    create();
                    zPressed = true;
                }
            } else {
                zPressed = false;
            }

            if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && VertexCountZ > 2) {
                if (!xPressed) {
                    VertexCountX--;
                    VertexCountZ--;
                    std::cout << VertexCountX << " " << VertexCountZ << std::endl;
                    create();
                    xPressed = true;
                }
            } else {
                xPressed = false;
            }
        }
 */

};

#endif
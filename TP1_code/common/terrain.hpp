#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include "Mesh.hpp"
#include "structs.hpp"
#include <GLFW/glfw3.h>


class Terrain : public Mesh {
private:
    int VertexCountX;
    int VertexCountZ;
    float sizeX;
    float sizeZ; 
    float originX;
    float originZ;

public:
    Terrain() 
      : VertexCountX(10), VertexCountZ(10), sizeX(1.0f), sizeZ(1.0f), originX(0.0f), originZ(0.0f) {}

    Terrain(int nX, int nY, float sX = 1.0f, float sY = 1.0f, float oX = 0.0f, float oZ = 0.0f)
      : VertexCountX(nX), VertexCountZ(nY), sizeX(sX), sizeZ(sY), originX(oX), originZ(oZ) {
        create(); 
      }

    void create() override {
        vertices.clear();
        indexes.clear();

        for (int i = 0; i < VertexCountX; i++) {
            for (int j = 0; j < VertexCountZ; j++) {
                float t_x = float(i) / float(VertexCountX - 1);
                float t_z = float(j) / float(VertexCountZ - 1);

                float x = t_x * sizeX - (sizeX * 0.5f) + originX;
                float z = t_z * sizeZ - (sizeZ * 0.5f) + originZ;

                float y = 0.0f;

                Vertex v;
                v.position = glm::vec3(x, y, z);
                //v.color = 
                //v.normal = 
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
                    indexes.push_back(topNeighbor);
                    indexes.push_back(rightNeighbor);

                    indexes.push_back(topRightNeighbor);
                    indexes.push_back(rightNeighbor);
                    indexes.push_back(topNeighbor);
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



};

#endif
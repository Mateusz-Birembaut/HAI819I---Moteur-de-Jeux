#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include "Mesh.hpp"
#include "texture.hpp"
#include "Transform.hpp"
#include <glm/fwd.hpp>
#include <list>
#include <memory>

class GameObject{

    public:
        GameObject* parent = nullptr;
        std::vector<GameObject*> children;

        Transform transformation; 

        Texture * texture;

        Mesh * mesh;
    
        int objectID = 0;

        GameObject() : parent(nullptr), texture(nullptr), mesh(nullptr){}


        void addChild(GameObject * child) {
            if (child) {
                child->parent = this;
                children.push_back(child);
            }
        }

        void updateSelfAndChild(float deltaTime){
            if (parent)
                transformation.modelMatrix = parent->transformation.modelMatrix * transformation.getLocalModelMatrix(deltaTime);
            else
                transformation.modelMatrix = transformation.getLocalModelMatrix(deltaTime);

            for (auto&& child : children){
                child->updateSelfAndChild(deltaTime);
            }
        }

        void drawSelfAndChild(GLuint shaderProgram) {
            GLuint objectIDLoc = glGetUniformLocation(shaderProgram, "objectID");
            glUniform1i(objectIDLoc, objectID);

            GLuint modelLoc = glGetUniformLocation(shaderProgram, "u_model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &transformation.modelMatrix[0][0]);        

            mesh->draw(shaderProgram); 

            for (auto&& child : children) {
                child->drawSelfAndChild(shaderProgram);
            }
        }
};


#endif
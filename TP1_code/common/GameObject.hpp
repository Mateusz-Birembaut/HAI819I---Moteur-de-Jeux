#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include "Mesh.hpp"
#include "Transform.hpp"
#include <glm/fwd.hpp>
#include <list>
#include <memory>

template<typename MeshType>
class GameObject{

    public:
        GameObject* parent = nullptr;
        std::vector<GameObject*> children;

        Transform transformation; 

        MeshType mesh;
    
        GameObject() : parent(nullptr){}


        void addChild(GameObject * child) {
            if (child) {
                child->parent = this;
                children.push_back(child);
            }
        }

        void updateSelfAndChild(){
            if (parent)
                transformation.modelMatrix = parent->transformation.modelMatrix * transformation.getLocalModelMatrix();
            else
                transformation.modelMatrix = transformation.getLocalModelMatrix();

            for (auto&& child : children){
                child->updateSelfAndChild();
            }
        }

        void draw(GLuint shaderProgram) {
            GLuint modelLoc = glGetUniformLocation(shaderProgram, "u_model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &transformation.modelMatrix[0][0]);        

            mesh.draw(shaderProgram); 

            for (auto&& child : children) {
                child->draw(shaderProgram);
            }
        }
};


#endif
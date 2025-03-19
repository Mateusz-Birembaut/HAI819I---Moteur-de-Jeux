#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP


#include "Components/Transform.hpp"
#include "Components/Mesh.hpp"
#include "Components/Texture.hpp"
#include "Components/Physics/RigidBody.hpp"
#include "Components/Physics/Collider.hpp"

#include "../Ressources/Globals.hpp"
#include "../Rendering/Camera.hpp"

#include "../Ressources/IdGenerator.hpp"
#include <glm/fwd.hpp>
#include <list>
#include <memory>

class GameObject{

    public:

        std::string gameObjectId;

        Transform transformation; 

        GameObject* parent = nullptr;
        std::vector<GameObject*> children;

        Texture* texture;

        Mesh* mesh;

        RigidBody* rigidBody;

        Collider* collider;


        GameObject() : gameObjectId(generateId()), parent(nullptr), texture(nullptr), mesh(nullptr), transformation(), collider(nullptr), rigidBody(nullptr){}

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

        void GameObject::drawSelfAndChild(GLuint shaderProgram) {
            bool toBeDrawn = true;
        
            //also checks if has collider
            if(!Camera::getInstance().isInCameraView(this)){
                toBeDrawn = false;
            }
        
            if (toBeDrawn && collider->showCollider) {
                collider->drawCollider(shaderProgram, transformation.modelMatrix);
            }
        
            if (mesh == nullptr) {
                toBeDrawn = false;
            }
            
            if (toBeDrawn){
                GLuint modelLoc = glGetUniformLocation(shaderProgram, "u_model");
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &transformation.modelMatrix[0][0]); 
        
                GLint hasTextureLoc = glGetUniformLocation(shaderProgram, "u_hasTexture");
            
                if (texture != nullptr) {
                    glUniform1i(hasTextureLoc, 1);
                    texture->bind(GL_TEXTURE0);  
                    GLint textureLoc = glGetUniformLocation(shaderProgram, "u_texture");
                    glUniform1i(textureLoc, 0);  
                }else {
                    glUniform1i(hasTextureLoc, 0);
                }
        
                mesh->draw(shaderProgram); 
            }
        
            for (auto&& child : children) {
                child->drawSelfAndChild(shaderProgram);
            }
        }

        void getSelfAndChild(std::vector<GameObject*>& objects) {
            objects.push_back(this);
            for (auto&& child : children) {
                child->getSelfAndChild(objects);
            }
        }

        void removeChild(GameObject * child) {
            for (size_t i = 0; i < children.size(); ++i) {
                if (children[i] == child) {
                    children.erase(children.begin() + i);
                    break;
                }
            }
        }


};


#endif
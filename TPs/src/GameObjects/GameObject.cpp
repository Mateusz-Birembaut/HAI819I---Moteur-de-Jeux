#include <GL/glew.h>

#include "GameObject.hpp"
#include "Components/Physics/AABB.hpp"
#include "Components/Physics/Collider.hpp"
#include "Components/Physics/RigidBody.hpp"
#include "Components/Controller.hpp"

#include "../Rendering/Camera.hpp"
#include "../Ressources/IdGenerator.hpp"
#include "../Ressources/Globals.hpp"

// Constructor implementation
GameObject::GameObject() : 
    gameObjectId(generateId()),
    parent(nullptr),
    texture(nullptr),
    heightmap(nullptr),
    mesh(nullptr),
    transformation(),
    collider(nullptr),
    rigidBody(nullptr),
    controller(nullptr),
    lods(nullptr),
    cullingAABB(){}


void GameObject::addChild(GameObject* child) {
    if (child) {
        child->parent = this;
        children.push_back(child);
    }
}

void GameObject::updateSelfAndChild(float deltaTime) {
    bool parentUpdated = false;
    bool toBeUpdated = true;

    if (frustumCulling){
        // if not in view et is not dirty, do not update 
        if (!Camera::getInstance().isInCameraView(this) && !transformation.needsUpdate()) {
            toBeUpdated = false;
        }
    }
    
    if(toBeUpdated){
        transformation.modelMatrix = transformation.getLocalModelMatrix(deltaTime);
        
        if (parent) {
            transformation.modelMatrix = parent->transformation.modelMatrix * transformation.modelMatrix;
        }
        
        cullingAABB.updateWorldMinMax(transformation.modelMatrix);

        if (collider != nullptr){
            collider->aabb.updateWorldMinMax(transformation.modelMatrix);
        }
    }
    
    for (auto&& child : children) {
        if (toBeUpdated) {
            child->transformation.isDirty = true;
        }
        child->updateSelfAndChild(deltaTime);
    }
}

void GameObject::drawSelfAndChild(GLuint shaderProgram, int & nbOfDraw) {
    bool toBeDrawn = true;

    if (frustumCulling){
        if (!Camera::getInstance().isInCameraView(this)) {
            toBeDrawn = false;
        }
    }

    if (toBeDrawn){

        if (collider && collider->aabb.show) {
            collider->drawCollider(shaderProgram, transformation.modelMatrix);
        }

        if (cullingAABB.show){
            cullingAABB.draw(shaderProgram, transformation.modelMatrix);
        }

        if (lods != nullptr)
        {
            float distToCamera = Camera::getInstance().getDistanceFrom(transformation.translation);
            Mesh* meshToDraw = lods->getMesh(distToCamera);
            
            nbOfDraw++;
            GLuint modelLoc = glGetUniformLocation(shaderProgram, "u_model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &transformation.modelMatrix[0][0]); 
    
            GLint hasTextureLoc = glGetUniformLocation(shaderProgram, "u_hasTexture");
            GLint hasHeightmapLoc = glGetUniformLocation(shaderProgram, "u_hasHeightmap");

            if (texture != nullptr) {
                glUniform1i(hasTextureLoc, 1);
                texture->bind(GL_TEXTURE0);  
                GLint textureLoc = glGetUniformLocation(shaderProgram, "u_texture");
                glUniform1i(textureLoc, 0);  
            } else {
                glUniform1i(hasTextureLoc, 0);
            }

            if (heightmap != nullptr) {
                glUniform1i(hasHeightmapLoc, 1);
                heightmap->bind(GL_TEXTURE1);  
                GLint heightmapLoc = glGetUniformLocation(shaderProgram, "u_heightmap");
                glUniform1i(heightmapLoc, 1);  
            } else {
                glUniform1i(hasHeightmapLoc, 0);
            }
    
            meshToDraw->draw(shaderProgram); 
            
        }else if (mesh != nullptr) {
            nbOfDraw++;
            GLuint modelLoc = glGetUniformLocation(shaderProgram, "u_model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &transformation.modelMatrix[0][0]); 
    
            GLint hasTextureLoc = glGetUniformLocation(shaderProgram, "u_hasTexture");
            GLint hasHeightmapLoc = glGetUniformLocation(shaderProgram, "u_hasHeightmap");

            if (texture != nullptr) {
                glUniform1i(hasTextureLoc, 1);
                texture->bind(GL_TEXTURE0);  
                GLint textureLoc = glGetUniformLocation(shaderProgram, "u_texture");
                glUniform1i(textureLoc, 0);  
            } else {
                glUniform1i(hasTextureLoc, 0);
            }

            if (heightmap != nullptr) {
                glUniform1i(hasHeightmapLoc, 1);
                heightmap->bind(GL_TEXTURE1);  
                GLint heightmapLoc = glGetUniformLocation(shaderProgram, "u_heightmap");
                glUniform1i(heightmapLoc, 1);  
            } else {
                glUniform1i(hasHeightmapLoc, 0);
            }
    
            mesh->draw(shaderProgram); 
        }
    }
    

    for (auto&& child : children) {
        child->drawSelfAndChild(shaderProgram, nbOfDraw);
    }
}

void GameObject::getSelfAndChild(std::vector<GameObject*>& objects) {
    objects.push_back(this);
    for (auto&& child : children) {
        child->getSelfAndChild(objects);
    }
}

void GameObject::removeChild(GameObject* child) {
    for (size_t i = 0; i < children.size(); ++i) {
        if (children[i] == child) {
            children.erase(children.begin() + i);
            break;
        }
    }
}
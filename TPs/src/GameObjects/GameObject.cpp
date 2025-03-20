#include <GL/glew.h>

#include "GameObject.hpp"
#include "../Rendering/Camera.hpp"
#include "../Ressources/IdGenerator.hpp"
#include "../Ressources/Globals.hpp"

// Constructor implementation
GameObject::GameObject() 
    : gameObjectId(generateId()), 
      parent(nullptr), 
      texture(nullptr), 
      mesh(nullptr), 
      transformation(), 
      collider(nullptr), 
      rigidBody(nullptr) {}

void GameObject::addChild(GameObject* child) {
    if (child) {
        child->parent = this;
        children.push_back(child);
    }
}

void GameObject::updateSelfAndChild(float deltaTime) {
    if (parent)
        transformation.modelMatrix = parent->transformation.modelMatrix * transformation.getLocalModelMatrix(deltaTime);
    else
        transformation.modelMatrix = transformation.getLocalModelMatrix(deltaTime);

    if (collider != nullptr){
        collider->aabb.updateWorldMinMax(transformation.modelMatrix);
    }

    for (auto&& child : children) {
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

    if (toBeDrawn && collider && collider->showCollider) {
        collider->drawCollider(shaderProgram, transformation.modelMatrix);
    }

    if (mesh == nullptr) {
        toBeDrawn = false;
    }
    
    if (toBeDrawn) {
        nbOfDraw++;
        GLuint modelLoc = glGetUniformLocation(shaderProgram, "u_model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &transformation.modelMatrix[0][0]); 

        GLint hasTextureLoc = glGetUniformLocation(shaderProgram, "u_hasTexture");
    
        if (texture != nullptr) {
            glUniform1i(hasTextureLoc, 1);
            texture->bind(GL_TEXTURE0);  
            GLint textureLoc = glGetUniformLocation(shaderProgram, "u_texture");
            glUniform1i(textureLoc, 0);  
        } else {
            glUniform1i(hasTextureLoc, 0);
        }

        mesh->draw(shaderProgram); 
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
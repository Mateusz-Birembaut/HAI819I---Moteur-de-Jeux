#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP


#include <vector>
#include <string>

#include "Components/Transform.hpp"
#include "Components/Mesh.hpp"
#include "Components/Texture.hpp"
#include "Components/Physics/RigidBody.hpp"
#include "Components/Physics/Collider.hpp"

class GameObject {
public:
    std::string gameObjectId;
    Transform transformation; 
    AABB cullingAABB;
    GameObject* parent = nullptr;
    std::vector<GameObject*> children;
    Texture* texture;
    Mesh* mesh;
    RigidBody* rigidBody;
    Collider* collider;

    GameObject();

    void addChild(GameObject* child);
    void updateSelfAndChild(float deltaTime);
    void drawSelfAndChild(GLuint shaderProgram, int & nb);
    void getSelfAndChild(std::vector<GameObject*>& objects);
    void removeChild(GameObject* child);
};

#endif
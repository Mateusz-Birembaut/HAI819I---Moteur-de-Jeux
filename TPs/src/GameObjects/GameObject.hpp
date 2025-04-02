#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP


#include <vector>
#include <string>

#include "Components/Transform.hpp"
#include "Components/Mesh.hpp"
#include "Components/Physics/AABB.hpp"
#include "Components/Texture.hpp"
#include "Components/Lods.hpp"

class AABB;
class RigidBody;
class Collider;
class Controller;


class GameObject {
public:
    std::string gameObjectId;
    Transform transformation; 
    AABB cullingAABB;
    GameObject* parent = nullptr;
    std::vector<GameObject*> children;
    Texture* texture;
    Texture* heightmap;
    Mesh* mesh;
    RigidBody* rigidBody;
    Collider* collider;
    Controller* controller;
    Lods* lods;

    GameObject();

    void addChild(GameObject* child);
    void updateSelfAndChild(float deltaTime);
    void drawSelfAndChild(GLuint shaderProgram, int & nb);
    void getSelfAndChild(std::vector<GameObject*>& objects);
    void removeChild(GameObject* child);
};

#endif
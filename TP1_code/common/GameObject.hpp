#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include "Mesh.hpp"
#include "Transform.hpp"
#include <glm/fwd.hpp>
#include <list>

class GameObject{

    public:
        GameObject* parent = nullptr;
        std::list<std::unique_ptr<GameObject>> children;

        Transform transformation; 

        Mesh mesh;
    
        GameObject() : parent(nullptr){}

        template<typename T>
        void addChild(const T child){
            children.emplace_back(std::make_unique<GameObject>(child));
            children.back()->parent = this;
        }

        void updateSelfAndChild()
        {
            if (parent)
                transformation->modelMatrix = parent->transformation->modelMatrix * transformation->getLocalModelMatrix();
            else
                transformation->modelMatrix = transformation->getLocalModelMatrix();

            for (auto&& child : children){
                child->updateSelfAndChild();
            }
        }

        void draw() {
            mesh.draw(); 
        }
};


#endif
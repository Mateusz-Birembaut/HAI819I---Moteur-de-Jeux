#ifndef SCENE_GRAPH_HPP
#define SCENE_GRAPH_HPP

#include "GameObject.hpp"
#include <vector>
#include <memory>
#include <algorithm>
#include <typeinfo>

class SceneGraph {
    
private:
    std::vector<GameObject*> rootObjects;
    
public:
    SceneGraph() {}
    
    ~SceneGraph() {
        clear();
    }
    
    void addObject(GameObject * object) {
        if (!object) return;
        
        if (object->parent == nullptr) {
            rootObjects.push_back(object);
        }
    }
    
    void removeObject(GameObject * object) {
        if (!object) return;
        
        for (size_t i = 0; i < rootObjects.size(); ++i) {
            if (rootObjects[i] == object) {
                rootObjects.erase(rootObjects.begin() + i);
                break;
            }
        }
    }
    
    void updateAll(float deltaTime) {
        for (size_t i = 0; i < rootObjects.size(); ++i) {
            rootObjects[i]->updateSelfAndChild(deltaTime);
        }
    }
    
    void drawAll(GLuint shaderProgram) {
        for (size_t i = 0; i < rootObjects.size(); ++i) {
            rootObjects[i]->drawSelfAndChild(shaderProgram);
        }
    }
    
    void clear() {
        rootObjects.clear();
    }
    
    size_t size() const {
        return rootObjects.size();
    }
};

#endif
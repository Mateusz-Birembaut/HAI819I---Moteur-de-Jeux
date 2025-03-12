#ifndef SCENE_GRAPH_HPP
#define SCENE_GRAPH_HPP

#include "GameObject.hpp"
#include <vector>
#include <memory>
#include <algorithm>
#include <typeinfo>

class SceneGraph {
    
private:
    std::vector<void*> rootObjects;
    std::vector<void(*)(void*, GLuint)> drawFunctions;
    std::vector<void(*)(void*)> updateFunctions;
    std::vector<void(*)(void*)> deleteFunctions;
    
public:
    SceneGraph() {}
    
    ~SceneGraph() {
        clear();
    }
    
    template<typename MeshType>
    void addObject(GameObject<MeshType>* object) {
        if (!object) return;
        
        if (object->parent == nullptr) {
            rootObjects.push_back(static_cast<void*>(object));
            
            drawFunctions.push_back([](void* obj, GLuint shader) {
                GameObject<MeshType>* gameObj = static_cast<GameObject<MeshType>*>(obj);
                gameObj->draw(shader);
            });
            
            updateFunctions.push_back([](void* obj) {
                GameObject<MeshType>* gameObj = static_cast<GameObject<MeshType>*>(obj);
                gameObj->updateSelfAndChild();
            });
            
            deleteFunctions.push_back([](void* obj) {
                GameObject<MeshType>* gameObj = static_cast<GameObject<MeshType>*>(obj);
                delete gameObj;
            });
        }
    }
    
    template<typename MeshType>
    void removeObject(GameObject<MeshType>* object) {
        if (!object) return;
        
        for (size_t i = 0; i < rootObjects.size(); ++i) {
            if (rootObjects[i] == static_cast<void*>(object)) {
                rootObjects.erase(rootObjects.begin() + i);
                drawFunctions.erase(drawFunctions.begin() + i);
                updateFunctions.erase(updateFunctions.begin() + i);
                deleteFunctions.erase(deleteFunctions.begin() + i);
                break;
            }
        }
    }
    
    void updateAll() {
        for (size_t i = 0; i < rootObjects.size(); ++i) {
            updateFunctions[i](rootObjects[i]);
        }
    }
    
    void drawAll(GLuint shaderProgram) {
        for (size_t i = 0; i < rootObjects.size(); ++i) {
            drawFunctions[i](rootObjects[i], shaderProgram);
        }
    }
    
    void clear() {
        for (size_t i = 0; i < rootObjects.size(); ++i) {
            deleteFunctions[i](rootObjects[i]);
        }
        rootObjects.clear();
        drawFunctions.clear();
        updateFunctions.clear();
        deleteFunctions.clear();
    }
    
    size_t size() const {
        return rootObjects.size();
    }
};

#endif
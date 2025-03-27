#ifndef SCENE_GRAPH_HPP
#define SCENE_GRAPH_HPP

#include "GameObject.hpp"


class SceneGraph {
private:
    SceneGraph();
    std::vector<GameObject*> rootObjects;
    int nbRenderedGameObjects = 0;

    ~SceneGraph() {
        for (auto* obj : rootObjects) {
            delete obj; 
        }
    }

public:
    static SceneGraph& getInstance();

    int getNbRenderedGameObjects();
    bool addObject(GameObject* object);
    void updateAll(float deltaTime);
    void drawAll(GLuint shaderProgram);
    void clear();
    void getAllObjects(std::vector<GameObject*>& objects);
    void handleInputs(float deltaTime, GLFWwindow *window);
    size_t size() const;
    GameObject* getObjectAt(size_t index);

};

#endif

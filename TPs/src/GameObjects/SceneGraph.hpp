#ifndef SCENE_GRAPH_HPP
#define SCENE_GRAPH_HPP

#include "GameObject.hpp"

class SceneGraph {
private:
    SceneGraph();
    std::vector<GameObject*> rootObjects;
    int nbRenderedGameObjects = 0;

public:
    static SceneGraph& getInstance();

    int getNbRenderedGameObjects();
    void addObject(GameObject* object);
    void updateAll(float deltaTime);
    void drawAll(GLuint shaderProgram);
    void clear();
    void getAllObjects(std::vector<GameObject*>& objects);
    size_t size() const;
    GameObject* getObjectAt(size_t index);
};

#endif

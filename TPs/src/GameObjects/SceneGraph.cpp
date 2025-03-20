#include "SceneGraph.hpp"


SceneGraph::SceneGraph() {}

SceneGraph& SceneGraph::getInstance() {
    static SceneGraph instance;
    return instance;
}

int SceneGraph::getNbRenderedGameObjects(){
    return nbRenderedGameObjects;
}

void SceneGraph::addObject(GameObject* object) {
    if (!object) return;

    if (object->parent == nullptr) {
        rootObjects.push_back(object);
    }
}

void SceneGraph::updateAll(float deltaTime) {
    for (size_t i = 0; i < rootObjects.size(); ++i) {
        rootObjects[i]->updateSelfAndChild(deltaTime);
    }
}

void SceneGraph::drawAll(GLuint shaderProgram) {
    nbRenderedGameObjects=0;
    for (size_t i = 0; i < rootObjects.size(); ++i) {
        rootObjects[i]->drawSelfAndChild(shaderProgram, nbRenderedGameObjects);
    }
}

void SceneGraph::clear() {
    rootObjects.clear();
}

void SceneGraph::getAllObjects(std::vector<GameObject*>& objects) {
    for (size_t i = 0; i < rootObjects.size(); ++i) {
        rootObjects[i]->getSelfAndChild(objects);
    }
}

size_t SceneGraph::size() const {
    return rootObjects.size();
}

GameObject* SceneGraph::getObjectAt(size_t index) {
    if (index < rootObjects.size()) {
        return rootObjects[index];
    }
    return nullptr;
}

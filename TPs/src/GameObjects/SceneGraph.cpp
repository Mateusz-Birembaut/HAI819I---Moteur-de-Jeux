#include "SceneGraph.hpp"
#include "SceneGraphOctree.hpp"
#include "../Ressources/Globals.hpp"

SceneGraph::SceneGraph() {}

SceneGraph& SceneGraph::getInstance() {
    static SceneGraph instance;
    return instance;
}

int SceneGraph::getNbRenderedGameObjects(){
    return nbRenderedGameObjects;
}

//add object to SceneGraph and Octree
bool SceneGraph::addObject(GameObject* object) {
    if (!object) return false;

    if (object->parent == nullptr) {
        SceneGraphOctree & sceneGraphOctree = SceneGraphOctree::getInstance();
        object->cullingAABB.updateWorldMinMax(object->transformation.getLocalModelMatrix(0.1f));
        rootObjects.push_back(object);
        if(!sceneGraphOctree.addGameObject(sceneGraphOctree.getRoot(), object)){
            std::cout << "Couldn't add to octree" << std::endl;
            return false;
        };
    }
    return true;
}

void SceneGraph::updateAll(float deltaTime) {
    for (size_t i = 0; i < rootObjects.size(); i++){
        if (rootObjects[i]->transformation.needsUpdate()){
            rootObjects[i]->updateSelfAndChild(deltaTime);
        }
    }
}

void SceneGraph::drawAll(GLuint shaderProgram) {
    nbRenderedGameObjects = 0;
    if (spacePartitionCulling) {
        std::unordered_set<GameObject*> inFrustum = SceneGraphOctree::getInstance().getToBeDrawnGOs();
        for(auto & go : inFrustum){
            go->drawSelfAndChild(shaderProgram, nbRenderedGameObjects);
        }
    } else {
        for (size_t i = 0; i < rootObjects.size(); ++i) {
            rootObjects[i]->drawSelfAndChild(shaderProgram, nbRenderedGameObjects);
        }
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

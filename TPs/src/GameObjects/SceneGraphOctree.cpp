#include <GL/glew.h>
#include "SceneGraphOctree.hpp"
#include "../UI/ImGuiConsole.hpp"
#include "../Rendering/Camera.hpp"


SceneGraphOctree& SceneGraphOctree::getInstance(){
    static SceneGraphOctree sceneGraphOctree;
    return sceneGraphOctree;

}

Octant * SceneGraphOctree::getRoot(){
    return root;
}

bool SceneGraphOctree::addGameObject(Octant * octant, GameObject * gameObject, int depth){
    if(!octant->isLeaf){
        for (int i = 0; i < 8; ++i) {
            float overlap;
            for (size_t i = 0; i < 8; i++){
                overlap = octant->children[i]->aabb.overlapValue(&gameObject->cullingAABB);
                if ( overlap > 0 ){
                    return addGameObject(octant->children[i], gameObject, depth+1);
                }
            }
        }
    }
    
    if (octant->gameObjects.size() + 1 < nbOfGameObjectsPerOctant || depth == depthMax){
        octant->gameObjects.push_back(gameObject);
        return true;
    } else {
        divide(octant); 
        float overlap;
        for (size_t i = 0; i < 8; i++){
            overlap = octant->children[i]->aabb.overlapValue(&gameObject->cullingAABB);
            if ( overlap > 0 ){
                return addGameObject(octant->children[i], gameObject, depth+1);
            }
        }
    }
    return false;
}

bool SceneGraphOctree::divide(Octant * octant){
    octant->isLeaf = false;
    glm::vec3 center = (octant->aabb.worldMax + octant->aabb.worldMin) * 0.5f;
    //std::cout << "Centre BB : " << center.x << "," << center.y<< ","  << center.z << std::endl;
    //std::cout << "Enfants : " << std::endl;
    for (int i = 0; i < 8; ++i) {
        glm::vec3 child_min = octant->aabb.worldMin;
        glm::vec3 child_max = octant->aabb.worldMax;

        if (i & 1)
            child_min[0] = center[0];
        else
            child_max[0] = center[0];

        if (i & 2)
            child_min[1] = center[1];
        else
            child_max[1] = center[1];

        if (i & 4)
            child_min[2] = center[2];
        else
            child_max[2] = center[2];

        octant->children[i] = new Octant(child_min, child_max);
        octant->children[i]->id = (octant->id << 3) | i; // child id  = 3 bits of parents + 3 bits of self
        
    }
    //int compteur = 0;
    for (int j = 0; j < octant->gameObjects.size(); ++j) {
        GameObject* go = octant->gameObjects[j];
        //std::cout << go->gameObjectId << std::endl;
/*         for (int i = 0; i < 8; ++i) {
           
            if (octant->children[i]->contains(go->cullingAABB)) {
                octant->children[i]->gameObjects.push_back(go);
                //compteur ++;
                break;
            } 
            std::cout << "pas contenu dans aucuns enfants" << std::endl;
        }  */


        float overlap;
        for (size_t i = 0; i < 8; i++){
            overlap = octant->children[i]->aabb.overlapValue(&go->cullingAABB);
            if ( overlap > 0 ){
                octant->children[i]->gameObjects.push_back(go);
            }
        }
    }

    //std::cout << compteur << std::endl;
    octant->gameObjects.clear(); // delete children, no longer a leaf
    return true;
}

std::unordered_set<GameObject*>  SceneGraphOctree::getToBeDrawnGOs(){
    Camera & camera = Camera::getInstance();
    std::unordered_set<GameObject*>  toBeDrawn;
    if(root->isLeaf){
        //std::cout << root->isLeaf << std::endl;
        if (camera.isInCameraView(&root->aabb)) {
            for (size_t i = 0; i < root->gameObjects.size(); i++){
                toBeDrawn.insert(root->gameObjects[i]);
            }
        }
    }else {
        for (size_t i = 0; i < 8; i++){
            if (camera.isInCameraView(&root->children[i]->aabb)) {
                getToBeDrawnGOsRecusrive(root->children[i], toBeDrawn);
            }
        }
    }
    return toBeDrawn;
}

void SceneGraphOctree::getToBeDrawnGOsRecusrive(Octant* octant, std::unordered_set<GameObject*> & toBeDrawn){
    Camera & camera = Camera::getInstance();
    
    if(octant->isLeaf){
        if (camera.isInCameraView(&octant->aabb)) {
            for (size_t i = 0; i < octant->gameObjects.size(); i++){
                toBeDrawn.insert(octant->gameObjects[i]);
            }
        }
    }else {
        for (size_t i = 0; i < 8; i++){
            if (camera.isInCameraView(&octant->children[i]->aabb)) {
                getToBeDrawnGOsRecusrive(octant->children[i], toBeDrawn);
            }
        }
    }
}

std::unordered_set<GameObject*>  SceneGraphOctree::getToBeUpdatedGOs(){
    Camera & camera = Camera::getInstance();
    std::unordered_set<GameObject*>  toBeUpdated;
    if(root->isLeaf){
        //std::cout << root->isLeaf << std::endl;
        if (camera.isInCameraView(&root->aabb)) {
            for (size_t i = 0; i < root->gameObjects.size(); i++){
                toBeUpdated.insert(root->gameObjects[i]);
            }
        }
    }else {
        for (size_t i = 0; i < 8; i++){
            if (camera.isInCameraView(&root->children[i]->aabb)) {
                getToBeUpdatedGOsRecusrive(root->children[i], toBeUpdated);
            }
        }
    }
    return toBeUpdated;
}

void SceneGraphOctree::getToBeUpdatedGOsRecusrive(Octant* octant, std::unordered_set<GameObject*> & toBeUpdated){
    Camera & camera = Camera::getInstance();
    
    if(octant->isLeaf){
        if (camera.isInCameraView(&octant->aabb)) {
            for (size_t i = 0; i < octant->gameObjects.size(); i++){
                if (octant->gameObjects[i]->transformation.needsUpdate()){ // adds only the "static" objects, the other will be added to this even if not in view
                    toBeUpdated.insert(octant->gameObjects[i]);
                }
            }
        }
    }else {
        for (size_t i = 0; i < 8; i++){
            if (camera.isInCameraView(&octant->children[i]->aabb)) {
                getToBeUpdatedGOsRecusrive(octant->children[i], toBeUpdated);
            }
        }
    }
}


int SceneGraphOctree::getObjectCount() const {
    if (!root) return 0;
    return countObjectsRecursive(root);
}

int SceneGraphOctree::countObjectsRecursive(Octant* octant) const {
    if (!octant) return 0;
    
    int count = octant->gameObjects.size();
    
    if (!octant->isLeaf) {
        for (int i = 0; i < 8; ++i) {
            count += countObjectsRecursive(octant->children[i]);
        }
    }
    
    return count;
}
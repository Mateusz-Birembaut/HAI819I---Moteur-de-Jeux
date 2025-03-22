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
    // Non-leaf case: delegate to appropriate child
    if(!octant->isLeaf){
        for (int i = 0; i < 8; ++i) {
            float overlap;
            for (size_t i = 0; i < 8; i++){
                overlap = octant->children[i]->aabb.overlapValue(&gameObject->collider->aabb);
                if ( overlap > 0 ){
                    return addGameObject(octant->children[i], gameObject, depth+1);
                }
            }

            if (octant->children[i]->contains(gameObject->collider->aabb)) {

            }
        }
        // If no child contains it, add to parent
        octant->gameObjects.push_back(gameObject);
        return true;
    }
    
    // Leaf case: add directly or divide if needed
    if (octant->gameObjects.size() + 1 < nbOfGameObjectsPerOctant || depth == depthMax){
        octant->gameObjects.push_back(gameObject);
        return true;
    } else {
        divide(octant);  // Don't exit!
        float overlap;
        for (size_t i = 0; i < 8; i++){
            overlap = octant->children[i]->aabb.overlapValue(&gameObject->collider->aabb);
            if ( overlap > 0 ){
                return addGameObject(octant->children[i], gameObject, depth+1);
            }
        }
        // Fallback if no child contains it
        octant->gameObjects.push_back(gameObject);
        return true;
    }
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
        octant->children[i]->id = (octant->id << 3) | i; // id enfant = 3 bits du parent + 3 bits de l'enfant ()
        if (octant->children[i]->isLeaf == false)
        {
            std::cout << "AAAAAAAAAAAAAAAAAa" << std::endl;
        }
        
    }
    //int compteur = 0;
    for (int j = 0; j < octant->gameObjects.size(); ++j) {
        GameObject* go = octant->gameObjects[j];
        if (go->collider == nullptr){
            Console::getInstance().addLog("Erreur ajout d'un gameObject sans collider");
            return false;
        }
        //std::cout << go->gameObjectId << std::endl;
/*         for (int i = 0; i < 8; ++i) {
           
            if (octant->children[i]->contains(go->collider->aabb)) {
                octant->children[i]->gameObjects.push_back(go);
                //compteur ++;
                break;
            } 
            std::cout << "pas contenu dans aucuns enfants" << std::endl;
        }  */


        float overlap;
        for (size_t i = 0; i < 8; i++){
            overlap = octant->children[i]->aabb.overlapValue(&go->collider->aabb);
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

void SceneGraphOctree::drawOctant(Octant& octant) const {
    glBegin(GL_LINES);

    glVertex3f(octant.aabb.worldMin[0], octant.aabb.worldMin[1], octant.aabb.worldMin[2]);
    glVertex3f(octant.aabb.worldMax[0], octant.aabb.worldMin[1], octant.aabb.worldMin[2]);

    glVertex3f(octant.aabb.worldMin[0], octant.aabb.worldMin[1], octant.aabb.worldMin[2]);
    glVertex3f(octant.aabb.worldMin[0], octant.aabb.worldMax[1], octant.aabb.worldMin[2]);

    glVertex3f(octant.aabb.worldMin[0], octant.aabb.worldMin[1], octant.aabb.worldMin[2]);
    glVertex3f(octant.aabb.worldMin[0], octant.aabb.worldMin[1], octant.aabb.worldMax[2]);

    glVertex3f(octant.aabb.worldMax[0], octant.aabb.worldMax[1], octant.aabb.worldMax[2]);
    glVertex3f(octant.aabb.worldMin[0], octant.aabb.worldMax[1], octant.aabb.worldMax[2]);

    glVertex3f(octant.aabb.worldMax[0], octant.aabb.worldMax[1], octant.aabb.worldMax[2]);
    glVertex3f(octant.aabb.worldMax[0], octant.aabb.worldMin[1], octant.aabb.worldMax[2]);

    glVertex3f(octant.aabb.worldMax[0], octant.aabb.worldMax[1], octant.aabb.worldMax[2]);
    glVertex3f(octant.aabb.worldMax[0], octant.aabb.worldMax[1], octant.aabb.worldMin[2]);

    glVertex3f(octant.aabb.worldMin[0], octant.aabb.worldMax[1], octant.aabb.worldMin[2]);
    glVertex3f(octant.aabb.worldMax[0], octant.aabb.worldMax[1], octant.aabb.worldMin[2]);

    glVertex3f(octant.aabb.worldMin[0], octant.aabb.worldMax[1], octant.aabb.worldMin[2]);
    glVertex3f(octant.aabb.worldMin[0], octant.aabb.worldMax[1], octant.aabb.worldMax[2]);

    glVertex3f(octant.aabb.worldMax[0], octant.aabb.worldMin[1], octant.aabb.worldMin[2]);
    glVertex3f(octant.aabb.worldMax[0], octant.aabb.worldMax[1], octant.aabb.worldMin[2]);

    glVertex3f(octant.aabb.worldMax[0], octant.aabb.worldMin[1], octant.aabb.worldMin[2]);
    glVertex3f(octant.aabb.worldMax[0], octant.aabb.worldMin[1], octant.aabb.worldMax[2]);

    glVertex3f(octant.aabb.worldMin[0], octant.aabb.worldMin[1], octant.aabb.worldMax[2]);
    glVertex3f(octant.aabb.worldMax[0], octant.aabb.worldMin[1], octant.aabb.worldMax[2]);

    glVertex3f(octant.aabb.worldMin[0], octant.aabb.worldMin[1], octant.aabb.worldMax[2]);
    glVertex3f(octant.aabb.worldMin[0], octant.aabb.worldMax[1], octant.aabb.worldMax[2]);

    glEnd();

    if (!octant.isLeaf) {
        for (int i = 0; i < 8; ++i) {
            drawOctant(*octant.children[i]);
        }
    }
}

void SceneGraphOctree::draw() const {
    // Save current program
    GLint currentProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    
    // Disable shader program for immediate mode rendering
    glUseProgram(0);
    
    // Set a bright color for the octree lines
    glColor3f(0.0f, 1.0f, 0.0f);  // Bright green
    
    // Optional: Disable depth test to make octree always visible
    GLboolean depthTest = glIsEnabled(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    
    // Draw the octree
    drawOctant(*root);
    
    // Restore OpenGL state
    if (depthTest) glEnable(GL_DEPTH_TEST);
    glUseProgram(currentProgram);
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
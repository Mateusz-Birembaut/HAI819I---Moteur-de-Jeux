#ifndef SCENE_GRAPH_OCTREE_HPP
#define SCENE_GRAPH_OCTREE_HPP

#include "GameObject.hpp"
#include "../GameObjects/Components/Physics/AABB.hpp"
#include <unordered_set>


struct Octant {
    std::vector<GameObject*> gameObjects; // gameObjects dans cet octant
    AABB aabb;

    Octant *children[8]; // adresse des fils
    bool isLeaf = true; // est-ce une feuille

    uint64_t id; // id unique de l'octant

    bool contains(const AABB & otherAABB) { // 
        return aabb.contains(otherAABB);
    }

    Octant(const glm::vec3 & min, const glm::vec3 & max){
        this->gameObjects = std::vector<GameObject*>();
        this->aabb.worldMin = min;
        this->aabb.worldMax = max;
        this->isLeaf = true;
        for(int i = 0; i < 8; i++) {
            children[i] = nullptr;
        }
    } 

    static void printOctantInfo(Octant* octant) {
        std::cout << "Octant ID: " << octant->id << std::endl;
        std::cout << "Is leaf: " << (octant->isLeaf ? "true" : "false") << std::endl;
        std::cout << "Game objects: " << octant->gameObjects.size() << std::endl;
        std::cout << "AABB min: (" 
                << octant->aabb.worldMin.x << ", " 
                << octant->aabb.worldMin.y << ", " 
                << octant->aabb.worldMin.z << ")" << std::endl;
        std::cout << "AABB max: (" 
                << octant->aabb.worldMax.x << ", " 
                << octant->aabb.worldMax.y << ", " 
                << octant->aabb.worldMax.z << ")" << std::endl;
        std::cout << "Size: (" 
                << (octant->aabb.worldMax.x - octant->aabb.worldMin.x) << ", "
                << (octant->aabb.worldMax.y - octant->aabb.worldMin.y) << ", "
                << (octant->aabb.worldMax.z - octant->aabb.worldMin.z) << ")" << std::endl;
    }
    
};


class SceneGraphOctree {
private:
    SceneGraphOctree(){
        root = new Octant(glm::vec3(-55.f), glm::vec3(55.f));
        root->id=1;
    };

    Octant* root;

    int nbRenderedGameObjects = 0;

    int nbOfGameObjectsPerOctant = 20;
    int depthMax = 14;

    int countObjectsRecursive(Octant* octant) const;

    static int bestChildOverlap(Octant *octant, AABB *aabb);

    void getToBeDrawnGOsRecusrive(Octant* octant, std::unordered_set<GameObject*> & toBeDrawn);

public:
    static SceneGraphOctree& getInstance();

    bool divide(Octant * octant);
    bool addGameObject(Octant * octant, GameObject* gameObject, int depth =0);

    std::unordered_set<GameObject*>  getToBeDrawnGOs();

    void drawOctant(Octant &octant) const;
    void draw() const;

    Octant * getRoot();
    
    int getObjectCount() const;
    
    //int getOctantIndex(GameObject* gameObject) maybe
    //int getOctantIndexRecursive(Octant &octant, GameObject* gameObject);
};

#endif
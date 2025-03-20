#ifndef RESSOURCES_MANAGER_HPP
#define RESSOURCES_MANAGER_HPP

#include <GL/glew.h>

#include "../GameObjects/Components/Texture.hpp"
#include "../GameObjects/Components/Mesh.hpp"
#include "../GameObjects/Components/Physics/Collider.hpp"
#include "../GameObjects/Components/Physics/RigidBody.hpp"
#include "../Utils/Sphere.hpp"
#include "../Utils/Terrain.hpp"
#include <map>
#include <string>
#include <vector>


class RessourceManager {
public:
    static RessourceManager& getInstance();

    enum class MeshType {
        SPHERE,
        TERRAIN,
        MESH
    };

    Texture* addTexture(std::string textureId, const char* path, bool flipVertically);
    Texture* getTexture(std::string textureId);
    std::vector<const char*> getTextureNames();

    Mesh* addMesh(std::string meshId);
    Mesh* getMesh(std::string meshId);
    std::vector<const char*> getMeshesNames();

    Collider* addCollider(std::string gameObjectId);
    bool removeCollider(std::string gameObjectId);

    RigidBody* addRigidBody(std::string gameObjectId);

private:
    RessourceManager(){};

    std::map<std::string, Texture> textures;
    std::map<std::string, Mesh> meshes;
    std::map<std::string, Collider> colliders;
    std::map<std::string, RigidBody> rigidbodys;
};

#endif // RESSOURCES_MANAGER_HPP

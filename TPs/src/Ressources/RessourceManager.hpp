

#ifndef RESSOURCES_MANAGER_HPP
#define RESSOURCES_MANAGER_HPP

#include "../GameObjects/Components/Texture.hpp"
#include "../GameObjects/Components/Mesh.hpp"
#include "../GameObjects/Components/Physics/Collider.hpp"
#include "../GameObjects/Components/Physics/RigidBody.hpp"
#include "../Utils/Sphere.hpp" 
#include "../Utils/Terrain.hpp" // terrain
#include <map>
#include <string>

class RessourceManager{

    public:

        static RessourceManager& getInstance(){
            static RessourceManager instance;
            return instance;
        } 

        enum class MeshType {
            SPHERE,
            TERRAIN,
            MESH
        };


        Texture* addTexture(std::string textureId, const char* path, bool flipVertically){
            auto it = textures.find(textureId);
            if (it != textures.end()){
                return &it->second;
            }else {
                Texture texture(path, flipVertically);
                bool testLoad = texture.load(true);
                if (!testLoad){
                    return nullptr;
                }
                textures.insert(std::pair<std::string, Texture>(textureId, texture));
                Texture* tex = &textures.find(textureId)->second;
                tex->loadTexture(GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
                return tex;
            }    
        }

        Texture* getTexture(std::string textureId){
            auto it = textures.find(textureId);
            if (it != textures.end()){
                return &it->second;
            }else {
                return nullptr;
            }    
        }

        std::vector<const char*> getTextureNames(){
            std::vector<const char*> textureNames;
            for (auto const& x : textures){
                textureNames.push_back(x.first.c_str());
            }
            return textureNames;
        }

        Mesh* addMesh(std::string meshId){
            auto it = meshes.find(meshId);
            if (it != meshes.end()){
                return &it->second;
            }else {
                Mesh mesh;
                meshes.insert(std::pair<std::string, Mesh>(meshId, mesh));
                return &meshes.find(meshId)->second;
            }
        }

        Mesh* getMesh(std::string meshId){
            auto it = meshes.find(meshId);
            if (it != meshes.end()){
                return &it->second;
            }else {
                return nullptr;
            }
        }

        std::vector<const char*> getMeshesNames(){
            std::vector<const char*> meshNames;
            for (auto const& x : meshes){
                meshNames.push_back(x.first.c_str());
            }
            return meshNames;
        }

        Collider* addCollider(std::string gameObjectId){
            Collider collider;
            colliders.insert(std::pair<std::string, Collider>(gameObjectId, collider));
            return &colliders.find(gameObjectId)->second;
        }

        bool removeCollider(std::string gameObjectId){
            auto it = colliders.find(gameObjectId);
            if (it != colliders.end()){
                colliders.erase(it);
                return true;
            }
            return false;
        }

        RigidBody* addRigidBody(std::string gameObjectId){
            RigidBody rigidBody;
            rigidbodys.insert(std::pair<std::string, RigidBody>(gameObjectId, rigidBody));
            return &rigidbodys.find(gameObjectId)->second;
        }


    private:

        RessourceManager(){}

        std::map<std::string, Texture> textures;
        std::map<std::string, Mesh> meshes;
        std::map<std::string, Collider> colliders;
        std::map<std::string, RigidBody> rigidbodys;

};


#endif

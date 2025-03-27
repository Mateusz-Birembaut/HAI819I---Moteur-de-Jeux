#include "RessourceManager.hpp"

RessourceManager& RessourceManager::getInstance() {
    static RessourceManager instance;
    return instance;
}

Texture* RessourceManager::addTexture(std::string textureId, const char* path, bool flipVertically) {
    auto it = textures.find(textureId);
    if (it != textures.end()) {
        return &it->second;
    } else {
        Texture texture(path, flipVertically);
        bool testLoad = texture.load(true);
        if (!testLoad) {
            return nullptr;
        }
        textures.insert(std::pair<std::string, Texture>(textureId, texture));
        Texture* tex = &textures.find(textureId)->second;
        tex->loadTexture(GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
        return tex;
    }
}

Texture* RessourceManager::getTexture(std::string textureId) {
    auto it = textures.find(textureId);
    if (it != textures.end()) {
        return &it->second;
    } else {
        return nullptr;
    }
}

std::vector<const char*> RessourceManager::getTextureNames() {
    std::vector<const char*> textureNames;
    for (auto const& x : textures) {
        textureNames.push_back(x.first.c_str());
    }
    return textureNames;
}

Texture* RessourceManager::addHeightmap(std::string heightmapId, const char* path, bool flipVertically) {
    auto it = heightmaps.find(heightmapId);
    if (it != heightmaps.end()) {
        return &it->second;
    } else {
        Texture heightmap(path, flipVertically);
        bool testLoad = heightmap.load(true);
        if (!testLoad) {
            std::cout<< "texture not loaded returned null ptr";
            return nullptr;
        }
        heightmaps.insert(std::pair<std::string, Texture>(heightmapId, heightmap));
        Texture* tex = &heightmaps.find(heightmapId)->second;
        tex->loadTexture(GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
        return tex;
    }
}

Texture* RessourceManager::getHeightmap(std::string heightmapId) {
    auto it = heightmaps.find(heightmapId);
    if (it != heightmaps.end()) {
        return &it->second;
    } else {
        return nullptr;
    }
}

std::vector<const char*> RessourceManager::getHeightmapNames() {
    std::vector<const char*> heightmapNames;
    for (auto const& x : heightmaps) {
        heightmapNames.push_back(x.first.c_str());
    }
    return heightmapNames;
}


Mesh* RessourceManager::addMesh(std::string meshId) {
    auto it = meshes.find(meshId);
    if (it != meshes.end()) {
        return &it->second;
    } else {
        Mesh mesh;
        meshes.insert(std::pair<std::string, Mesh>(meshId, mesh));
        return &meshes.find(meshId)->second;
    }
}

Mesh* RessourceManager::getMesh(std::string meshId) {
    auto it = meshes.find(meshId);
    if (it != meshes.end()) {
        return &it->second;
    } else {
        return nullptr;
    }
}

std::vector<const char*> RessourceManager::getMeshesNames() {
    std::vector<const char*> meshNames;
    for (auto const& x : meshes) {
        meshNames.push_back(x.first.c_str());
    }
    return meshNames;
}

Collider* RessourceManager::addCollider(std::string gameObjectId) {
    Collider collider;
    colliders.insert(std::pair<std::string, Collider>(gameObjectId, collider));
    return &colliders.find(gameObjectId)->second;
}

bool RessourceManager::removeCollider(std::string gameObjectId) {
    auto it = colliders.find(gameObjectId);
    if (it != colliders.end()) {
        colliders.erase(it);
        return true;
    }
    return false;
}

RigidBody* RessourceManager::addRigidBody(std::string gameObjectId) {
    RigidBody rigidBody;
    rigidbodys.insert(std::pair<std::string, RigidBody>(gameObjectId, rigidBody));
    return &rigidbodys.find(gameObjectId)->second;
}

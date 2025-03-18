#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "../GameObjects/SceneGraph.hpp"
#include "../GameObjects/GameObject.hpp"

#include <vector>
#include <string>


// 3 boolean checkboxes in a single line
bool Checkbox3(const char* label, bool* x, bool* y, bool* z) {
    bool changed = false;
    ImGui::PushID(label);
    
    ImGui::Text("%s", label);
    ImGui::SameLine();
    
    // Length of each checkbox
    float width = ImGui::CalcItemWidth() / 3 - 10;
    
    // X
    ImGui::PushItemWidth(width);
    ImGui::PushID("X");
    ImGui::AlignTextToFramePadding();
    ImGui::Text("X");
    ImGui::SameLine();
    changed |= ImGui::Checkbox("##X", x);
    ImGui::PopID();
    ImGui::SameLine();
    
    // Y
    ImGui::PushID("Y");
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Y");
    ImGui::SameLine();
    changed |= ImGui::Checkbox("##Y", y);
    ImGui::PopID();
    ImGui::SameLine();
    
    // Z
    ImGui::PushID("Z");
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Z");
    ImGui::SameLine();
    changed |= ImGui::Checkbox("##Z", z);
    ImGui::PopID();
    
    ImGui::PopItemWidth();
    ImGui::PopID();
    return changed;
}

// Fonction récursive pour afficher un objet et ses enfants
void displayGameObject(GameObject* obj, int id) {
    // Créer un ID unique pour cet objet (pour éviter les collisions d'ID d'ImGui)
    std::string objName = "Objet " + std::to_string(id);
    
    // Créer un nœud d'arbre
    if (ImGui::TreeNode(objName.c_str())) {
        // Afficher les propriétés de l'objet
        ImGui::Text("Position: %.2f, %.2f, %.2f", 
            obj->transformation.translation.x,
            obj->transformation.translation.y,
            obj->transformation.translation.z);
        
        ImGui::Text("Scale: %.2f, %.2f, %.2f", 
            obj->transformation.scale.x,
            obj->transformation.scale.y,
            obj->transformation.scale.z);
        
        ImGui::Text("Rotation Speed: %.2f", obj->transformation.rotationSpeed);
        
        // Permettre l'édition de certaines propriétés
        ImGui::SliderFloat3("Position", &obj->transformation.translation.x, -10.0f, 10.0f);
        ImGui::SliderFloat("Rotation Speed", &obj->transformation.rotationSpeed, 0.0f, 20.0f);
        
        // Texture info
        ImGui::Text("Has Texture: %s", (obj->texture != nullptr) ? "Yes" : "No");
        
        // Afficher les enfants récursivement
        if (!obj->children.empty()) {
            ImGui::Text("Children: %zu", obj->children.size());
            
            for (size_t i = 0; i < obj->children.size(); i++) {
                displayGameObject(obj->children[i], id * 100 + i + 1);
            }
        }
        
        ImGui::TreePop();
    }
}

// Fonction auxiliaire pour afficher les objets dans la hiérarchie
void displayObjectInHierarchy(GameObject* obj, GameObject** selectedObject) {
    std::string objName = "Objet " + obj->gameObjectId;
    
    if (ImGui::TreeNodeEx(objName.c_str(), 
                        ImGuiTreeNodeFlags_OpenOnArrow | 
                        (*selectedObject == obj ? ImGuiTreeNodeFlags_Selected : 0)))
    {
        if (ImGui::IsItemClicked()) {
            *selectedObject = obj;
        }
        
        // Afficher les enfants récursivement
        for (size_t i = 0; i < obj->children.size(); i++) {
            displayObjectInHierarchy(obj->children[i], selectedObject);
        }
        
        ImGui::TreePop();
    }
    else if (ImGui::IsItemClicked()) {
        *selectedObject = obj;
    }
}


void gameObjectTransformMenu(GameObject * selectedObject){
    ImGui::Text("Transform");
    ImGui::SameLine();
    if (ImGui::Button("Reset")) {
        selectedObject->transformation.resetTransform();
    }
    ImGui::DragFloat3("Position", &selectedObject->transformation.translation.x, 0.1f);
    ImGui::DragFloat3("Rotation", &selectedObject->transformation.eulerRot.x, 0.1f);
    Checkbox3("Rotate continuously", &selectedObject->transformation.continuouslyRotate.x,&selectedObject->transformation.continuouslyRotate.y, &selectedObject->transformation.continuouslyRotate.z);
    ImGui::DragFloat3("Scale", &selectedObject->transformation.scale.x, 0.1f);
    ImGui::SliderFloat("Rotation Speed", &selectedObject->transformation.rotationSpeed, -20.0f, 20.0f);
}


void gameObjectMeshMenu(GameObject * selectedObject){
    ImGui::Text("Mesh");
    ImGui::SameLine();
    if (selectedObject->mesh == nullptr){
        if (ImGui::Button("Add Mesh")) {
            // TODO : ajouter fenetre pour choisir le type du nouveau mesh avec son id et paramètres et le creer ou un utilsier mesh existant
            Console::getInstance().addLog("Ajout du mesh à l'object");
            Mesh* newMesh = RessourceManager::getInstance().addMesh("sphere");
            selectedObject->mesh = newMesh;
        }
    }else {
        //ImGui::Text("Mesh: %s", selectedObject->mesh->getName());
        if (ImGui::Button("Remove Mesh")) {
            Console::getInstance().addLog("Suppression du lien entre l'objet et le mesh");
            selectedObject->mesh = nullptr;
        }
    }
}


void gameObjectTextureMenu(GameObject * selectedObject){
    bool hasTexture = (selectedObject->texture != nullptr);
    if (hasTexture){
        ImGui::Text("Texture : %s", selectedObject->texture->getName());
        if (ImGui::Button("Remove Texture")) {
            selectedObject->texture = nullptr;
            Console::getInstance().addLog("Suppression du lien entre l'objet et la texture");
        }
    }else {
        ImGui::Text("No Texture");
        ImGui::SameLine();
        if (ImGui::Button("Add Texture")) {
            // TODO : ajouter fenetre pour choisir path de la texture
            // message si essaie d'ajouter une texture alors qu'il y en a déjà une qui porte le même nom et que le path est différent
            Console::getInstance().addLog("Ajout de la texture");
            Texture* newTexture = RessourceManager::getInstance().addTexture("earth", "azeaze", true);
            selectedObject->texture = newTexture;
        }
    }
}


void gameObjectColliderMenu(GameObject * selectedObject){
    bool hasCollider = selectedObject->collider != nullptr;
    if (hasCollider){
        ImGui::Text("Collider :");
        ImGui::DragFloat3("min", &selectedObject->collider->aabb.min.x, 0.1f);
        ImGui::DragFloat3("max", &selectedObject->collider->aabb.max.x, 0.1f);

        if (ImGui::Button("Remove Collider")) {
            selectedObject->collider = nullptr;
            RessourceManager::getInstance().removeCollider(selectedObject->gameObjectId);
            Console::getInstance().addLog("Suppression du collider");
        }
    }else {
        ImGui::Text("No Collider");
        ImGui::SameLine();
        if (ImGui::Button("Add Collider")) {
            Collider* newCollider = RessourceManager::getInstance().addCollider(generateId());
            selectedObject->collider = newCollider;
            Console::getInstance().addLog("Ajout d'un collider");
            if (selectedObject->mesh != nullptr){
                Console::getInstance().addLog("AABB généré autour du mesh");
                newCollider->aabb.fitToMesh(selectedObject->mesh);
            }
            
        }
    }
}

void gameObjectRigidBodyMenu(GameObject * selectedObject){
    ImGui::Text("Mesh");
    ImGui::SameLine();
    if (selectedObject->mesh == nullptr){
        if (ImGui::Button("Add Mesh")) {
            // TODO : ajouter fenetre pour choisir le type du nouveau mesh avec son id et paramètres et le creer ou un utilsier mesh existant
            Console::getInstance().addLog("Ajout du mesh à l'object");
            Mesh* newMesh = RessourceManager::getInstance().addMesh("sphere");
            selectedObject->mesh = newMesh;
        }
    }else {
        //ImGui::Text("Mesh: %s", selectedObject->mesh->getName());
        if (ImGui::Button("Remove Mesh")) {
            Console::getInstance().addLog("Suppression du lien entre l'objet et le mesh");
            selectedObject->mesh = nullptr;
        }
    }
}
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "../SceneGraph.hpp"
#include "../GameObject.hpp"

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
    std::string objName = "Objet " + std::to_string(id) + " (ID: " + std::to_string(obj->objectID) + ")";
    
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
void displayObjectInHierarchy(GameObject* obj, int id, GameObject** selectedObject) {
    std::string objName = "Objet " + std::to_string(id) + " (ID: " + std::to_string(obj->objectID) + ")";
    
    if (ImGui::TreeNodeEx(objName.c_str(), 
                        ImGuiTreeNodeFlags_OpenOnArrow | 
                        (*selectedObject == obj ? ImGuiTreeNodeFlags_Selected : 0)))
    {
        if (ImGui::IsItemClicked()) {
            *selectedObject = obj;
        }
        
        // Afficher les enfants récursivement
        for (size_t i = 0; i < obj->children.size(); i++) {
            displayObjectInHierarchy(obj->children[i], id * 100 + i + 1, selectedObject);
        }
        
        ImGui::TreePop();
    }
    else if (ImGui::IsItemClicked()) {
        *selectedObject = obj;
    }
}
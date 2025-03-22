#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuiFileDialog.h>

#include "../GameObjects/SceneGraph.hpp"
#include "../GameObjects/GameObject.hpp"
#include "../Ressources/IdGenerator.hpp"

#include <common/objloader.hpp>
#include <vector>
#include <string>


bool showAddTextureModal = false;
bool isAddingNewTexture = false;

bool showAddMeshModal = false;
bool isAddingNewMesh = false;

const std::vector<const char*> meshTypes = { "Sphere", "Terrain", "Custom File" };



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
    ImGui::Checkbox("Is static (ex : decor, won't be updated if not in view)", &selectedObject->transformation.isStatic);
    ImGui::DragFloat3("Scale", &selectedObject->transformation.scale.x, 0.1f);
    ImGui::DragFloat3("Position", &selectedObject->transformation.translation.x, 0.1f);
    ImGui::DragFloat3("Rotation", &selectedObject->transformation.eulerRot.x, 0.1f);
    ImGui::SliderFloat("Rotation Speed", &selectedObject->transformation.rotationSpeed, -100.0f, 100.0f);
    Checkbox3("Rotate continuously", &selectedObject->transformation.continuouslyRotate.x,&selectedObject->transformation.continuouslyRotate.y, &selectedObject->transformation.continuouslyRotate.z);
}


void gameObjectMeshMenu(GameObject * selectedObject){
    ImGui::Text("Mesh");
    ImGui::SameLine();
    if (selectedObject->mesh != nullptr){
        ImGui::Text("Number of vertices: %zu", selectedObject->mesh->getVertices().size());
        if (ImGui::Button("Remove Mesh")) {
            Console::getInstance().addLog("Suppression du lien entre l'objet et le mesh");
            selectedObject->mesh = nullptr;
        }
    }else {
        ImGui::Text("No Mesh");
        ImGui::SameLine();
        if (ImGui::Button("Add Mesh")) {
            showAddMeshModal = true;
            ImGui::OpenPopup("Add Mesh");
        }

        if (ImGui::BeginPopupModal("Add Mesh", &showAddMeshModal, ImGuiWindowFlags_AlwaysAutoResize)){
            if (!isAddingNewMesh) {
                ImGui::Text("Select existing Mesh to add");
                ImGui::Separator();

                std::vector<const char*> existingMeshesNames = RessourceManager::getInstance().getMeshesNames();
                static int selectedMesh = 0;
                if (existingMeshesNames.size() != 0){
                    ImGui::ListBox("Existing Meshes", &selectedMesh, existingMeshesNames.data(), existingMeshesNames.size());
                }

                if (ImGui::Button("Valider")){
                    Mesh* newMesh = RessourceManager::getInstance().getMesh(existingMeshesNames[selectedMesh]);
                    if (newMesh == nullptr){
                        Console::getInstance().addLog("Mesh not found");
                        showAddMeshModal = false;
                        
                    }else {
                        selectedObject->mesh = newMesh;
                        Console::getInstance().addLog("Ajout du mesh");
                        showAddMeshModal = false;
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancel")){
                    showAddMeshModal = false;
                }
                ImGui::SameLine();
                if (ImGui::Button("Add a new mesh")){
                    isAddingNewMesh = true;
                }
            }else {
                ImGui::Text("Creating a new mesh");
                ImGui::Separator();

                //select mesh type
                static int selectedMeshType = 0;
                ImGui::ListBox("Existing Meshes", &selectedMeshType, meshTypes.data(), meshTypes.size());

                static char meshName[256] = "";
                static char meshPath[256] = "";

                if(selectedMeshType == 0){
                    static int sphereResolution[2] = { 10, 10 };
                    ImGui::InputText("Mesh Name ", meshName, IM_ARRAYSIZE(meshName));
                    ImGui::InputInt2("Resolution", (int*)&sphereResolution);

                    if(ImGui::Button("Valider")){
                        Mesh* newMesh = RessourceManager::getInstance().addMesh(meshName);
                        if (newMesh == nullptr){
                            Console::getInstance().addLog("Cound not create mesh");
                        }else {
                            Sphere::create(*newMesh, sphereResolution[0], sphereResolution[1]);
                            selectedObject->mesh = newMesh;
                            Console::getInstance().addLog("Ajout du mesh");
                            showAddMeshModal = false;
                            isAddingNewMesh = false;
                            meshName[0] = '\0';
                        }
                    }
                }

                if(selectedMeshType == 1){

                }

                if (selectedMeshType == 2){
        
                    ImVec2 minSize = ImVec2(700, 500);  // Largeur, hauteur minimales
                    ImVec2 maxSize = ImVec2(1200, 800); // Largeur, hauteur maximales (optionnel)             

                    ImGui::InputText("Mesh Name ", meshName, IM_ARRAYSIZE(meshName));
                    ImGui::InputText("Mesh Path ", meshPath, IM_ARRAYSIZE(meshPath));
                    ImGui::SameLine();

                    if (ImGui::Button("Browse...")) {
                        ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose Mesh File", 
                            ".off,.obj,.ply", IGFD::FileDialogConfig());
                    }

                    
                    if(ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey", ImGuiWindowFlags_NoCollapse, minSize, maxSize)){
                        if (ImGuiFileDialog::Instance()->IsOk()){
                            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                            strcpy(meshPath, filePathName.c_str());

                            if (meshName[0] == '\0') {
                                std::string fileName = ImGuiFileDialog::Instance()->GetCurrentFileName();
                                strcpy(meshName, fileName.c_str());
                            }
                        }
                        ImGuiFileDialog::Instance()->Close();
                    }
        
                    if(ImGui::Button("Valider") && meshName[0] != '\0' && meshPath[0] != '\0'){
                        Mesh newMesh;
                        if (strstr(meshPath, ".off") != NULL){
                            if(!loadOFF(meshPath, newMesh)){
                                Console::getInstance().addLog("Mesh not found");
                                return;
                            }
                        }else if (strstr(meshPath, ".obj") != NULL){
                            if (!loadOBJ(meshPath, newMesh)){
                                Console::getInstance().addLog("Mesh not found");
                                return;
                            }
                            
                        }else {
                            Console::getInstance().addLog("Mesh format not supported");
                            return;
                        }
                        if (newMesh.getVertices().size() <= 0){
                            Console::getInstance().addLog("Could not load mesh");
                            return;
                        }
                        
                        Mesh* nMesh = RessourceManager::getInstance().addMesh(meshName);
                        //RessourceManager::getInstance().addMesh(meshName);
                        if (nMesh == nullptr){
                            Console::getInstance().addLog("Mesh not found");
                            return;
                        }else {
                            nMesh->setVertices(newMesh.getVertices());
                            nMesh->setIndexes(newMesh.getIndexes());
                            nMesh->createBuffers();
                            selectedObject->mesh = nMesh;
                            Console::getInstance().addLog("Ajout du mesh");
                            showAddMeshModal = false;
                            isAddingNewMesh = false;
                            meshName[0] = '\0';
                            meshPath[0] = '\0';
                        }
                    } 
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancel")){
                    isAddingNewMesh = false;
                    meshName[0] = '\0';
                    meshPath[0] = '\0';
                }           
            }

            ImGui::EndPopup();
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
            showAddTextureModal = true;
            ImGui::OpenPopup("Add Texture");
        }

        if (ImGui::BeginPopupModal("Add Texture", &showAddTextureModal, ImGuiWindowFlags_AlwaysAutoResize)){
            if (!isAddingNewTexture) {
                ImGui::Text("Select existing Texture to apply");
                ImGui::Separator();

                std::vector<const char*> existingTexturesNames = RessourceManager::getInstance().getTextureNames();
                static int selectedTexture = 0;
                if (existingTexturesNames.size() != 0){
                    ImGui::ListBox("Existing Textures", &selectedTexture, existingTexturesNames.data(), existingTexturesNames.size());
                }

                if (ImGui::Button("Valider")){
                    Texture* newTexture = RessourceManager::getInstance().getTexture(existingTexturesNames[selectedTexture]);
                    if (newTexture == nullptr){
                        Console::getInstance().addLog("Texture not found");
                        showAddTextureModal = false;
                        
                    }else {
                        selectedObject->texture = newTexture;
                        Console::getInstance().addLog("Ajout de la texture");
                        showAddTextureModal = false;
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancel")){
                    showAddTextureModal = false;
                }
                ImGui::SameLine();
                if (ImGui::Button("Add a new texture")){
                    isAddingNewTexture = true;

 
                }
            }else {
                ImGui::Text("Creating a new texture");
                ImGui::Separator();
                static char textureName[256] = "";
                static char texturePath[256] = "";
                static bool flipHorizontal = false;
    
                ImVec2 minSize = ImVec2(700, 500);  // Largeur, hauteur minimales
                ImVec2 maxSize = ImVec2(1200, 800); // Largeur, hauteur maximales (optionnel)             

                ImGui::InputText("Texture Name ", textureName, IM_ARRAYSIZE(textureName));
                ImGui::InputText("Texture Path ", texturePath, IM_ARRAYSIZE(texturePath));
                ImGui::SameLine();

                if (ImGui::Button("Browse...")) {
                    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose Texture File", 
                        ".png,.jpg,.jpeg,.bmp,.tga", IGFD::FileDialogConfig());
                }

                
                if(ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey", ImGuiWindowFlags_NoCollapse, minSize, maxSize)){
                    if (ImGuiFileDialog::Instance()->IsOk()){
                        std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                        strcpy(texturePath, filePathName.c_str());

                        if (textureName[0] == '\0') {
                            std::string fileName = ImGuiFileDialog::Instance()->GetCurrentFileName();
                            strcpy(textureName, fileName.c_str());
                        }
                    }
                    ImGuiFileDialog::Instance()->Close();
                }

                ImGui::Checkbox("Flip Horizontal", &flipHorizontal);
    
                if(ImGui::Button("Valider") && textureName[0] != '\0' && texturePath[0] != '\0'){
                    Texture* newTexture = RessourceManager::getInstance().addTexture(textureName, texturePath, flipHorizontal);
                    if (newTexture == nullptr){
                        Console::getInstance().addLog("Texture not found");
                    }else {
                        selectedObject->texture = newTexture;
                        Console::getInstance().addLog("Ajout de la texture");
                        showAddTextureModal = false;
                        isAddingNewTexture = false;
                        textureName[0] = '\0';
                        texturePath[0] = '\0';
                        flipHorizontal = false;
                    }
                } 
                ImGui::SameLine();
                if (ImGui::Button("Cancel")){
                    isAddingNewTexture = false;
                    textureName[0] = '\0';
                    texturePath[0] = '\0';
                    flipHorizontal = false;
                }           
            }

            ImGui::EndPopup();
        }
        
    }
}


void gameObjectColliderMenu(GameObject * selectedObject){
    bool hasCollider = selectedObject->collider != nullptr;
    if (hasCollider){
        ImGui::Text("Collider :");
        ImGui::SameLine();
        if (selectedObject->collider->showCollider ?  ImGui::Button("Hide Collider") : ImGui::Button("Display Collider") ){
            selectedObject->collider->showCollider = !selectedObject->collider->showCollider;
        }
        ImGui::DragFloat3("world min", &selectedObject->collider->aabb.worldMin.x, 0.1f);
        ImGui::DragFloat3("world max", &selectedObject->collider->aabb.worldMax.x, 0.1f);
        ImGui::DragFloat3("min", &selectedObject->collider->aabb.min.x, 0.1f);
        ImGui::DragFloat3("max", &selectedObject->collider->aabb.max.x, 0.1f);

        if (ImGui::Button("Fit Collider to mesh")) {
            if(selectedObject->mesh != nullptr){
                selectedObject->collider->aabb.fitToMesh(selectedObject->mesh);
            }
            Console::getInstance().addLog("Collider resized around mesh");
        }
        ImGui::SameLine();
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

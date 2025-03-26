# HAI819I---Moteur-de-Jeux

## Installation

### Cloner le dépôt principal
git clone https://github.com/votre-nom/HAI819I---Moteur-de-Jeux.git
cd HAI819I---Moteur-de-Jeux

#### Initialiser les sous-modules
git submodule init
git submodule update

#### Si les sous-modules ne sont pas encore configurés, les ajouter avec:
git submodule add https://github.com/ocornut/imgui.git TPs/external/imgui
git submodule add https://github.com/aiekick/ImGuiFileDialog.git TPs/external/ImGuiFileDialog

#### Passer à la branche docking pour ImGui
cd TPs/external/imgui
git checkout docking
cd ../../..


#### Créer et aller dans le répertoire de build
cd TPs
mkdir build
cd build

#### Générer les fichiers de build avec CMake
cmake ..

#### Compiler le projet
make

#### Exécuter le programme
./TP
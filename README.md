# HAI819I---Moteur-de-Jeux

## Installation

### Cloner le dépôt principal
```bash
git clone https://github.com/votre-nom/HAI819I---Moteur-de-Jeux.git
cd HAI819I---Moteur-de-Jeux
```

#### Initialiser les sous-modules
```bash
git submodule init
git submodule update
```
#### Si les sous-modules ne sont pas encore configurés, les ajouter avec:
```bash
git submodule add https://github.com/ocornut/imgui.git TPs/external/imgui
git submodule add https://github.com/aiekick/ImGuiFileDialog.git TPs/external/ImGuiFileDialog
```
#### Passer à la branche docking pour ImGui
```bash
cd TPs/external/imgui
git checkout docking
cd ../../..
```

#### Créer et aller dans le répertoire de build
```bash
cd TPs
mkdir build
cd build
```
#### Générer les fichiers de build avec CMake
```bash
cmake ..
```
#### Compiler le projet
```bash
make -j
```
#### Exécuter le programme
```bash
./TP
```
#chmod +x /home/mat/HAI819I---Moteur-de-Jeux/TPs/start.sh
#!/bin/bash

# Supprimer le répertoire build s'il existe
rm -rf build

# Créer un nouveau répertoire build
mkdir build

# Changer le répertoire courant pour build
cd build

# Configurer le projet avec CMake
cmake ..

# Compiler le projet en utilisant plusieurs threads
make -j

# Exécuter l'exécutable TP
./TP

cd ..
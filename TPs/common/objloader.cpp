#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <cstring>
#include <fstream>

#include <glm/glm.hpp>

#include "Conversion.hpp"
#include "objloader.hpp"

#include "../src/Ressources/Structs.hpp"
#include "../src/GameObjects/Components/Mesh.hpp"


// Very, VERY simple OBJ loader.
// Here is a short list of features a real function would provide : 
// - Binary files. Reading a model should be just a few memcpy's away, not parsing a file at runtime. In short : OBJ is not very great.
// - Animations & bones (includes bones weights)
// - Multiple UVs
// - All attributes should be optional, not "forced"
// - More stable. Change a line in the OBJ file and it crashes.
// - More secure. Change another line and you can inject code.
// - Loading from memory, stream, etc





bool loadOBJ(const std::string & path, Mesh & mesh) {

    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;

    FILE * file = fopen(path.c_str(), "r");
    if(file == NULL) {
        printf("Impossible to open the file! Check the path.\n");
        return false;
    }

    while(1) {
        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF) break;

        if (strcmp(lineHeader, "v") == 0) {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);
        } else if (strcmp(lineHeader, "vt") == 0) {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            uv.y = -uv.y; // Invert V coordinate for DDS textures
            temp_uvs.push_back(uv);
        } else if (strcmp(lineHeader, "vn") == 0) {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_normals.push_back(normal);
        } else if (strcmp(lineHeader, "f") == 0) {
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
                &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
                
            if (matches != 9) {
                printf("File can't be read by our simple parser\n");
                fclose(file);
                return false;
            }
            
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        } else {
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }
    }

    std::vector<Vertex> vertices;
    std::vector<unsigned short> indices;
    
    // Pour chaque vertex de chaque triangle
    for(unsigned int i = 0; i < vertexIndices.size(); i++) {
        unsigned int vertexIndex = vertexIndices[i];
        unsigned int uvIndex = uvIndices[i];
        unsigned int normalIndex = normalIndices[i];
        
        // Obtenir les attributs
        glm::vec3 position = temp_vertices[vertexIndex-1];
        glm::vec2 uv = temp_uvs[uvIndex-1];
        glm::vec3 normal = temp_normals[normalIndex-1];
        
        // Créer le Vertex et l'ajouter au Mesh
        Vertex vertex;
        vertex.position = position;
        normalToUnsignedShort(normal, vertex.pitchYaw[0], vertex.pitchYaw[1]);
        vertex.uv = uv;
        
        vertices.push_back(vertex);
        indices.push_back(i);
    }
    mesh.setIndexes(indices);
    mesh.setVertices(vertices);

    
    fclose(file);
    return true;
}

bool loadOFF(const std::string & filename, Mesh & mesh) {
    std::vector<glm::vec3> vertices;
    std::vector<unsigned short> indices;
    std::vector<std::vector<unsigned short>> triangles;
    
    std::ifstream myfile;
    myfile.open(filename.c_str());
    if (!myfile.is_open()) {
        std::cout << filename << " cannot be opened" << std::endl;
        return false;
    }

    std::string magic_s;
    myfile >> magic_s;
    if(magic_s != "OFF") {
        std::cout << magic_s << " != OFF : We handle ONLY *.off files." << std::endl;
        myfile.close();
        return false;
    }

    int n_vertices, n_faces, dummy_int;
    myfile >> n_vertices >> n_faces >> dummy_int;

    vertices.resize(n_vertices);
    
    // Charger les sommets
    for(int v = 0; v < n_vertices; ++v) {
        myfile >> vertices[v].x >> vertices[v].y >> vertices[v].z;
        if(std::isnan(vertices[v].x)) vertices[v].x = 0.0;
        if(std::isnan(vertices[v].y)) vertices[v].y = 0.0;
        if(std::isnan(vertices[v].z)) vertices[v].z = 0.0;
    }

    // Charger les faces
    for(int f = 0; f < n_faces; ++f) {
        int n_vertices_on_face;
        myfile >> n_vertices_on_face;

        if(n_vertices_on_face == 3) {
            unsigned short v1, v2, v3;
            myfile >> v1 >> v2 >> v3;
            indices.push_back(v1);
            indices.push_back(v2);
            indices.push_back(v3);
        }
        else if(n_vertices_on_face > 3) {
            // Gérer les polygones avec plus de 3 sommets
            std::vector<unsigned short> vhandles(n_vertices_on_face);
            for(int i = 0; i < n_vertices_on_face; ++i)
                myfile >> vhandles[i];
            
            // Convertir en triangles (triangulation en éventail)
            for(unsigned short i = 0; i < vhandles.size() - 2; ++i) {
                indices.push_back(vhandles[0]);
                indices.push_back(vhandles[i + 1]);
                indices.push_back(vhandles[i + 2]);
            }
        }
        else {
            std::cout << "OFFIO::open error: Face number " << f 
                      << " has " << n_vertices_on_face << " vertices" << std::endl;
            myfile.close();
            return false;
        }
    }
    
    myfile.close();
    
    std::vector<Vertex> vertices_mesh;
    // Remplir le Mesh avec les vertex
    vertices_mesh.clear();
    
    // Calcul des normales pour chaque face
    std::vector<glm::vec3> normals(n_vertices, glm::vec3(0.0f));
    
    // Parcourir toutes les faces pour calculer les normales
    for(size_t i = 0; i < indices.size(); i += 3) {
        unsigned short i1 = indices[i];
        unsigned short i2 = indices[i + 1];
        unsigned short i3 = indices[i + 2];
        
        glm::vec3 v1 = vertices[i1];
        glm::vec3 v2 = vertices[i2];
        glm::vec3 v3 = vertices[i3];
        
        // Calculer la normale de la face
        glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));
        
        // Accumuler les normales pour chaque sommet
        normals[i1] += normal;
        normals[i2] += normal;
        normals[i3] += normal;
    }
    
    // Normaliser toutes les normales
    for(auto& n : normals) {
        if(glm::length(n) > 0.0f) {
            n = glm::normalize(n);
        }
    }
    
    // Ajouter les sommets au maillage en utilisant les indices
    for(size_t i = 0; i < indices.size(); ++i) {
        unsigned short idx = indices[i];
        
        Vertex vertex;
        vertex.position = vertices[idx];
        normalToUnsignedShort(normals[idx], vertex.pitchYaw[0], vertex.pitchYaw[1]);
        vertex.uv = glm::vec2(0.0f, 0.0f);          // UV par défaut
        
        vertices_mesh.push_back(vertex);
    }
    
    // Create sequential indices (0, 1, 2, ...) since vertices are already in the right order
    std::vector<unsigned short> sequential_indices(vertices_mesh.size());
    for(size_t i = 0; i < vertices_mesh.size(); i++) {
        sequential_indices[i] = i;
    }
    
    mesh.setVertices(vertices_mesh);
    mesh.setIndexes(sequential_indices);
    return true;
}

bool loadOFF( const std::string & filename ,
              std::vector< glm::vec3 > & vertices ,
              std::vector< unsigned short > & indices,
              std::vector< std::vector<unsigned short > > & triangles )
{
    bool convertToTriangles = true;
    bool randomize = false;

    std::ifstream myfile;
    myfile.open(filename.c_str());
    if (!myfile.is_open())
    {
        std::cout << filename << " cannot be opened" << std::endl;
        return false;
    }

    std::string magic_s;

    myfile >> magic_s;

    if( magic_s != "OFF" )
    {
        std::cout << magic_s << " != OFF :   We handle ONLY *.off files." << std::endl;
        myfile.close();
        return false;
    }

    int n_vertices , n_faces , dummy_int;
    myfile >> n_vertices >> n_faces >> dummy_int;

    vertices.resize(n_vertices);

    for( int v = 0 ; v < n_vertices ; ++v )
    {
        glm::vec3 vertex;
        myfile >> vertex.x >> vertex.y >> vertex.z;
        if( std::isnan(vertex.x) )
            vertex.x = 0.0;
        if( std::isnan(vertex.y) )
            vertex.y = 0.0;
        if( std::isnan(vertex.z) )
            vertex.z = 0.0;
        vertices[v] = vertex;
    }


    for( int f = 0 ; f < n_faces ; ++f )
    {
        int n_vertices_on_face;
        myfile >> n_vertices_on_face;
        if( n_vertices_on_face == 3 )
        {
            unsigned short _v1 , _v2 , _v3;
            std::vector< unsigned short > _v;
            myfile >> _v1 >> _v2 >> _v3;
            _v.push_back( _v1 );
            _v.push_back( _v2 );
            _v.push_back( _v3 );
            triangles.push_back( _v );
            indices.push_back( _v1 );
            indices.push_back( _v2 );
            indices.push_back( _v3 );

        }
        else if( n_vertices_on_face > 3 )
        {
            std::vector< unsigned short > vhandles;
            vhandles.resize(n_vertices_on_face);
            for( int i=0 ; i < n_vertices_on_face ; ++i )
                myfile >> vhandles[i];

            if( convertToTriangles )
            {
                unsigned short k=(randomize)?(rand()%vhandles.size()):0;
                for (unsigned short i=0;i<vhandles.size()-2;++i)
                {
                    std::vector< unsigned short > tri(3);
                    tri[0]=vhandles[(k+0)%vhandles.size()];
                    tri[1]=vhandles[(k+i+1)%vhandles.size()];
                    tri[2]=vhandles[(k+i+2)%vhandles.size()];
                    triangles.push_back(tri);
                    indices.push_back(tri[0]);
                    indices.push_back(tri[1]);
                    indices.push_back(tri[2]);
                }
            }
            else
            {
                //careful not triangles
                triangles.push_back(vhandles);
                for( int i=0 ; i < vhandles.size() ; ++i )
                    indices.push_back(vhandles[i]);
            }
        }
        else
        {
            std::cout << "OFFIO::open error : Face number " << f << " has " << n_vertices_on_face << " vertices" << std::endl;
            myfile.close();
            return false;
        }
    }

    myfile.close();
    return true;
}


bool loadOFF( const std::string & filename ,
              std::vector< glm::vec3 > & vertices,
              std::vector< unsigned short > & faces)
{
    bool convertToTriangles = true;
    bool randomize = false;

    std::ifstream myfile;
    myfile.open(filename.c_str());
    if (!myfile.is_open())
    {
        std::cout << filename << " cannot be opened" << std::endl;
        return false;
    }

    std::string magic_s;

    myfile >> magic_s;

    if( magic_s != "OFF" )
    {
        std::cout << magic_s << " != OFF :   We handle ONLY *.off files." << std::endl;
        myfile.close();
        return false;
    }

    int n_vertices , n_faces , dummy_int;
    myfile >> n_vertices >> n_faces >> dummy_int;

    vertices.resize(n_vertices);

    for( int v = 0 ; v < n_vertices ; ++v )
    {
        glm::vec3 vertex;
        myfile >> vertex.x >> vertex.y >> vertex.z;
        if( std::isnan(vertex.x) )
            vertex.x = 0.0;
        if( std::isnan(vertex.y) )
            vertex.y = 0.0;
        if( std::isnan(vertex.z) )
            vertex.z = 0.0;
        vertices[v] = vertex;
    }


    for( int f = 0 ; f < n_faces ; ++f )
    {
        int n_vertices_on_face;
        myfile >> n_vertices_on_face;
        if( n_vertices_on_face == 3 )
        {
            unsigned short _v1 , _v2 , _v3;
            std::vector< unsigned short > _v;
            myfile >> _v1 >> _v2 >> _v3;
            //            _v.push_back( _v1 );
            //            _v.push_back( _v2 );
            //            _v.push_back( _v3 );
            //            faces.push_back( _v );
            faces.push_back( _v1 );
            faces.push_back( _v2 );
            faces.push_back( _v3 );

        }
        else if( n_vertices_on_face > 3 )
        {
            std::vector< unsigned short > vhandles;
            vhandles.resize(n_vertices_on_face);
            for( int i=0 ; i < n_vertices_on_face ; ++i )
                myfile >> vhandles[i];

            if( convertToTriangles )
            {
                unsigned short k=(randomize)?(rand()%vhandles.size()):0;
                for (unsigned short i=0;i<vhandles.size()-2;++i)
                {
                    std::vector< unsigned short > tri(3);
                    tri[0]=vhandles[(k+0)%vhandles.size()];
                    tri[1]=vhandles[(k+i+1)%vhandles.size()];
                    tri[2]=vhandles[(k+i+2)%vhandles.size()];
                    //faces.push_back(tri);
                    faces.push_back(tri[0]);
                    faces.push_back(tri[1]);
                    faces.push_back(tri[2]);
                }
            }
            else
            {
                //faces.push_back(vhandles);
                for( int i=0 ; i < vhandles.size() ; ++i )
                    faces.push_back(vhandles[i]);
            }
        }
        else
        {
            std::cout << "OFFIO::open error : Face number " << f << " has " << n_vertices_on_face << " vertices" << std::endl;
            myfile.close();
            return false;
        }
    }

    myfile.close();
    return true;
}



#ifdef USE_ASSIMP // don't use this #define, it's only for me (it AssImp fails to compile on your machine, at least all the other tutorials still work)

// Include AssImp
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

bool loadAssImp(
        const char * path,
        std::vector<unsigned short> & indices,
        std::vector<glm::vec3> & vertices,
        std::vector<glm::vec2> & uvs,
        std::vector<glm::vec3> & normals
        ){

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(path, 0/*aiProcess_JoinIdenticalVertices | aiProcess_SortByPType*/);
    if( !scene) {
        fprintf( stderr, importer.GetErrorString());
        getchar();
        return false;
    }
    const aiMesh* mesh = scene->mMeshes[0]; // In this simple example code we always use the 1rst mesh (in OBJ files there is often only one anyway)

    // Fill vertices positions
    vertices.reserve(mesh->mNumVertices);
    for(unsigned int i=0; i<mesh->mNumVertices; i++){
        aiVector3D pos = mesh->mVertices[i];
        vertices.push_back(glm::vec3(pos.x, pos.y, pos.z));
    }

    // Fill vertices texture coordinates
    uvs.reserve(mesh->mNumVertices);
    for(unsigned int i=0; i<mesh->mNumVertices; i++){
        aiVector3D UVW = mesh->mTextureCoords[0][i]; // Assume only 1 set of UV coords; AssImp supports 8 UV sets.
        uvs.push_back(glm::vec2(UVW.x, UVW.y));
    }

    // Fill vertices normals
    normals.reserve(mesh->mNumVertices);
    for(unsigned int i=0; i<mesh->mNumVertices; i++){
        aiVector3D n = mesh->mNormals[i];
        normals.push_back(glm::vec3(n.x, n.y, n.z));
    }


    // Fill face indices
    indices.reserve(3*mesh->mNumFaces);
    for (unsigned int i=0; i<mesh->mNumFaces; i++){
        // Assume the model has only triangles.
        indices.push_back(mesh->mFaces[i].mIndices[0]);
        indices.push_back(mesh->mFaces[i].mIndices[1]);
        indices.push_back(mesh->mFaces[i].mIndices[2]);
    }

    // The "scene" pointer will be deleted automatically by "importer"
    return true;
}

#endif

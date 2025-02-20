#ifndef TERRAIN_HPP
#define TERRAIN_HPP


struct Vertex{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 normal;
    glm::vec2 uv;
};

void setTesselatedSquare ( std::vector<unsigned short> & indices, 
    std::vector<std::vector<unsigned short> > & triangles,
    std::vector<Vertex> & indexed_vertices)  {
    indices.clear();
    triangles.clear();
    indexed_vertices.clear();

    int nX = 16;
    int nY = 16;

    for (int i = 0; i < nX; i++) {
        for (int j = 0; j < nY; j++) { 
            float t_x, t_z, x, y, z;

            t_x = (float(i) / float(nX - 1))*2;
            t_z = (float(j) / float(nY - 1))*2;
            
            x = t_x -1;
            z = t_z -1;

            y =  0.0f;//static_cast<float>(std::rand()) / RAND_MAX * 0.06f - 0.03f;
            
            glm::vec3 position = glm::vec3(x, y, z);

            Vertex v;
            v.position = position;
            //v.color = position;
            //v.normal = glm::vec3(0.0, 1.0, 0.0);
            v.uv = glm::vec2(float(i)/(float(nX-1)), float(j)/(float(nY-1)));
            //v.uv = glm::vec2(0.0,0.0);

            indexed_vertices.push_back(v);
        }
    }
    for (int i = 0; i < nX; i ++) {
        for (int j = 0; j < nY; j++) {
            if(i != nX-1 && j != nY-1){
                short unsigned int de_base = i*nX + j;
                short unsigned int voisin_droite = i*nX + j+1;
                short unsigned int voisin_haut = (i+1)*nX + j;
                short unsigned int voisin_haut_droite = (i+1)*nX + j+1;


                indices.push_back(de_base);
                indices.push_back(voisin_haut);
                indices.push_back(voisin_droite);
                triangles.push_back({de_base, voisin_haut, voisin_droite});


                indices.push_back(voisin_haut_droite);
                indices.push_back(voisin_droite);
                indices.push_back(voisin_haut);
                triangles.push_back({voisin_haut_droite, voisin_droite, voisin_haut});

            }
        }
    }
}

void setTerrainWithHeightMap ( std::vector<unsigned short> & indices, 
    std::vector<std::vector<unsigned short> > & triangles,
    std::vector<Vertex> & indexed_vertices, unsigned char *& data, int & width, int & height , int & nChannels)  {

    indices.clear();
    triangles.clear();
    indexed_vertices.clear();

    int nX = 32;
    int nY = 32;

    for (int i = 0; i < nX; i++) {
        for (int j = 0; j < nY; j++) { 
            float t_x, t_z, x, y, z;
            t_x = i;//(float(i) / float(nX - 1))*2;
            t_z = j;//(float(j) / float(nY - 1))*2;

            int t_i = std::min((int)((i / float(nY - 1)) * height), height - 1);
            int t_j = std::min((int)((j / float(nX - 1)) * width), width - 1);

            //int flipped_i = height - 1 - t_i;
            unsigned char* texel = data + (t_j * width + t_i) * nChannels;

            //unsigned char* texel = data + (t_i * width + t_j) * nChannels;


            //std::cout << (float)texel[0] << std::endl;
            
            float texelValueNormalized = texel[0]/255.0;
            
            x = t_x -1;
            z = t_z -1;

            y = texelValueNormalized;
            
            glm::vec3 position = glm::vec3(x, y, z);

            Vertex v;
            v.position = position;
            //v.color = position;
            //v.normal = glm::vec3(0.0, 1.0, 0.0);
            v.uv = glm::vec2(float(i)/(float(nX-1)), float(j)/(float(nY-1)));;

            indexed_vertices.push_back(v);
        }
    }
    for (int i = 0; i < nX; i ++) {
        for (int j = 0; j < nY; j++) {
            if(i != nX-1 && j != nY-1){
                short unsigned int de_base = i*nX + j;
                short unsigned int voisin_droite = i*nX + j+1;
                short unsigned int voisin_haut = (i+1)*nX + j;
                short unsigned int voisin_haut_droite = (i+1)*nX + j+1;


                indices.push_back(de_base);
                indices.push_back(voisin_haut);
                indices.push_back(voisin_droite);
                triangles.push_back({de_base, voisin_haut, voisin_droite});


                indices.push_back(voisin_haut_droite);
                indices.push_back(voisin_droite);
                indices.push_back(voisin_haut);
                triangles.push_back({voisin_haut_droite, voisin_droite, voisin_haut});

            }
        }
    }
}


#endif
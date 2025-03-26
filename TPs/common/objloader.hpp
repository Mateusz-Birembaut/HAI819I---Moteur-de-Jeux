#ifndef OBJLOADER_H
#define OBJLOADER_H
#include <string>

#include "../src/Ressources/Structs.hpp"
#include "../src/GameObjects/Components/Mesh.hpp"


bool loadOBJ(const std::string & filename ,Mesh & mesh);


bool loadAssImp(
	const char * path, 
	std::vector<unsigned short> & indices,
	std::vector<glm::vec3> & vertices,
	std::vector<glm::vec2> & uvs,
	std::vector<glm::vec3> & normals
);


bool loadOFF( const std::string & filename ,
              std::vector< glm::vec3 > & vertices ,
              std::vector< unsigned short > & faces) ;

bool loadOFF( const std::string & filename ,Mesh & mesh);
  


bool loadOFF( const std::string & filename ,
              std::vector< glm::vec3 > & vertices ,
              std::vector< unsigned short > & indices,
              std::vector< std::vector<unsigned short > > & triangles) ;



#endif

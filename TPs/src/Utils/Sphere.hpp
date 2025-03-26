#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <limits>

#include "../../common/Conversion.hpp"

#include "../Ressources/Structs.hpp"
#include "../GameObjects/Components/Mesh.hpp"

class Sphere {


    public:

        static void create(Mesh & mesh, int nTheta, int nPhi) {
            std::vector<Vertex> vertices;
            std::vector<unsigned short> indexes;

            for( unsigned int thetaIt = 0 ; thetaIt < nTheta ; ++thetaIt ) {
                float u = (float)(thetaIt) / (float)(nTheta-1);
                float theta = u * 2 * M_PI;
                for( unsigned int phiIt = 0 ; phiIt < nPhi ; ++phiIt ) {
                    unsigned int vertexIndex = thetaIt + phiIt * nTheta;
                    float v = (float)(phiIt) / (float)(nPhi-1);
                    float phi = - M_PI/2.0 + v * M_PI;

                    Vertex vertex;
                    vertex.position = SphericalCoordinatesToEuclidean( theta , phi );

                    vertex.position = SphericalCoordinatesToEuclidean(theta, phi);


                    vertex.pitchYaw[0] = static_cast<unsigned short>(65535.0f * v); 
                    vertex.pitchYaw[1] =   static_cast<unsigned short>(65535.0f * u); 
                    //std::cout <<" Pitch : " << vertex.pitchYaw[0] << " Yaw :  " << vertex.pitchYaw[1] << std::endl;
                    vertex.uv = {-u,-v}; // flip to be in the correct orientation
                    
                    vertices.push_back(vertex);

                }
            }

            for( unsigned int thetaIt = 0 ; thetaIt < nTheta - 1 ; ++thetaIt ) {
                for( unsigned int phiIt = 0 ; phiIt < nPhi - 1 ; ++phiIt ) {
                    unsigned int vertexuv = thetaIt + phiIt * nTheta;
                    unsigned int vertexUv = thetaIt + 1 + phiIt * nTheta;
                    unsigned int vertexuV = thetaIt + (phiIt+1) * nTheta;
                    unsigned int vertexUV = thetaIt + 1 + (phiIt+1) * nTheta;
                    indexes.push_back( vertexuv );
                    indexes.push_back( vertexUv );
                    indexes.push_back( vertexUV );
                    indexes.push_back( vertexuv );
                    indexes.push_back( vertexUV );
                    indexes.push_back( vertexuV );
                }
            }


            mesh.setVertices(vertices);
            mesh.setIndexes(indexes);
            mesh.createBuffers();
        }


};
    
/* 
void build_arrays(){
    unsigned int nTheta = 20 , nPhi = 20;
    positions_array.resize(3 * nTheta * nPhi );
    normalsArray.resize(3 * nTheta * nPhi );
    uvs_array.resize(2 * nTheta * nPhi );
    for( unsigned int thetaIt = 0 ; thetaIt < nTheta ; ++thetaIt ) {
        float u = (float)(thetaIt) / (float)(nTheta-1);
        float theta = u * 2 * M_PI;
        for( unsigned int phiIt = 0 ; phiIt < nPhi ; ++phiIt ) {
            unsigned int vertexIndex = thetaIt + phiIt * nTheta;
            float v = (float)(phiIt) / (float)(nPhi-1);
            float phi = - M_PI/2.0 + v * M_PI;
            Vec3 xyz = SphericalCoordinatesToEuclidean( theta , phi );
            positions_array[ 3 * vertexIndex + 0 ] = m_center[0] + m_radius * xyz[0];
            positions_array[ 3 * vertexIndex + 1 ] = m_center[1] + m_radius * xyz[1];
            positions_array[ 3 * vertexIndex + 2 ] = m_center[2] + m_radius * xyz[2];
            normalsArray[ 3 * vertexIndex + 0 ] = xyz[0];
            normalsArray[ 3 * vertexIndex + 1 ] = xyz[1];
            normalsArray[ 3 * vertexIndex + 2 ] = xyz[2];
            uvs_array[ 2 * vertexIndex + 0 ] = u;
            uvs_array[ 2 * vertexIndex + 1 ] = v;
        }
    }
    triangles_array.clear();
    for( unsigned int thetaIt = 0 ; thetaIt < nTheta - 1 ; ++thetaIt ) {
        for( unsigned int phiIt = 0 ; phiIt < nPhi - 1 ; ++phiIt ) {
            unsigned int vertexuv = thetaIt + phiIt * nTheta;
            unsigned int vertexUv = thetaIt + 1 + phiIt * nTheta;
            unsigned int vertexuV = thetaIt + (phiIt+1) * nTheta;
            unsigned int vertexUV = thetaIt + 1 + (phiIt+1) * nTheta;
            triangles_array.push_back( vertexuv );
            triangles_array.push_back( vertexUv );
            triangles_array.push_back( vertexUV );
            triangles_array.push_back( vertexuv );
            triangles_array.push_back( vertexUV );
            triangles_array.push_back( vertexuV );
        }
    }
}
 */


#endif
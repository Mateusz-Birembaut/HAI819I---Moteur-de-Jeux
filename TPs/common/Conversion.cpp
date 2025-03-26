#include "Conversion.hpp"

void EuclideanToSphericalCoordinates(const glm::vec3& normal, float& theta, float& phi) {
    glm::vec3 n = glm::normalize(normal);
    phi = asin(n.y);
    
    if (abs(n.y) > 0.9999f) {
        theta = 0.0f;
    } else {
        theta = atan2(n.z, n.x);
        
        if (theta < 0.0f) {
            theta += 2.0f * M_PI;
        }
    }
}

void normalToUnsignedShort(const glm::vec3& normal, unsigned short& pitchBits, unsigned short& yawBits) {
    float theta, phi;
    EuclideanToSphericalCoordinates(normal, theta, phi);
    
    float v = (phi + M_PI/2) / M_PI;

    float u = theta / (2.0f * M_PI);
    
    pitchBits = static_cast<unsigned short>(65535.0f * v);
    yawBits = static_cast<unsigned short>(65535.0f * u);
}

glm::vec3 SphericalCoordinatesToEuclidean( float theta , float phi ) {
    return glm::vec3( cos(theta) * cos(phi), sin(phi), sin(theta) * cos(phi));
}
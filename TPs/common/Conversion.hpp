#ifndef CONVERSION_HPP
#define CONVERSION_HPP

#include <glm/glm.hpp>
#include <cmath>

void EuclideanToSphericalCoordinates(const glm::vec3& normal, float& theta, float& phi);

void normalToUnsignedShort(const glm::vec3& normal, unsigned short& pitchBits, unsigned short& yawBits);

glm::vec3 SphericalCoordinatesToEuclidean( float theta , float phi );

#endif
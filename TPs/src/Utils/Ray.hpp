#ifndef RAY_HPP
#define RAY_HPP

#include <glm/vec3.hpp> // Include GLM header for glm::vec3

#include "../GameObjects/Components/Physics/AABB.hpp"

class GameObject;

struct Ray
{
    glm::vec3 origin;
    glm::vec3 direction;

    Ray(glm::vec3 origin, glm::vec3 direction) : origin(origin), direction(direction) {}

    static bool isParallelTo(const Ray *ray, glm::vec3 normal);

    static bool rayIntersectsAABB(const Ray &ray, AABB *aabb, float &distance);

    static bool rayTriangleIntersection(const Ray &ray, const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, float &_t);

    static bool rayIntersectsMesh(const Ray &ray, GameObject *go, float &outDistance, glm::vec3 &outPosition);
};



#endif
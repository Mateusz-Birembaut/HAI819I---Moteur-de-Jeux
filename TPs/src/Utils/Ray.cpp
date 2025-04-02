#include "Ray.hpp"

#include "../GameObjects/Components/Physics/AABB.hpp"
#include "../GameObjects/Components/Physics/Collider.hpp"
#include "../GameObjects/Components/Mesh.hpp"
#include "../GameObjects/GameObject.hpp"
#include "../Ressources/Structs.hpp"



bool Ray::isParallelTo(const Ray * ray, glm::vec3 normal ) {
    if (glm::dot(ray->direction, normal) >= 0.0001){ 
        return true;
    }
    return false;
}

bool Ray::rayIntersectsAABB(const Ray& ray, AABB * aabb,float& distance)
{
    float tMin = -std::numeric_limits<float>::max();
    float tMax = std::numeric_limits<float>::max();
    
    // for each axis, check if the ray intersects
    for (int i = 0; i < 3; i++) {
        // if the ray is parrallel and outside the box, there is no intersection
        if (std::abs(ray.direction[i]) < 1e-6) {
            if (ray.origin[i] < aabb->worldMin[i] || ray.origin[i] > aabb->worldMax[i])
                return false;
        } else {
            // compute the intersection of the ray with the planes of the box
            float ood = 1.0f / ray.direction[i];
            float t1 = (aabb->worldMin[i] - ray.origin[i]) * ood;
            float t2 = (aabb->worldMax[i] - ray.origin[i]) * ood;
            
            if (t1 > t2) std::swap(t1, t2);
            
            // Compute the intersection of slab intersection intervals
            tMin = std::max(tMin, t1);
            tMax = std::min(tMax, t2);
            
            // Exit with no collision if slab intersection is empty
            if (tMin > tMax) return false;
        }
    }
    
    // Return distance to intersection point
    distance = tMin > 0 ? tMin : tMax;
    return distance >= 0;  // Check if intersection is in front of the ray
}



bool Ray::rayTriangleIntersection( 
    const Ray& ray,
    const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2,
    float& _t
) {

    glm::vec3 e1 = v1 - v0;
    glm::vec3 e2 = v2 - v0;

    glm::vec3 normal = glm::normalize(glm::cross(e1, e2));

    // 1) check that the ray is not parallel to the triangle:
    if(isParallelTo( &ray, normal)){
        return false;
    }

    float m_D = -glm::dot(normal, v0);

    // 2) check that the triangle is "in front of" the ray:
    float t = - (glm::dot(normal, ray.origin) + m_D) / glm::dot(normal, ray.direction);
    if (t <= 0.001){
        // the ray is behind the triangle
        return false;
    }

    // 3) check that the intersection point is inside the triangle:
    // CONVENTION: compute u,v such that p = w0*c0 + w1*c1 + w2*c2, check that 0 <= w0,w1,w2 <= 1

    glm::vec3 intersection_point = ray.origin + t * ray.direction;
    glm::vec3 local_point = intersection_point - v0;

    float d00 = glm::dot(e1, e1);
    float d01 = glm::dot(e1, e2);
    float d11 = glm::dot(e2, e2);
    float d20 = glm::dot(local_point, e1);
    float d21 = glm::dot(local_point, e2);

    float denom = 1 / (d00 * d11 - d01 * d01);
    float v = (d11 * d20 - d01 * d21) * denom;
    float w = (d00 * d21 - d01 * d20) * denom;
    float u = 1.0f - v - w;

    // 4) Finally, if all conditions were met, then there is an intersection! :
    if (u >= 0 && v >= 0 && w >= 0) {
        _t = t;
        return true;
    }

    return false;
}


bool Ray::rayIntersectsMesh(const Ray& ray, GameObject* go, float& outDistance, glm::vec3& outPosition) {
    if (go->mesh == nullptr) {
        return false;
    }
    
    std::vector<Vertex> vertices = go->mesh->getVertices();
    const std::vector<unsigned short>& indices = go->mesh->getIndexes();
    
    bool foundIntersection = false;
    outDistance = std::numeric_limits<float>::max();
    
    //update vertices to world space
    for (size_t i = 0; i < vertices.size(); ++i) {
        glm::vec4 worldPos = go->transformation.modelMatrix * glm::vec4(vertices[i].position, 1.0f);
        vertices[i].position = glm::vec3(worldPos);
    }

    for (size_t i = 0; i < indices.size(); i += 3) {
        if (i + 2 >= indices.size()) {
            continue;
        }
        
        const glm::vec3& v0 = vertices[indices[i]].position;
        const glm::vec3& v1 = vertices[indices[i + 1]].position;
        const glm::vec3& v2 = vertices[indices[i + 2]].position;

        float t;
        
        if (Ray::rayTriangleIntersection(ray, v0, v1, v2, t)) {
            if (t < outDistance) {
                outDistance = t;
                outPosition = ray.origin + t * ray.direction;
                foundIntersection = true;
            }
        }
    }
    
    return foundIntersection;
}
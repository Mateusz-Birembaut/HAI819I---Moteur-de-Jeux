#include "Controller.hpp"
#include <GLFW/glfw3.h>

#include "../GameObject.hpp" // Inclure ici, pas dans le .hpp
#include "../SceneGraphOctree.hpp"
#include "../../Utils/Ray.hpp"

void Controller::rayCastDown(GameObject *go)
{
    float sizeY = std::abs(go->transformation.scale.y);
    glm::vec3 position = go->transformation.translation;
    SceneGraphOctree &sceneGraph = SceneGraphOctree::getInstance();
    std::unordered_set<GameObject *> objectsBeneath = sceneGraph.getObjectsBeneath(position);

    Ray ray = Ray(position, glm::vec3(0.0f, -1.0f, 0.0f));
    ray.direction = glm::vec3(go->transformation.getLocalModelMatrix(0.0f) * glm::vec4(ray.direction, 0.0f)); // gives the direction in world space

    float distanceMin = std::numeric_limits<float>::max();
    GameObject *closestObject = nullptr;
    float distanceCurrent = 0.0f;

    // first check if the ray intersects with the AABB of the objects possibly beneath
    for (auto &object : objectsBeneath)
    {
        if (Ray::rayIntersectsAABB(ray, &object->cullingAABB, distanceCurrent) && object != go)
        {
            if (distanceCurrent < distanceMin)
            {
                distanceMin = distanceCurrent;
                closestObject = object;
            }
        }
    }

    // if we found an object, we check if the ray intersects with the mesh to get the exact intersection point based on the mesh
    if (closestObject && closestObject->mesh)
    {
        glm::vec3 intersectionPoint;
        float meshDistance;

        if (Ray::rayIntersectsMesh(ray, closestObject, meshDistance, intersectionPoint))
        {
            glm::vec3 intersectionPointObjecytSpace = glm::vec3(glm::inverse(closestObject->transformation.modelMatrix) * glm::vec4(intersectionPoint, 1.0f));
            
            if (closestObject->heightmap != nullptr)
            {

                glm::vec3 localSpaceIntersectionPoint = glm::vec3(glm::inverse(closestObject->transformation.modelMatrix) * glm::vec4(intersectionPoint, 1.0f));
  
                float u = (localSpaceIntersectionPoint.x - closestObject->mesh->getMinBounds().x) / (closestObject->mesh->getMaxBounds().x - closestObject->mesh->getMinBounds().x);
                float v = (localSpaceIntersectionPoint.z - closestObject->mesh->getMinBounds().z) / (closestObject->mesh->getMaxBounds().z - closestObject->mesh->getMinBounds().z);

                intersectionPoint.y += closestObject->heightmap->sampleTexture(u, v);
            }

            float heightDifference = go->transformation.translation.y - intersectionPoint.y;

            if (heightDifference <= sizeY)
            {
                go->transformation.translation.y = intersectionPoint.y + sizeY;
            }
        }
    }
}

void Controller::handleInputs(float deltaTime, GLFWwindow *window, GameObject *gameObject)
{
    Transform *transform = &gameObject->transformation;
    float displacement = movementSpeed * deltaTime;
    bool moved = false;

    // Sauvegarde de la position originale pour restauration si nécessaire
    glm::vec3 originalPos = transform->translation;

    // Traiter les entrées de mouvement horizontal (XZ)
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        transform->translation[2] += displacement;
        moved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        transform->translation[2] -= displacement;
        moved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        transform->translation[0] -= displacement;
        moved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        transform->translation[0] += displacement;
        moved = true;
    }

    // Permettre le mouvement vertical manuel (optionnel)
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        transform->translation.y -= displacement;
        moved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        transform->translation.y += displacement;
        moved = true;
    }

    // Si l'objet s'est déplacé horizontalement, ajuster sa hauteur
    if (moved)
    {
        // Appliquer le ray casting pour adapter la hauteur
        rayCastDown(gameObject);
        transform->isDirty = true;
        gameObject->updateSelfAndChild(deltaTime);
    }
}
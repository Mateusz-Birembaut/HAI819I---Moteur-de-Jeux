#include "Collider.hpp"

Collider::Collider() {
    aabb = AABB();
}

void Collider::drawCollider(GLuint programID, const glm::mat4& modelMatrix) {
    aabb.draw(programID, modelMatrix);
}

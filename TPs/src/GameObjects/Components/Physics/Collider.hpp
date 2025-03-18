

#ifndef COLLIDER_HPP
#define COLLIDER_HPP

#include "AABB.hpp"

class Collider{
    public:

        AABB aabb;
        bool isTrigger = false;
        bool showCollider = false;

        Collider(){
            aabb = AABB();
        }

        void drawCollider(GLuint programID, const glm::mat4& modelMatrix){
            aabb.draw(programID, modelMatrix);
        }



};


#endif
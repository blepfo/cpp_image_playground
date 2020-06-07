#ifndef _INTERSECTABLES_H
#define _INTERSECTABLES_H

#include<glm/glm.hpp>

#include "../include/Raytracing.h"

namespace Intersectables {

class Sphere : public Raytracing::RayIntersectable {
    public:
        Raytracing::HitInfo intersect(const Raytracing::Ray& ray) override; 
        Sphere(float r, glm::vec3 c) : r(r), c(c) {}

    private:
        const float r;
        const glm::vec3 c;
}; 

} // namespace Intersectables

#endif

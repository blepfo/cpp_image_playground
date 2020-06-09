#pragma once

#include<glm/glm.hpp>

#include "SimpleGraphics/Material.hpp"

namespace SimpleGraphics {

/** 
 * Class represenging a ray intersectable object
 * Contains a materialId. For now, assume this corresponds to a
 * ::SimpleGraphics::Material stored in a ::SimpleGraphics::Scene
 */
class Intersectable : public Raytracing::RayIntersectable {
    public:
        int materialId;

        Intersectable(int materialId) 
            : materialId(materialId), Raytracing::RayIntersectable() {}

        Intersectable(int materialId, glm::mat4 transform) 
            : materialId(materialId), Raytracing::RayIntersectable(transform) {}
};



class Sphere : public ::SimpleGraphics::Intersectable {
    public:
        Raytracing::HitInfo intersect(const Raytracing::Ray& ray) override; 

        Sphere(float r, glm::vec3 c, int materialId) 
            : r(r), c(c), ::SimpleGraphics::Intersectable(materialId) {}

        Sphere(float r, glm::vec3 c, int materialId, glm::mat4 transform) 
            : r(r), c(c), ::SimpleGraphics::Intersectable(materialId, transform) {}

    private:
        const float r;
        const glm::vec3 c;
}; 

class Triangle : public ::SimpleGraphics::Intersectable {
    public:
        Raytracing::HitInfo intersect(const Raytracing::Ray& ray) override;

        Triangle(glm::vec3* vertices, int materialId) 
            : vertices(vertices), ::SimpleGraphics::Intersectable(materialId) {}

        Triangle(glm::vec3* vertices, int materialId, glm::mat4 transform) 
            : vertices(vertices), ::SimpleGraphics::Intersectable(materialId, transform) {}

    private:
        // For now, assume vertices in clockwise orientation
        glm::vec3 normal;
        glm::vec3* vertices;

};

} // namespace SimpleGraphic

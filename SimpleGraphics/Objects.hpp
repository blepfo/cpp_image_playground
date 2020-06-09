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

        Triangle(
            glm::vec3 A, 
            glm::vec3 B, 
            glm::vec3 C, 
            int materialId
        ) 
            : A(A), B(B), C(C),
            ::SimpleGraphics::Intersectable(materialId) {
                this->normal = glm::normalize(glm::cross(B-A, C-A));
            }

        Triangle(
            glm::vec3 A, 
            glm::vec3 B, 
            glm::vec3 C, 
            int materialId, 
            glm::mat4 transform
        ) 
            : A(A), B(B), C(C),
            ::SimpleGraphics::Intersectable(materialId, transform) {
                this->normal = glm::normalize(glm::cross(B-A, C-A));
            }

    private:
        // For now, assume vertices in clockwise orientation
        glm::vec3 A;
        glm::vec3 B;
        glm::vec3 C;
        glm::vec3 normal;

};

} // namespace SimpleGraphic

#pragma once

#include <sstream>
#include <string>

#include<glm/glm.hpp>

namespace SimpleGraphics {

/** 
 * Class represenging a ray intersectable object
 * Contains a materialId. For now, assume this corresponds to a
 * ::SimpleGraphics::Material stored in a ::SimpleGraphics::Scene
 */
class Intersectable : public Raytracing::RayIntersectable {
    public:
        int materialId;

        virtual std::string toString() const = 0;

        Intersectable(int materialId) 
            : materialId(materialId), Raytracing::RayIntersectable() {}

        Intersectable(int materialId, glm::mat4 transform) 
            : materialId(materialId), Raytracing::RayIntersectable(transform) {}
};



class Sphere : public ::SimpleGraphics::Intersectable {
    public:
        Raytracing::HitInfo intersect(const Raytracing::Ray& ray) const override; 

        std::string toString() const override {
            std::stringstream ss;
            ss 
                << "SimpleGraphics::Sphere"
                << "\n\tradius=" << this->r
                << "\n\tcenter=(" << this->c[0] << ", " << this->c[1] << ", " << this->c[2] << ")"
                << "\n\tmatId=" << this->materialId << ")"
            ;
            return ss.str();
        }

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
        Raytracing::HitInfo intersect(const Raytracing::Ray& ray) const override;

        Triangle(
            const glm::vec3 A, 
            const glm::vec3 B, 
            const glm::vec3 C, 
            int materialId
        ) 
            : A(A), B(B), C(C),
            ::SimpleGraphics::Intersectable(materialId) {
                this->normal = glm::normalize(glm::cross(B-A, C-A));
            }

        std::string toString() const override {
            std::stringstream ss;
            ss 
                << "SimpleGraphics::Triangle"
                << "\n\tA=(" << this->A[0] << ", " << this->A[1] << ", " << this->A[2] << ")"
                << "\n\tB=(" << this->B[0] << ", " << this->B[1] << ", " << this->B[2] << ")"
                << "\n\tC=(" << this->C[0] << ", " << this->C[1] << ", " << this->C[2] << ")"
                << "\n\tmatId=" << this->materialId
            ;
            return ss.str();
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

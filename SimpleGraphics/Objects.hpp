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
        bool castShadows = true;

        virtual std::string toString() const = 0;

        Intersectable(int materialId) 
            : materialId(materialId), Raytracing::RayIntersectable() {}

        Intersectable(int materialId, bool castShadows) 
            : materialId(materialId), castShadows(castShadows), Raytracing::RayIntersectable() {}

        Intersectable(int materialId, glm::dmat4 transform) 
            : materialId(materialId), Raytracing::RayIntersectable(transform) {}

        Intersectable(int materialId, glm::dmat4 transform, bool castShadows) 
            : materialId(materialId), castShadows(castShadows), Raytracing::RayIntersectable(transform) {}

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
                << "\n\tmatId=" << this->materialId
                << "\n\tcastShadows=" << this->castShadows
            ;
            return ss.str();
        }

        Sphere(double r, glm::dvec3 c, int materialId, bool castShadows=true) 
            : r(r), c(c), ::SimpleGraphics::Intersectable(materialId, castShadows) {}

        Sphere(double r, glm::dvec3 c, int materialId, glm::dmat4 transform, bool castShadows=true) 
            : r(r), c(c), ::SimpleGraphics::Intersectable(materialId, transform, castShadows) {}

    private:
        const double r;
        const glm::dvec3 c;
};


class Triangle : public ::SimpleGraphics::Intersectable {
    public:
        Raytracing::HitInfo intersect(const Raytracing::Ray& ray) const override;

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
            const glm::dvec3 A, 
            const glm::dvec3 B, 
            const glm::dvec3 C, 
            int materialId,
            bool castShadows=true
        ) 
            : A(A), B(B), C(C),
            ::SimpleGraphics::Intersectable(materialId, castShadows) {
                this->normal = glm::normalize(glm::cross(B-A, C-A));
            }

        Triangle(
            const glm::dvec3 A, 
            const glm::dvec3 B, 
            const glm::dvec3 C, 
            int materialId, 
            const glm::dmat4 transform,
            bool castShadows=true
        )
            : A(A), B(B), C(C),
        ::SimpleGraphics::Intersectable(materialId, transform, castShadows) {
                this->normal = glm::normalize(glm::cross(B-A, C-A));
            }

    private:
        // For now, assume vertices in clockwise orientation
        glm::dvec3 A;
        glm::dvec3 B;
        glm::dvec3 C;
        // TODO - vertex normal
        glm::dvec3 normal;

};

/**
 * Class representing a triangle whose vertices are pointers to glm::dvec3
 * Useful when e.g. all vertices in a scene are stored elsewhere and we want
 * a triangle that references them.
 */
class RefTriangle : public ::SimpleGraphics::Intersectable {
    public:
        Raytracing::HitInfo intersect(const Raytracing::Ray& ray) const override;

        std::string toString() const override {
            std::stringstream ss;
            ss 
                << "SimpleGraphics::RefTriangle"
                << "\n\tA=(" << (*this->A)[0] << ", " << (*this->A)[1] << ", " << (*this->A)[2] << ")"
                << "\n\tB=(" << (*this->B)[0] << ", " << (*this->B)[1] << ", " << (*this->B)[2] << ")"
                << "\n\tC=(" << (*this->C)[0] << ", " << (*this->C)[1] << ", " << (*this->C)[2] << ")"
                << "\n\tmatId=" << this->materialId
            ;
            return ss.str();
        }

        RefTriangle(
            glm::dvec3* A,
            glm::dvec3* B,
            glm::dvec3* C,
            int materialId,
            bool castShadows=true
        ) 
            : A(A), B(B), C(C),
            ::SimpleGraphics::Intersectable(materialId, castShadows) {
                this->normal = glm::normalize(glm::cross(*B-*A, *C-*A));
            }

        RefTriangle(
            glm::dvec3* A, 
            glm::dvec3* B, 
            glm::dvec3* C, 
            int materialId, 
            glm::dmat4 transform,
            bool castShadows=true
        )
            : A(A), B(B), C(C),
            ::SimpleGraphics::Intersectable(materialId, transform, castShadows) {
                this->normal = glm::normalize(glm::cross(*B-*A, *C-*A));
            }
    private: 
        glm::dvec3* A;
        glm::dvec3* B;
        glm::dvec3* C;
        // TODO - vertex normals
        glm::dvec3 normal;
};

} // namespace SimpleGraphic

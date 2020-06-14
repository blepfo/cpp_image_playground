#include<algorithm>

// TODO
#include "Raytracing.hpp"

#include "SimpleGraphics/Objects.hpp"

namespace SimpleGraphics {

::Raytracing::HitInfo intersectTriangle(
    const ::Raytracing::Ray& ray, 
    const glm::dvec3 A,
    const glm::dvec3 B,
    const glm::dvec3 C,
    // TODO vertex normals
    const glm::dvec3 normal,
    const int materialId
);


::Raytracing::HitInfo Sphere::intersect(const ::Raytracing::Ray& ray) const {
    const glm::dvec3 originMinusCenter = ray.o - this->c;
    const double a = glm::dot(ray.d, ray.d);
    const double b = 2.0 * glm::dot(ray.d, originMinusCenter);
    const double c = glm::dot(originMinusCenter, originMinusCenter) - (this->r * this->r);

    double tIntersection = -1.0;
    const double discriminant = (b*b) - (4.0 * a * c);
    if (discriminant < 0.00001) {
        // Ray does not intersect sphere (forward or backward)
        return ::Raytracing::noHit;
    } else {
        // Intersection
        const double denom = 1.0 / (2.0 * a);
        const double sqrtDiscriminant = sqrt(discriminant);
        const double t1 = (-b + sqrtDiscriminant) * denom;
        const double t2 = (-b - sqrtDiscriminant) * denom;
        if (t1 > 0.0 && t2 > 0.0) tIntersection = std::min(t1, t2);
        if (t1 > 0.0 && t2 < 0.0) tIntersection = t1;
        if (t1 < 0.0 && t2 > 0.0) tIntersection = t2;
    }
    if (tIntersection > 0.0) {
        // Compute intersection info
        const glm::dvec3 intersectionPoint = ray.o + (tIntersection * ray.d);
        const glm::dvec3 normal = glm::normalize(intersectionPoint - this->c);

        const double distanceToIntersection = glm::distance(ray.o, intersectionPoint);

        const ::Raytracing::HitInfo intersectionInfo = {
            distanceToIntersection,                 // distance
            intersectionPoint,                      // p
            normal,                                 // n
            this->materialId                        // materialId
        };
        return intersectionInfo;
    } else {
        // Only reach here if the intersection is behind the camera (t1 < 0 && t2 < 0)
        return ::Raytracing::noHit;
    }
}


::Raytracing::HitInfo Triangle::intersect(const ::Raytracing::Ray& ray) const {
    return intersectTriangle(ray, this->A, this->B, this->C, this->normal, this->materialId);
}

::Raytracing::HitInfo RefTriangle::intersect(const ::Raytracing::Ray& ray) const {
    return intersectTriangle(
        ray, 
        *(this->A), 
        *(this->B), 
        *(this->C), 
        this->normal, 
        this->materialId
    );
}

::Raytracing::HitInfo intersectTriangle(
    const ::Raytracing::Ray& ray, 
    const glm::dvec3 A,
    const glm::dvec3 B,
    const glm::dvec3 C,
    // TODO vertex normals
    const glm::dvec3 normal,
    const int materialId
) {
    // Ray-plane intersection using Moller-Trumbore
    // Triangle vertices = A B C
    // Barycentric coordinates = alpha beta gamma
    const glm::dvec3 AB = B - A;
    const glm::dvec3 AC = C - A;

    // Reused calculations
    const glm::dvec3 AO = ray.o - A;
    const glm::dvec3 AOxAB = glm::cross(AO, AB);
    const glm::dvec3 DxAC = glm::cross(ray.d, AC);
    
    const double totalDeterminant = glm::dot(DxAC, AB);
    // If determinant is 0, unable to solve system
    // If determinant is < 0, then ray is in same direction as normal == backface
    if (totalDeterminant <= 0.00001) {
        return ::Raytracing::noHit;
    }

    const double invTotalDeterminant = 1.0 / totalDeterminant;

    const double t = invTotalDeterminant * glm::dot(AOxAB, AC);
    if (t < 0.0) return ::Raytracing::noHit;

    const double beta = invTotalDeterminant * glm::dot(DxAC, AO);
    if (beta < 0.0 || beta > 1.0) return ::Raytracing::noHit;

    const double gamma = invTotalDeterminant * glm::dot(AOxAB, ray.d);
    if (gamma < 0.0 || gamma + beta > 1.0) return ::Raytracing::noHit;

    // If we reach here, found forward intersection inside the triangle
    const glm::dvec3 intersectionPoint = ray.o + (t * ray.d);
    const double intersectionDistance = glm::distance(ray.o, intersectionPoint);
    const ::Raytracing::HitInfo triangleHit = {
        intersectionDistance,               // distance
        intersectionPoint,                  // p
        normal,                       // n
        materialId                    // material
    };
    return triangleHit;
}


} // namespace SimpleGraphics

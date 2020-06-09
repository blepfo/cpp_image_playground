#include<algorithm>

// TODO
#include "Raytracing.hpp"

#include "SimpleGraphics/Objects.hpp"

namespace SimpleGraphics {

::Raytracing::HitInfo Sphere::intersect(const ::Raytracing::Ray& ray) {
    glm::vec3 originMinusCenter = ray.o - this->c;
    float a = glm::dot(ray.d, ray.d);
    float b = 2.0f * glm::dot(ray.d, originMinusCenter);
    float c = glm::dot(originMinusCenter, originMinusCenter) - (this->r * this->r);

    float tIntersection = -1.0f;
    float discriminant = (b*b) - (4.0f * a * c);
    if (discriminant < 0) {
        // Ray does not intersect sphere (forward or backward)
        return ::Raytracing::noHit;
    } else {
        // Intersection
        float t1 = (-b + sqrt(discriminant)) / (2.0f * a);
        float t2 = (-b - sqrt(discriminant)) / (2.0f * a);
        if (t1 > 0 && t2 > 0) tIntersection = std::min(t1, t2);
        if (t1 > 0 && t2 < 0) tIntersection = t1;
        if (t1 < 0 && t2 > 0) tIntersection = t2;
    }
    if (tIntersection > 0) {
        // Compute intersection info
        glm::vec3 intersectionPoint = ray.o + (tIntersection * ray.d);
        glm::vec3 normal = glm::normalize(intersectionPoint - this->c);

        float distanceToIntersection = glm::distance(ray.o, intersectionPoint);

        ::Raytracing::HitInfo intersectionInfo = {
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


::Raytracing::HitInfo Triangle::intersect(const ::Raytracing::Ray& ray) {
    // Ray-plane intersection using Moller-Trumbore
    // Triangle vertices = A B C
    // Barycentric coordinates = alpha beta gamma
    glm::vec3 AB = this->B - this->A;
    glm::vec3 AC = this->C - this->A;

    // Reused calculations
    glm::vec3 AO = ray.o - this->A;
    glm::vec3 AOxAB = glm::cross(AO, AB);
    glm::vec3 DxAC = glm::cross(ray.d, AC);
    
    float totalDeterminant = glm::dot(DxAC, AB);
    // If determinant is 0, unabel to solve system
    if (std::abs(totalDeterminant) < 0.01) {
        return ::Raytracing::noHit;
    }
    // Backface culling
    if (totalDeterminant < 0.0f) {
        return ::Raytracing::noHit;
    }

    float invTotalDeterminant = 1.0f / totalDeterminant;

    float t = invTotalDeterminant * glm::dot(AOxAB, AC);
    if (t < 0.0f) return ::Raytracing::noHit;

    float beta = invTotalDeterminant * glm::dot(DxAC, AO);
    if (beta < 0.0f || beta > 1.0f) return ::Raytracing::noHit;

    float gamma = invTotalDeterminant * glm::dot(AOxAB, ray.d);
    if (gamma < 0.0f || gamma + beta > 1.0f) return ::Raytracing::noHit;

    // If we reach here, found forward intersection inside the triangle
    glm::vec3 intersectionPoint = ray.o + (t * ray.d);
    float intersectionDistance = glm::distance(ray.o, intersectionPoint);
    ::Raytracing::HitInfo triangleHit = {
        intersectionDistance,               // distance
        intersectionPoint,                  // p
        this->normal,                       // n
        this->materialId                    // material
    };
    return triangleHit;
}


} // namespace SimpleGraphics

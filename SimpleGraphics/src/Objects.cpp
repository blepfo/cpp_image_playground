#include<algorithm>

// TODO
#include "Raytracing.hpp"

#include "SimpleGraphics/Objects.hpp"

namespace SimpleGraphics {

::Raytracing::HitInfo Sphere::intersect(const ::Raytracing::Ray& ray) {
    ::Raytracing::Ray localRay = this->localRay(ray);

    glm::vec3 originMinusCenter = localRay.o - this->c;
    float a = glm::dot(localRay.d, localRay.d);
    float b = 2.0f * glm::dot(localRay.d, originMinusCenter);
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
        glm::vec3 intersectionLocal = localRay.o + (tIntersection * localRay.d);
        glm::vec3 normalLocal = glm::normalize(intersectionLocal - this->c);

        glm::vec3 intersectionWorld = this->localPointToWorld(intersectionLocal);
        float distanceToIntersection = glm::distance(ray.o, intersectionWorld);

        ::Raytracing::HitInfo intersectionInfo = {
            distanceToIntersection,                 // distance
            intersectionWorld,                      // p
            this->localNormalToWorld(normalLocal),  // n
            this->materialId                        // materialId
        };
        return intersectionInfo;
    } else {
        // Only reach here if the intersection is behind the camera (t1 < 0 && t2 < 0)
        return ::Raytracing::noHit;
    }
}

::Raytracing::HitInfo Triangle::intersect(const ::Raytracing::Ray& ray) {
    // Transform ray to object space, intersect, and transform intersection to world space
    ::Raytracing::Ray localRay = this->localRay(ray);
    
    // Ray-plane intersection

    // TODO
    return ::Raytracing::noHit;
}


} // namespace SimpleGraphics

#include<algorithm>

#include "../include/Intersectables.h"
#include "../include/Raytracing.h"

namespace Intersectables {

Raytracing::HitInfo Sphere::intersect(const Raytracing::Ray& ray) {
    glm::vec3 originMinusCenter = ray.o - this->c;
    float a = glm::dot(ray.d, ray.d);
    float b = 2.0f * glm::dot(ray.d, originMinusCenter);
    float c = glm::dot(originMinusCenter, originMinusCenter) - (this->r * this->r);

    float tIntersection = -1.0f;
    float discriminant = (b*b) - (4.0f * a * c);
    if (discriminant < 0) {
        // No sphere intersection
        return Raytracing::noHit;
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
        glm::vec3 intersectionPoint = ray.o + tIntersection*ray.d;
        glm::vec3 normal = glm::normalize(intersectionPoint - this->c);
        Raytracing::HitInfo intersectionInfo = { tIntersection, intersectionPoint, normal, this };
        return intersectionInfo;
    } else {
        // Only reach here if the intersection is behind the camera (t1 < 0 && t2 < 0)
        return Raytracing::noHit;
    }
}


} // namespace Intersectables

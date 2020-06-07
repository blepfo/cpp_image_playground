#ifndef _RAYTRACING_H
#define _RAYTRACING_H

#include<glm/glm.hpp>

namespace Raytracing {
    typedef struct {} SceneObject;
    
    typedef struct {
        glm::vec3 o;    // Origin
        glm::vec3 d;    // Direction
    } Ray;

    typedef struct {
        float t;
        glm::vec3 p;    // Intersection point
        glm::vec3 n;    // Normal
        SceneObject* obj;
    } HitInfo;

    const HitInfo noHit = { 0.0f, glm::vec3(0.0f), glm::vec3(0.0f), NULL };

    class RayIntersectable : public SceneObject {
        virtual HitInfo intersect(Ray r) = 0;
    };

    class Illuminator {
        virtual glm::vec3 illuminate(HitInfo hitInfo, glm::vec3 eye) = 0;
    };

} // namespace Raytracing

#endif

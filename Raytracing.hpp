#pragma once

#include<glm/glm.hpp>


namespace Raytracing {
    typedef struct {} SceneObject;
   
    typedef struct {
        glm::vec3 o;    // Origin
        glm::vec3 d;    // Direction
    } Ray;

    typedef struct {
        float distance;
        glm::vec3 p;    // Intersection point
        glm::vec3 n;    // Normal
        int materialId;
    } HitInfo;

    const HitInfo noHit = { -1.0f, glm::vec3(0.0f), glm::vec3(0.0f), -1 };

    class RayIntersectable : public SceneObject {
        public:
            virtual HitInfo intersect(const Ray& r) const = 0;
            glm::mat4 transform;

            void setTransform(const glm::mat4 transform) {
                this->transform = transform;
            }

            void rightTransform(const glm::mat4 toRightMultiply) {
                this->transform = this->transform * toRightMultiply;
            }

            void leftTransform(const glm::mat4 toLeftMultiply) {
                this->transform = toLeftMultiply * this->transform;
            }

            Ray localRay(const Ray& worldRay) const {
                // To intersect transformed object, we transform ray into local space,
                // do intersection, then transform back to world space
                glm::mat4 worldToLocal = glm::inverse(this->transform);
                glm::vec4 localOriginH = worldToLocal * glm::vec4(worldRay.o, 1.0f);
                glm::vec3 localRayOrigin = glm::vec3(localOriginH) / localOriginH[3];
                // Note that we do NOT normalize the local ray 
                // to preserve the space scaling
                glm::vec3 localRayDirection = glm::vec3(worldToLocal * glm::vec4(worldRay.d, 0.0f));
                Ray localRay = { localRayOrigin, localRayDirection };
                return localRay;
            }

            glm::vec3 localPointToWorld(const glm::vec3 pLocal) const {
                glm::vec4 pWorld = this->transform * glm::vec4(pLocal, 1.0f);
                return glm::vec3(pWorld) / pWorld[3];
            }

            glm::vec3 localNormalToWorld(const glm::vec3 nLocal) const {
                glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(this->transform)));
                return glm::normalize(normalMatrix * nLocal);
            }
            
            RayIntersectable() : transform(glm::mat4(1.0f)) {}
            RayIntersectable(glm::mat4 transform) : transform(transform) {}
    };

} // namespace Raytracing

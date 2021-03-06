#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>


namespace Raytracing {
    typedef struct {} SceneObject;
   
    typedef struct {
        glm::dvec3 o;    // Origin
        glm::dvec3 d;    // Direction
    } Ray;

    typedef struct {
        double distance;
        glm::dvec3 p;    // Intersection point
        glm::dvec3 n;    // Normal
        int materialId;
    } HitInfo;

    const HitInfo noHit = { -1.0, glm::dvec3(0.0), glm::dvec3(0.0), -1 };

    class RayIntersectable : public SceneObject {
        public:
            virtual HitInfo intersect(const Ray& r) const = 0;
            glm::dmat4 transform;
            glm::dmat4 invTransform;
            glm::dmat3 normalMatrix;

            void setTransform(const glm::dmat4 transform) {
                this->transform = transform;
                this->invTransform = glm::inverse(this->transform);
                this->normalMatrix = glm::inverseTranspose(glm::dmat3(this->transform));
            }

            void rightTransform(const glm::dmat4 toRightMultiply) {
                this->setTransform(this->transform * toRightMultiply);
            }

            void leftTransform(const glm::dmat4 toLeftMultiply) {
                this->setTransform(toLeftMultiply * this->transform);
            }

            Ray localRay(const Ray& worldRay) const {
                // To intersect transformed object, we transform ray into local space,
                // do intersection, then transform back to world space
                const glm::dvec4 localOriginH = this->invTransform * glm::dvec4(worldRay.o, 1.0);
                const glm::dvec3 localRayOrigin = glm::dvec3(localOriginH) / localOriginH[3];
                // Note that we do NOT normalize the local ray 
                // to preserve the space scaling
                const glm::dvec3 localRayDirection = glm::dvec3(this->invTransform * glm::dvec4(worldRay.d, 0.0));
                const Ray localRay = { localRayOrigin, localRayDirection };
                return localRay;
            }

            glm::dvec3 localPointToWorld(const glm::dvec3 pLocal) const {
                const glm::dvec4 pWorld = this->transform * glm::dvec4(pLocal, 1.0);
                return glm::dvec3(pWorld) / pWorld[3];
            }

            glm::dvec3 localNormalToWorld(const glm::dvec3 nLocal) const {
                return glm::normalize(this->normalMatrix * nLocal);
            }
            
            RayIntersectable() : transform(glm::dmat4(1.0)) {
                this->setTransform(this->transform);
            }
            RayIntersectable(glm::dmat4 transform) : transform(transform) {
                this->setTransform(this->transform);
            }
    };

} // namespace Raytracing

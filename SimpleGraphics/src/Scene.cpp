#include<glm/glm.hpp>

// TODO
#include "Raytracing.hpp"

#include "SimpleGraphics/Lights.hpp"
#include "SimpleGraphics/Materials.hpp"
#include "SimpleGraphics/Objects.hpp"
#include "SimpleGraphics/Scene.hpp"


namespace SimpleGraphics {

::Raytracing::HitInfo rayCast(
    const ::Raytracing::Ray& ray, 
    const ::SimpleGraphics::Scene& scene,
    const bool shadowRay
) {
    ::Raytracing::HitInfo sceneHit = ::Raytracing::noHit;
    ::SimpleGraphics::Intersectable* hitObject = NULL;
    for (int objectNum = 0; objectNum < scene.objects.size(); objectNum++) {
        ::SimpleGraphics::Intersectable* currentObject = scene.objects[objectNum];

        // Don't bother intersecting if this is a shadow ray and the object doesn't cast shadows
        bool canIntersectObject = (!shadowRay || (shadowRay && currentObject->castShadows));
        if (!canIntersectObject) continue;

        // Intersect in local space, then transform back to world space
        ::Raytracing::Ray localRay = currentObject->localRay(ray);
        ::Raytracing::HitInfo localHit = currentObject->intersect(localRay);

        if (localHit.distance < 0.00001) continue;
        else {
            // Convert from local space to world space
            glm::dvec3 worldIntersectionPoint = currentObject->localPointToWorld(localHit.p);
            double intersectionDistance = glm::distance(worldIntersectionPoint, ray.o);
            ::Raytracing::HitInfo worldHit = {
                intersectionDistance,
                worldIntersectionPoint,
                currentObject->localNormalToWorld(localHit.n),
                localHit.materialId
            };
            // Found first forward intersection OR found closer forward intersection
            if (sceneHit.distance < 0.0 || (intersectionDistance < sceneHit.distance)) {
                sceneHit = worldHit;
                hitObject = currentObject;
            }
        }
    }
    return sceneHit;
}

bool isInShadow(
    const glm::dvec3 shadowRayOrigin, 
    const glm::dvec3 lightDir, 
    const double lightDistance, 
    const ::SimpleGraphics::Scene& scene
) {
    const ::Raytracing::Ray rayTowardsLight = { shadowRayOrigin, glm::normalize(lightDir) };
    const ::Raytracing::HitInfo hitOnWayTowardsLight = rayCast(rayTowardsLight, scene, true);

    if (hitOnWayTowardsLight.distance > 0.0) {
        return hitOnWayTowardsLight.distance < lightDistance;
    } else {
        // No forward hit
        return false;
    }
}


glm::vec3 whittedRayTrace(
    const ::Raytracing::Ray& viewRay, 
    const ::SimpleGraphics::Scene& scene,
    const glm::dvec3 missColor,
    const int maxBounces
) {
    if (maxBounces < 0) {
        // When maxBounces negative, finished raytracing
        return glm::dvec3(0.0);
    } else {
        ::Raytracing::HitInfo sceneHit = rayCast(viewRay, scene);
        if (sceneHit.distance > 0.0) {
            // Get material for object
            ::SimpleGraphics::Material* material = scene.materials[sceneHit.materialId];
            glm::dvec3 illumination = material->ambient(sceneHit.p) + material->emission(sceneHit.p);
            for (int lightNum = 0; lightNum < scene.lights.size(); lightNum++) {
                // Determine visibility using Shadow Ray
                ::SimpleGraphics::Light* currentLight = scene.lights[lightNum];
                glm::dvec3 lightDir = currentLight->getDirection(sceneHit.p);
                double lightDistance = currentLight->getDistance(sceneHit.p);
                glm::dvec3 shadowRayOrigin = sceneHit.p + (sceneHit.n * 0.00001);
                bool shadowed = isInShadow(shadowRayOrigin, lightDir, lightDistance, scene);

                // No light contribution if in shadow
                if (!shadowed) {
                    illumination += currentLight->illuminate(
                        sceneHit.p,
                        sceneHit.n,
                        viewRay.o,
                        material
                    );
                }
            }
            // Weight reflection using specular
            glm::dvec3 specular = material->specular(sceneHit.p);
            glm::dvec3 reflectedDir = glm::reflect(viewRay.d, sceneHit.n);

            // Move reflection away from the surface to prevent numerical issues
            glm::dvec3 reflectionOrigin = sceneHit.p + (sceneHit.n * 0.00001);
            ::Raytracing::Ray reflectionRay = { reflectionOrigin, reflectedDir };
            glm::dvec3 reflectionColor = whittedRayTrace(
                reflectionRay,
                scene,
                missColor,
                maxBounces - 1
            );
            return illumination + (reflectionColor * specular);
        } else {
            // No Scene hit
            return missColor;
        }
    }
}

} // namespace SimpleGraphics

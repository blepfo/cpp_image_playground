#include<glm/glm.hpp>

// TODO
#include "Raytracing.hpp"

#include "SimpleGraphics/Lights.hpp"
#include "SimpleGraphics/Materials.hpp"
#include "SimpleGraphics/Objects.hpp"
#include "SimpleGraphics/Scene.hpp"


namespace SimpleGraphics {

::Raytracing::HitInfo rayCast(const ::Raytracing::Ray& ray, const ::SimpleGraphics::Scene& scene) {
    ::Raytracing::HitInfo sceneHit = ::Raytracing::noHit;
    ::SimpleGraphics::Intersectable* hitObject = NULL;
    for (int objectNum = 0; objectNum < scene.objects.size(); objectNum++) {
        ::SimpleGraphics::Intersectable* currentObject = scene.objects[objectNum];
        // Intersect in local space, then transform back to world space
        ::Raytracing::Ray localRay = currentObject->localRay(ray);
        ::Raytracing::HitInfo localHit = currentObject->intersect(localRay);

        if (localHit.distance < 0) continue;
        else {
            // Convert from local space to world space
            glm::vec3 worldIntersectionPoint = currentObject->localPointToWorld(localHit.p);
            float intersectionDistance = glm::distance(worldIntersectionPoint, ray.o);
            ::Raytracing::HitInfo worldHit = {
                intersectionDistance,
                worldIntersectionPoint,
                currentObject->localNormalToWorld(localHit.n),
                localHit.materialId
            };
            if (sceneHit.distance < 0 || (intersectionDistance < sceneHit.distance)) {
                // Found first forward intersection OR found closer forward intersection
                sceneHit = worldHit;
                hitObject = currentObject;
            }
        }
    }
    return sceneHit;
}

bool isInShadow(
    const glm::vec3 p, 
    const glm::vec3 lightDir, 
    const float lightDistance, 
    const ::SimpleGraphics::Scene& scene
) {
    ::Raytracing::Ray rayTowardsLight = { p, glm::normalize(lightDir) };
    ::Raytracing::HitInfo hitOnWayTowardsLight = rayCast(rayTowardsLight, scene);

    if (hitOnWayTowardsLight.distance > 0) {
        return hitOnWayTowardsLight.distance < lightDistance;
    } else {
        // No forward hit
        return false;
    }
}


glm::vec3 whittedRayTrace(
    const ::Raytracing::Ray& viewRay, 
    const ::SimpleGraphics::Scene& scene,
    glm::vec3 missColor
) {
    ::Raytracing::HitInfo sceneHit = rayCast(viewRay, scene);
    if (sceneHit.distance > 0) {
        // Get material for object
        ::SimpleGraphics::Material* material = scene.materials[sceneHit.materialId];
        glm::vec3 illumination = material->ambient(sceneHit.p) + material->emission(sceneHit.p);
        for (int lightNum = 0; lightNum < scene.lights.size(); lightNum++) {
            // Determine visibility using Shadow Ray
            ::SimpleGraphics::Light* currentLight = scene.lights[lightNum];
            glm::vec3 lightDir = currentLight->getDirection(sceneHit.p);
            float lightDistance = currentLight->getDistance(sceneHit.p);
            glm::vec3 startPoint = sceneHit.p + (sceneHit.n * 0.01f);
            bool shadowed = isInShadow(startPoint, lightDir, lightDistance, scene);

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

        // TODO Secondary rays
        // Reflection 
        return illumination;
    } else {
        // No Scene hit
        return missColor;
    }
}

} // namespace SimpleGraphics

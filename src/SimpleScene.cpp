#include<glm/glm.hpp>

#include "../include/Raytracing.h"
#include "../include/SimpleLights.h"
#include "../include/SimpleScene.h"



namespace SimpleScene {

Raytracing::HitInfo rayCast(const Raytracing::Ray& ray, const Scene& scene) {
    Raytracing::HitInfo sceneHit = Raytracing::noHit;
    SimpleObject* hitObject = NULL;
    for (int objectNum = 0; objectNum < scene.objects.size(); objectNum++) {
        Raytracing::HitInfo currentHit = scene.objects[objectNum]->intersect(ray);

        if (currentHit.distance < 0) continue;
        else if (sceneHit.distance < 0 || (currentHit.distance < sceneHit.distance)) {
            // Found first forward intersection OR found closer forward intersection
            sceneHit = currentHit;
            hitObject = scene.objects[objectNum];
        }
    }
    return sceneHit;
}

bool isInShadow(
    const glm::vec3 p, 
    const glm::vec3 lightDir, 
    const float lightDistance, 
    const Scene& scene
) {
    Raytracing::Ray rayTowardsLight = { p, glm::normalize(lightDir) };
    Raytracing::HitInfo hitOnWayTowardsLight = rayCast(rayTowardsLight, scene);

    if (hitOnWayTowardsLight.distance > 0) {
        return hitOnWayTowardsLight.distance < lightDistance;
    } else {
        // No forward hit
        return false;
    }
}


glm::vec3 whittedRayTrace(
    const Raytracing::Ray& viewRay, 
    const Scene& scene,
    glm::vec3 missColor
) {
    Raytracing::HitInfo sceneHit = rayCast(viewRay, scene);
    if (sceneHit.distance > 0) {
        // Get material for object
        SimpleMaterial::Material* material = scene.materials[sceneHit.materialId];
        glm::vec3 illumination = material->ambient + material->emission;
        for (int lightNum = 0; lightNum < scene.lights.size(); lightNum++) {
            // Determine visibility using Shadow Ray
            SimpleLights::SimpleLight* currentLight = scene.lights[lightNum];
            glm::vec3 lightDir = currentLight->getDirection(sceneHit.p);
            float lightDistance = currentLight->getDistance(sceneHit.p);
            glm::vec3 startPoint = sceneHit.p + (sceneHit.n * 0.1f);
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

} // namespace Raytracing

#include<glm/glm.hpp>

#include "../include/Raytracing.h"
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
            illumination += scene.lights[lightNum]->illuminate(
                sceneHit.p,
                sceneHit.n,
                viewRay.o,
                material
            );
        }

        // TODO Secondary rays
        // Shadow Ray 
        // Reflection 
        return illumination;
    } else {
        // No Scene hit
        return missColor;
    }
}

} // namespace Raytracing

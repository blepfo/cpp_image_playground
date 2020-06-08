#pragma once

#include<vector>

#include<glm/glm.hpp>

#include "./Raytracing.h"
#include "./SimpleMaterial.h"
#include "./SimpleLights.h"

namespace SimpleScene {

class SimpleObject : public Raytracing::RayIntersectable {
    public:
        int materialId;

        SimpleObject(int materialId) 
            : materialId(materialId), Raytracing::RayIntersectable() {}

        SimpleObject(int materialId, glm::mat4 transform) 
            : materialId(materialId), Raytracing::RayIntersectable(transform) {}
};

class Scene {
    public:
        std::vector<SimpleObject*> objects;
        std::vector<SimpleLights::SimpleLight*> lights;
        std::vector<SimpleMaterial::Material*> materials;

        // Return ID of added material
        int addMaterial(SimpleMaterial::Material* material) {
            int materialId = this->materials.size();
            this->materials.push_back(material);
            return materialId;
        } 

        int addObject(SimpleObject* object) {
            int objectId = this->objects.size();
            this->objects.push_back(object);
            return objectId;
        }

        int addLight(SimpleLights::SimpleLight* light) {
            int lightId = this->lights.size();
            this->lights.push_back(light);
            return lightId;
        }

        Raytracing::HitInfo rayCast(const Raytracing::Ray& ray, const Scene& scene);

        glm::vec3 whittedRayTrace(
            const Raytracing::Ray& viewRay,
            const Scene& scene,
            glm::vec3 missColor
        );

        Scene() {}

        Scene(std::vector<SimpleLights::SimpleLight*> lights)
            : lights(lights) {}
};

Raytracing::HitInfo rayCast(const Raytracing::Ray& ray, const Scene& scene); 

glm::vec3 whittedRayTrace(
    const Raytracing::Ray& viewRay, 
    const Scene& scene,
    glm::vec3 missColor
);

} // namespace SimpleScene

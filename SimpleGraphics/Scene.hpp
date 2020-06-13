#pragma once

#include <sstream>
#include <string>
#include <vector>

#include <glm/glm.hpp>

// TODO
#include "Raytracing.hpp"

#include "SimpleGraphics/Lights.hpp"
#include "SimpleGraphics/Materials.hpp"
#include "SimpleGraphics/Objects.hpp"

namespace SimpleGraphics {

class Scene {
    public:
        std::vector<::SimpleGraphics::Intersectable*> objects;
        std::vector<::SimpleGraphics::Light*> lights;
        std::vector<::SimpleGraphics::Material*> materials;

        // Return ID of added material
        int addMaterial(::SimpleGraphics::Material* material) {
            int materialId = this->materials.size();
            this->materials.push_back(material);
            return materialId;
        } 

        int addObject(::SimpleGraphics::Intersectable* object) {
            int objectId = this->objects.size();
            this->objects.push_back(object);
            return objectId;
        }

        int addLight(::SimpleGraphics::Light* light) {
            int lightId = this->lights.size();
            this->lights.push_back(light);
            return lightId;
        }

        std::string toString() {
            std::stringstream ss;
            ss << "----- SCENE -----";
            // Lights
            ss << "\n-- LIGHTS --";
            for (int i = 0; i < this->lights.size(); i++) {
                ss << "\n" << i << " - " << this->lights[i]->toString();
            }
            // Materials
            ss << "\n-- MATERIALS --";
            for (int i = 0; i < this->materials.size(); i++) {
                ss << "\n" << i << " - " << this->materials[i]->toString();
            }
            ss << "\n-- OBJECTS --";
            for (int i = 0; i < this->objects.size(); i++) {
                ss << "\n" << i << " - " << this->objects[i]->toString();
            }
            return ss.str();
        }

        Scene() {}

        Scene(std::vector<::SimpleGraphics::Light*> lights)
            : lights(lights) {}
};


::Raytracing::HitInfo rayCast(const ::Raytracing::Ray& ray, const ::SimpleGraphics::Scene& scene); 

bool isInShadow(
    const glm::vec3 p, 
    const glm::vec3 lightDir, 
    const float lightDistance, 
    const ::SimpleGraphics::Scene& scene
);

glm::vec3 whittedRayTrace(
    const ::Raytracing::Ray& viewRay, 
    const ::SimpleGraphics::Scene& scene,
    glm::vec3 missColor,
    int maxBounces
);

} // namespace SimpleScene

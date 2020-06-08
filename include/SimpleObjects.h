#pragma once

#include<vector>

#include<glm/glm.hpp>

#include "../include/SimpleScene.h"
#include "../include/SimpleMaterial.h"

namespace SimpleObjects {

class Sphere : public SimpleScene::SimpleObject {
    public:
        Raytracing::HitInfo intersect(const Raytracing::Ray& ray) override; 

        Sphere(float r, glm::vec3 c, int materialId) 
            : r(r), c(c), SimpleScene::SimpleObject(materialId) {}

        Sphere(float r, glm::vec3 c, int materialId, glm::mat4 transform) 
            : r(r), c(c), SimpleScene::SimpleObject(materialId, transform) {}

    private:
        const float r;
        const glm::vec3 c;
}; 

} // namespace Intersectables

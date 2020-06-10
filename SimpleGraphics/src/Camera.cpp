#include <glm/glm.hpp>

// TODO
#include "Raytracing.hpp"

#include "SimpleGraphics/Camera.hpp"

namespace SimpleGraphics {

::Raytracing::Ray Camera::viewRay(const float uvX, const float uvY) const {
    glm::vec3 imagePlaneCenter = this->eye + this->f;
    glm::vec3 imagePlaneIntersection = imagePlaneCenter
        + (this->r * uvX * this->fovXMultiplier)
        + (this->u * uvY * this->fovYMultiplier);
    glm::vec3 direction = glm::normalize(imagePlaneIntersection - this->eye);
    Raytracing::Ray ray = { this->eye, direction };
    return ray;
}

} // namespace SimpleGraphics

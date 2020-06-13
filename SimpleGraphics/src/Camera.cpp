#include <glm/glm.hpp>

// TODO
#include "Raytracing.hpp"

#include "SimpleGraphics/Camera.hpp"

namespace SimpleGraphics {

::Raytracing::Ray Camera::viewRay(const double uvX, const double uvY) const {
    const glm::dvec3 imagePlaneCenter = this->eye + this->f;
    const glm::dvec3 imagePlaneIntersection = imagePlaneCenter
        + (this->r * uvX * this->fovXMultiplier)
        + (this->u * uvY * this->fovYMultiplier);
    const glm::dvec3 direction = glm::normalize(imagePlaneIntersection - this->eye);
    const Raytracing::Ray ray = { this->eye, direction };
    return ray;
}

} // namespace SimpleGraphics

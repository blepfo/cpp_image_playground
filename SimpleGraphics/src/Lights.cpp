#include<algorithm>

#include<glm/glm.hpp>

#include "SimpleGraphics/Lights.hpp"
#include "SimpleGraphics/Materials.hpp"

namespace SimpleGraphics {

// Helper reused by PointLight and DirectionLight
glm::dvec3 illuminate(
    glm::dvec3 intensity, 
    glm::dvec3 lightDir, 
    glm::dvec3 p,
    glm::dvec3 n, 
    glm::dvec3 eye,
    const ::SimpleGraphics::Material* material
);

// TODO - doc
glm::dvec3 PointLight::illuminate(
    const glm::dvec3 p,
    const glm::dvec3 n,
    const glm::dvec3 eye,
    const ::SimpleGraphics::Material* material
) const {
    const glm::dvec3 lightDir = glm::normalize(this->o - p);
    const double lightDist = glm::distance(this->o, p);
    const double attenuation = 1.0 / (this->attenuation[0] + (this->attenuation[1] * lightDist) + (this->attenuation[2] * lightDist * lightDist));
    const glm::dvec3 effectiveIntensity = this->i * attenuation;
    return ::SimpleGraphics::illuminate(effectiveIntensity, lightDir, p, n, eye, material);
}

// TODO - doc
glm::dvec3 DirectionLight::illuminate(
    const glm::dvec3 p,
    const glm::dvec3 n,
    const glm::dvec3 eye,
    const ::SimpleGraphics::Material* material
) const {
    return ::SimpleGraphics::illuminate(this->i, this->d, p, n, eye, material);
}

/** 
 * Blinn-Phong illumination
 */
glm::dvec3 illuminate(
    const glm::dvec3 intensity, 
    const glm::dvec3 lightDir, 
    const glm::dvec3 p,
    const glm::dvec3 n,
    const glm::dvec3 eye,
    const SimpleGraphics::Material* material
) {
    // Diffuse
    const double intensity_diffuse = std::max(0.0, glm::dot(lightDir, n));
    const glm::dvec3 illumination_diffuse = intensity_diffuse * material->diffuse(p);
    // Specular
    //const glm::dvec3 viewDir = glm::normalize(eye - p);
    //const glm::dvec3 halfVector = glm::normalize(viewDir + lightDir);
    const glm::dvec3 halfVector = glm::normalize(eye + lightDir);
    const double intensity_specular = std::pow(std::max(0.0, glm::dot(halfVector, n)), material->shiny(p));
    const glm::dvec3 illumination_specular = intensity_specular * material->specular(p);

    return intensity * (illumination_diffuse + illumination_specular);

}


} // namespace SimpleGraphics

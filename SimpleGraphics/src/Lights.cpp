#include<algorithm>

#include<glm/glm.hpp>

#include "SimpleGraphics/Lights.hpp"
#include "SimpleGraphics/Materials.hpp"

namespace SimpleGraphics {

// Helper reused by PointLight and DirectionLight
glm::vec3 illuminate(
    glm::vec3 intensity, 
    glm::vec3 lightDir, 
    glm::vec3 p,
    glm::vec3 n, 
    glm::vec3 eye,
    const ::SimpleGraphics::Material* material
);

// TODO - doc
glm::vec3 PointLight::illuminate(
    glm::vec3 p,
    glm::vec3 n,
    glm::vec3 eye,
    const ::SimpleGraphics::Material* material
) const {
    glm::vec3 lightDir = glm::normalize(this->o - p);
    float lightDist = glm::distance(p, this->o);
    float attenuation = 1.0f / (this->attenuation[0] + (this->attenuation[1] * lightDist) + (this->attenuation[2] * lightDist * lightDist));
    glm::vec3 effectiveIntensity = this->i * attenuation;
    return ::SimpleGraphics::illuminate(effectiveIntensity, lightDir, p, n, eye, material);
}

// TODO - doc
glm::vec3 DirectionLight::illuminate(
    glm::vec3 p,
    glm::vec3 n,
    glm::vec3 eye,
    const ::SimpleGraphics::Material* material
) const {
    return ::SimpleGraphics::illuminate(this->i, this->d, p, n, eye, material);
}

/** 
 * Blinn-Phong illumination
 */
glm::vec3 illuminate(
    glm::vec3 intensity, 
    glm::vec3 lightDir, 
    glm::vec3 p,
    glm::vec3 n,
    glm::vec3 eye,
    const SimpleGraphics::Material* material
) {
    // Diffuse
    float intensity_diffuse = std::max(0.0f, glm::dot(lightDir, n));
    glm::vec3 illumination_diffuse = intensity_diffuse * intensity * material->diffuse(p);
    // Specular
    glm::vec3 halfVector = glm::normalize(eye + lightDir);
    float intensity_specular = std::pow(std::max(0.0f, glm::dot(halfVector, n)), material->shiny(p));
    glm::vec3 illumination_specular = intensity_specular * intensity * material->specular(p);

    return illumination_diffuse + illumination_specular;

}


} // namespace SimpleGraphics

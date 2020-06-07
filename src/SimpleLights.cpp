#include<algorithm>

#include<glm/glm.hpp>

#include "../include/Raytracing.h"
#include "../include/SimpleLights.h"

namespace SimpleLights {

// Helper reused by PointLight and DirectionLight
glm::vec3 illuminate(glm::vec3 intensity, glm::vec3 lightDir, Raytracing::HitInfo hitInfo, glm::vec3 eye); 

glm::vec3 PointLight::illuminate(Raytracing::HitInfo hitInfo, glm::vec3 eye) {
    glm::vec3 lightDir = glm::normalize(hitInfo.p - this->o);
    float lightDist = glm::distance(hitInfo.p, this->o);
    float attenuation = 1.0f / (this->attenuation[0] + (this->attenuation[1] * lightDist) + (this->attenuation[2] * lightDist * lightDist));
    glm::vec3 effectiveIntensity = this->i * attenuation;
    return ::SimpleLights::illuminate(effectiveIntensity, lightDir, hitInfo, eye);
}

glm::vec3 DirectionLight::illuminate(Raytracing::HitInfo hitInfo, glm::vec3 eye) {
    return ::SimpleLights::illuminate(this->i, this->d, hitInfo, eye);
}

/** 
 * Blinn-Phong illumination
 */
glm::vec3 illuminate(glm::vec3 intensity, glm::vec3 lightDir, Raytracing::HitInfo hitInfo, glm::vec3 eye) {
    float intensity_diffuse = std::max(0.0f, glm::dot(lightDir, hitInfo.n));
    // TODO - get material from hitInfo.obj
    glm::vec3 illumination_diffuse = intensity_diffuse * intensity * glm::vec3(1.0f, 0.0f, 0.0f);
    return illumination_diffuse;

}


} // namespace SimpleLights

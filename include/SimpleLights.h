#ifndef _SIMPLE_LIGHTS_H
#define _SIMPLE_LIGHTS_H

#include<glm/glm.hpp>

#include "../include/Raytracing.h"

namespace SimpleLights {

class SimpleLight : public Raytracing::Illuminator {
    public: 
        virtual glm::vec3 illuminate(Raytracing::HitInfo hitInfo, glm::vec3 eye) = 0;

        glm::vec3 i;    // Intensity
        glm::vec3 ambient;

        SimpleLight(glm::vec3 i, glm::vec3 ambient)
            : i(i), ambient(ambient) {}

};


class PointLight : public SimpleLight {
    public:
        glm::vec3 illuminate(Raytracing::HitInfo hitinfo, glm::vec3 eye) override;
        
        glm::vec3 o;    // Origin
        glm::vec3 attenuation;

        PointLight(glm::vec3 o, glm::vec3 attenuation, glm::vec3 i, glm::vec3 ambient)
            : o(o), attenuation(attenuation), SimpleLight(i, ambient) {}
};


class DirectionLight : public SimpleLight {
    public:
        glm::vec3 illuminate(Raytracing::HitInfo hitinfo, glm::vec3 eye) override;

        glm::vec3 d;    // Direction

        DirectionLight(glm::vec3 d, glm::vec3 i, glm::vec3 ambient)
            : d(d), SimpleLight(i, ambient) {}
};

} // namespace SimpleLights

#endif

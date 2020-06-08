#pragma once

#include<limits>

#include<glm/glm.hpp>

#include "../include/SimpleMaterial.h"

namespace SimpleLights {

class SimpleLight {
    public:
        virtual glm::vec3 illuminate(
            glm::vec3 p,
            glm::vec3 n,
            glm::vec3 eye,
            const SimpleMaterial::Material* material
        ) = 0;

        virtual float getDistance(glm::vec3 p) = 0;
        virtual glm::vec3 getDirection(glm::vec3 p) = 0;

        glm::vec3 i;    // Intensity
        glm::vec3 ambient;

        SimpleLight(glm::vec3 i, glm::vec3 ambient)
            : i(i), ambient(ambient) {}
};


class PointLight : public SimpleLight {
    public:
        glm::vec3 illuminate(
            glm::vec3 p,
            glm::vec3 n,
            glm::vec3 eye,
            const SimpleMaterial::Material* material
        ) override;
        
        glm::vec3 o;    // Origin
        glm::vec3 attenuation;

        glm::vec3 getDirection(glm::vec3 p) override {
            return glm::normalize(this->o - p);
        }

        float getDistance(glm::vec3 p) override {
            return glm::distance(p, this->o);
        }

        PointLight(glm::vec3 o, glm::vec3 attenuation, glm::vec3 i, glm::vec3 ambient)
            : o(o), attenuation(attenuation), SimpleLight(i, ambient) {}
};

class DirectionLight : public SimpleLight {
    public:
        glm::vec3 illuminate(
            glm::vec3 p,
            glm::vec3 n,
            glm::vec3 eye,
            const SimpleMaterial::Material* material
        ) override;

        glm::vec3 d;    // Direction

        glm::vec3 getDirection(glm::vec3 p) override {
            return this->d;
        }

        float getDistance(glm::vec3 p) override {
            return std::numeric_limits<float>::infinity();
        }

        DirectionLight(glm::vec3 d, glm::vec3 i, glm::vec3 ambient)
            : d(glm::normalize(d)), SimpleLight(i, ambient) {}
};

} // namespace SimpleLights

#pragma once

#include<limits>

#include<glm/glm.hpp>

#include "SimpleGraphics/Material.hpp"

namespace SimpleGraphics {

class Light {
    public:
        virtual glm::vec3 illuminate(
            glm::vec3 p,
            glm::vec3 n,
            glm::vec3 eye,
            const ::SimpleGraphics::Material* material
        ) = 0;

        virtual float getDistance(const glm::vec3 p) = 0;
        virtual glm::vec3 getDirection(const glm::vec3 p) = 0;

        glm::vec3 i;    // Intensity
        glm::vec3 ambient;

        Light(glm::vec3 i, glm::vec3 ambient)
            : i(i), ambient(ambient) {}
};


class PointLight : public ::SimpleGraphics::Light {
    public:
        glm::vec3 illuminate(
            glm::vec3 p,
            glm::vec3 n,
            glm::vec3 eye,
            const ::SimpleGraphics::Material* material
        ) override;
        
        glm::vec3 o;    // Origin
        glm::vec3 attenuation;

        glm::vec3 getDirection(const glm::vec3 p) override {
            return glm::normalize(this->o - p);
        }

        float getDistance(const glm::vec3 p) override {
            return glm::distance(p, this->o);
        }

        PointLight(glm::vec3 o, glm::vec3 attenuation, glm::vec3 i, glm::vec3 ambient)
            : o(o), attenuation(attenuation), ::SimpleGraphics::Light(i, ambient) {}
};

class DirectionLight : public ::SimpleGraphics::Light {
    public:
        glm::vec3 illuminate(
            glm::vec3 p,
            glm::vec3 n,
            glm::vec3 eye,
            const ::SimpleGraphics::Material* material
        ) override;

        glm::vec3 d;    // Direction

        glm::vec3 getDirection(const glm::vec3 p) override {
            return this->d;
        }

        float getDistance(const glm::vec3 p) override {
            return std::numeric_limits<float>::infinity();
        }

        DirectionLight(glm::vec3 d, glm::vec3 i, glm::vec3 ambient)
            : d(glm::normalize(d)), ::SimpleGraphics::Light(i, ambient) {}
};

} // namespace SimpleGraphics

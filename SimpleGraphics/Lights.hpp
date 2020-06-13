#pragma once

#include <limits>
#include <sstream>
#include <string>

#include<glm/glm.hpp>

#include "SimpleGraphics/Materials.hpp"

namespace SimpleGraphics {

class Light {
    public:
        virtual glm::vec3 illuminate(
            const glm::vec3 p,
            const glm::vec3 n,
            const glm::vec3 eye,
            const ::SimpleGraphics::Material* material
        ) const = 0;

        virtual float getDistance(const glm::vec3 p) const = 0;
        virtual glm::vec3 getDirection(const glm::vec3 p) const = 0;

        virtual std::string toString() const = 0;

        glm::vec3 i;    // Intensity
        glm::vec3 ambient;

        Light(glm::vec3 i, glm::vec3 ambient)
            : i(i), ambient(ambient) {}
};


class PointLight : public ::SimpleGraphics::Light {
    public:
        glm::vec3 illuminate(
            const glm::vec3 p,
            const glm::vec3 n,
            const glm::vec3 eye,
            const ::SimpleGraphics::Material* material
        ) const override;
        
        glm::vec3 o;    // Origin
        glm::vec3 attenuation;

        glm::vec3 getDirection(const glm::vec3 p) const override {
            return glm::normalize(this->o - p);
        }

        float getDistance(const glm::vec3 p) const override {
            return glm::distance(p, this->o);
        }

        std::string toString() const override {
            std::stringstream ss;
            ss 
                << "SimpleGraphics::PointLight\n"
                << "\tintensity=(" << this->i[0] << ", " << this->i[1] << ", " << this->i[2] << ")\n"
                << "\tambient=(" << this->ambient[0] << ", " << this->ambient[1] << ", " << this->ambient[2] << ")\n"
                << "\torigin=(" << this->o[0] << ", " << this->o[1] << ", " << this->o[2] << ")\n"
                << "\tattenuation=(" << this->attenuation[0] << ", " << this->attenuation[1] << ", " << this->attenuation[2] << ")\n"
            ;
            return ss.str();
        }

        PointLight(glm::vec3 o, glm::vec3 attenuation, glm::vec3 i, glm::vec3 ambient)
            : o(o), attenuation(attenuation), ::SimpleGraphics::Light(i, ambient) {}
};

class DirectionLight : public ::SimpleGraphics::Light {
    public:
        glm::vec3 illuminate(
            const glm::vec3 p,
            const glm::vec3 n,
            const glm::vec3 eye,
            const ::SimpleGraphics::Material* material
        ) const override;

        glm::vec3 d;    // Direction

        glm::vec3 getDirection(const glm::vec3 p) const override {
            return this->d;
        }

        float getDistance(const glm::vec3 p) const override {
            return std::numeric_limits<float>::infinity();
        }

        std::string toString() const override {
            std::stringstream ss;
            ss 
                << "SimpleGraphics::DirectionLight"
                << "\n\tintensity=(" << this->i[0] << ", " << this->i[1] << ", " << this->i[2] << ")"
                << "\n\tambient=(" << this->ambient[0] << ", " << this->ambient[1] << ", " << this->ambient[2] << ")"
                << "\n\tdirection=(" << this->d[0] << ", " << this->d[1] << ", " << this->d[2] << ")"
            ;
            return ss.str();
        }

        DirectionLight(glm::vec3 d, glm::vec3 i, glm::vec3 ambient)
            : d(glm::normalize(d)), ::SimpleGraphics::Light(i, ambient) {}
};

} // namespace SimpleGraphics

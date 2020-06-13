#pragma once

#include <limits>
#include <sstream>
#include <string>

#include<glm/glm.hpp>

#include "SimpleGraphics/Materials.hpp"

namespace SimpleGraphics {

class Light {
    public:
        virtual glm::dvec3 illuminate(
            const glm::dvec3 p,
            const glm::dvec3 n,
            const glm::dvec3 eye,
            const ::SimpleGraphics::Material* material
        ) const = 0;

        virtual double getDistance(const glm::dvec3 p) const = 0;
        virtual glm::dvec3 getDirection(const glm::dvec3 p) const = 0;

        virtual std::string toString() const = 0;

        glm::dvec3 i;    // Intensity
        glm::dvec3 ambient;

        Light(glm::dvec3 i, glm::dvec3 ambient)
            : i(i), ambient(ambient) {}
};


class PointLight : public ::SimpleGraphics::Light {
    public:
        glm::dvec3 illuminate(
            const glm::dvec3 p,
            const glm::dvec3 n,
            const glm::dvec3 eye,
            const ::SimpleGraphics::Material* material
        ) const override;
        
        glm::dvec3 o;    // Origin
        glm::dvec3 attenuation;

        glm::dvec3 getDirection(const glm::dvec3 p) const override {
            return glm::normalize(this->o - p);
        }

        double getDistance(const glm::dvec3 p) const override {
            return glm::distance(this->o, p);
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

        PointLight(glm::dvec3 o, glm::dvec3 attenuation, glm::dvec3 i, glm::dvec3 ambient)
            : o(o), attenuation(attenuation), ::SimpleGraphics::Light(i, ambient) {}
};

class DirectionLight : public ::SimpleGraphics::Light {
    public:
        glm::dvec3 illuminate(
            const glm::dvec3 p,
            const glm::dvec3 n,
            const glm::dvec3 eye,
            const ::SimpleGraphics::Material* material
        ) const override;

        glm::dvec3 d;    // Direction

        glm::dvec3 getDirection(const glm::dvec3 p) const override {
            return this->d;
        }

        double getDistance(const glm::dvec3 p) const override {
            return std::numeric_limits<double>::infinity();
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

        DirectionLight(glm::dvec3 d, glm::dvec3 i, glm::dvec3 ambient)
            : d(glm::normalize(d)), ::SimpleGraphics::Light(i, ambient) {}
};

} // namespace SimpleGraphics

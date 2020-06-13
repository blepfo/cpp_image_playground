#pragma once

#include <cmath>
#include <sstream>
#include <string>
#include <glm/glm.hpp>

namespace SimpleGraphics {

class Material {
    public:
        virtual glm::dvec3 diffuse(const glm::dvec3 p) const = 0;
        virtual glm::dvec3 specular(const glm::dvec3 p) const = 0;
        virtual glm::dvec3 ambient(const glm::dvec3 p) const = 0;
        virtual glm::dvec3 emission(const glm::dvec3 p) const = 0;
        virtual double shiny(const glm::dvec3 p) const = 0;

        virtual std::string toString() const = 0;
};


class StaticMaterial : public ::SimpleGraphics::Material {
	public:
		glm::dvec3 _diffuse;
		glm::dvec3 _specular;
		glm::dvec3 _ambient;
		glm::dvec3 _emission;
        float _shiny;

        glm::dvec3 diffuse(const glm::dvec3 p) const override {
            return this->_diffuse;
        }
        
        glm::dvec3 specular(const glm::dvec3 p) const override {
            return this->_specular;
        }

        glm::dvec3 ambient(const glm::dvec3 p) const override {
            return this->_ambient;
        }

        glm::dvec3 emission(const glm::dvec3 p) const override {
            return this->_emission;
        }

        double shiny (const glm::dvec3 p) const override {
            return this->_shiny;
        }

        std::string toString() const override {
            std::stringstream ss;
            ss
                << "SimpleGraphics::StaticMaterial\n"
                << "\tdiffuse=(" << this->_diffuse[0] << ", " << this->_diffuse[1] << ", " << this->_diffuse[2] << ")\n"
                << "\tspecular=(" << this->_specular[0] << ", " << this->_specular[1] << ", " << this->_specular[2] << ")\n"
                << "\tambient=(" << this->_ambient[0] << ", " << this->_ambient[1] << ", " << this->_ambient[2] << ")\n"
                << "\temission=(" << this->_emission[0] << ", " << this->_emission[1] << ", " << this->_emission[2] << ")\n"
                << "\tshiny=" << this->_shiny
            ;
            return ss.str();
        }

		StaticMaterial(
            glm::dvec3 diffuse, 
            glm::dvec3 specular, 
            glm::dvec3 ambient, 
            glm::dvec3 emission, 
            double shiny
        )
			: _diffuse(diffuse), 
              _specular(specular), 
              _ambient(ambient), 
              _emission(emission), 
              _shiny(shiny) {}
};

class CheckerboardXZ : public ::SimpleGraphics::Material {
    public:
        double scaleX;
        double scaleZ;
        double ambientMultiplier;
        glm::dvec3 colorOdd;
        glm::dvec3 colorEven;
        glm::dvec3 _specular;
        double _shiny;

        bool isEvenTile(const glm::dvec3 p) const {
            int tileX = (int)(std::floor(p[0] * this->scaleX));
            int tileZ = (int)(std::floor(p[2] * this->scaleZ));
            return (((tileX + tileZ) % 2)) == 0;

        }

        glm::dvec3 diffuse(const glm::dvec3 p) const override {
            if (this->isEvenTile(p)) return this->colorEven;
            else return this->colorOdd;
        }

        glm::dvec3 specular(const glm::dvec3 p) const override {
            return this->_specular;
        }

        glm::dvec3 ambient(const glm::dvec3 p) const override {
            if (this->isEvenTile(p)) return (this->colorEven * this->ambientMultiplier);
            else return (this->colorOdd * this->ambientMultiplier);
        }

        glm::dvec3 emission(const glm::dvec3 p) const override {
            return glm::dvec3(0.0f);
        }

        double shiny(const glm::dvec3 p) const override {
            return this->_shiny;
        }

        // TODO - tostring
        std::string toString() const override {
            return "TODO - implement checkerboard tostring";
        }

        CheckerboardXZ(
            double scaleX,
            double scaleZ,
            double ambientMultiplier,
            glm::dvec3 colorOdd,
            glm::dvec3 colorEven,
            glm::dvec3 specular,
            double shiny
        ) : scaleX(scaleX),
            scaleZ(scaleZ),
            ambientMultiplier(ambientMultiplier),
            colorOdd(colorOdd),
            colorEven(colorEven),
            _specular(specular),
            _shiny(shiny) 
        {}
};

} // namespace SimpleMaterial

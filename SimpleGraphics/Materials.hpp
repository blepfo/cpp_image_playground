#pragma once

#include<cmath>
#include<glm/glm.hpp>

namespace SimpleGraphics {

class Material {
    public:
        virtual glm::vec3 diffuse(const glm::vec3 p) const = 0;
        virtual glm::vec3 specular(const glm::vec3 p) const = 0;
        virtual glm::vec3 ambient(const glm::vec3 p) const = 0;
        virtual glm::vec3 emission(const glm::vec3 p) const = 0;
        virtual float shiny(const glm::vec3 p) const = 0;
};


class StaticMaterial : public ::SimpleGraphics::Material {
	public:
		glm::vec3 _diffuse;
		glm::vec3 _specular;
		glm::vec3 _ambient;
		glm::vec3 _emission;
        float _shiny;

        glm::vec3 diffuse(const glm::vec3 p) const override {
            return this->_diffuse;
        }
        
        glm::vec3 specular(const glm::vec3 p) const override {
            return this->_specular;
        }

        glm::vec3 ambient(const glm::vec3 p) const override {
            return this->_ambient;
        }

        glm::vec3 emission(const glm::vec3 p) const override {
            return this->_emission;
        }

        float shiny (const glm::vec3 p) const override {
            return this->_shiny;
        }

		StaticMaterial(
            glm::vec3 diffuse, 
            glm::vec3 specular, 
            glm::vec3 ambient, 
            glm::vec3 emission, 
            float shiny
        )
			: _diffuse(diffuse), 
              _specular(specular), 
              _ambient(ambient), 
              _emission(emission), 
              _shiny(shiny) {}
};

class CheckerboardXZ : public ::SimpleGraphics::Material {
    public:
        float scaleX;
        float scaleZ;
        float ambientMultiplier;
        glm::vec3 colorOdd;
        glm::vec3 colorEven;
        glm::vec3 _specular;
        float _shiny;

        bool isEvenTile(const glm::vec3 p) const {
            int tileX = (int)(std::floor(p[0] * this->scaleX));
            int tileZ = (int)(std::floor(p[2] * this->scaleZ));
            return (((tileX + tileZ) % 2)) == 0;

        }

        glm::vec3 diffuse(const glm::vec3 p) const {
            if (this->isEvenTile(p)) return this->colorEven;
            else return this->colorOdd;
        }

        glm::vec3 specular(const glm::vec3 p) const {
            return this->_specular;
        }

        glm::vec3 ambient(const glm::vec3 p) const {
            if (this->isEvenTile(p)) return (this->colorEven * this->ambientMultiplier);
            else return (this->colorOdd * this->ambientMultiplier);
        }

        glm::vec3 emission(const glm::vec3 p) const {
            return glm::vec3(0.0f);
        }

        float shiny(const glm::vec3 p) const {
            return this->_shiny;
        }

        CheckerboardXZ(
            float scaleX,
            float scaleZ,
            float ambientMultiplier,
            glm::vec3 colorOdd,
            glm::vec3 colorEven,
            glm::vec3 specular,
            float shiny
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

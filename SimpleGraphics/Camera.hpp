#pragma once

#include<cmath>
#include<string>
#include<sstream>

#include <glm/glm.hpp>

// TODO
#include "Raytracing.hpp"

namespace SimpleGraphics {

class Camera {
    public: 
        glm::dvec3 eye;
        glm::dvec3 f;    // Forward
        glm::dvec3 u;    // Up
        glm::dvec3 r;    // Right
        double fovXMultiplier;
        double fovYMultiplier;

        ::Raytracing::Ray viewRay(const double uvX, const double uvY) const;

        std::string toString() const {
            std::stringstream ss;
            ss 
                << "SimpleGraphics::Camera"
                << "\n\teye=(" << this->eye[0] << ", " << this->eye[1] << ", " << this->eye[2] << ")"
                << "\n\tf=(" << this->f[0] << ", " << this->f[1] << ", " << this->f[2] << ")"
            ;
            return ss.str();
        }

        Camera( 
            const glm::dvec3 eye,
            const glm::dvec3 lookAt,
            const glm::dvec3 up, 
            double fovDegrees,
            double aspectRatio
        ) {
            this->eye = eye;
            this->f = glm::normalize(lookAt - eye);
            this->r = glm::normalize(glm::cross(this->f, glm::normalize(up)));
            this->u = glm::cross(this->r, this->f);
            this->fovYMultiplier = tan((fovDegrees * M_PI) / 360.0);
            // Each pixel should be square, so (fovX / fovY) = width / height
            double fovX = fovDegrees * aspectRatio;
            this->fovXMultiplier = fovYMultiplier * aspectRatio;
        }
};

} // namespace SimpleGraphics

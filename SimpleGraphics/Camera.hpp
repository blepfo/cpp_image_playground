#pragma once

#include<string>
#include<sstream>

#include <glm/glm.hpp>

// TODO
#include "Raytracing.hpp"

namespace SimpleGraphics {

class Camera {
    public: 
        glm::vec3 eye;
        glm::vec3 f;    // Forward
        glm::vec3 u;    // Up
        glm::vec3 r;    // Right
        float fovXMultiplier;
        float fovYMultiplier;

        ::Raytracing::Ray viewRay(const float uvX, const float uvY) const;

        std::string toString() const {
            std::stringstream ss;
            ss 
                << "Camera<"
                << " eye=(" << this->eye[0] << " " << this->eye[1] << " " << this->eye[2] << ")"
                << " f=(" << this->f[0] << " " << this->f[1] << " " << this->f[2] << ")"
                << " >"
            ;
            return ss.str();
        }

        Camera( 
            const glm::vec3 eye,
            const glm::vec3 lookAt,
            const glm::vec3 up, 
            float fovDegrees,
            float aspectRatio
        ) {
            this->eye = eye;
            this->f = glm::normalize(lookAt - eye);
            this->r = glm::normalize(glm::cross(this->f, up));
            this->u = glm::cross(this->r, this->f);
            this->fovYMultiplier = tan(glm::radians(fovDegrees / 2.0f));
            // Each pixel should be square, wo fov = (fovY / height) * width
            // Then (fovX / fovY) = width / height
            float fovX = fovDegrees * aspectRatio;
            this->fovXMultiplier = tan(glm::radians(fovX / 2.0f));
        }
};

} // namespace SimpleGraphics

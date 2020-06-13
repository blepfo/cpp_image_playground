#pragma once

#include<glm/glm.hpp>

namespace Transform {
    glm::dmat4 translate(const double x, const double y, const double z); 
    glm::dmat4 scale(const double x, const double y, const double z);
    glm::dmat4 rotate(const glm::dvec3 axis, const double theta);

} // namespace Transform

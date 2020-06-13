#include<cmath>

#include<glm/glm.hpp>

// TODO
#include "Transform.hpp"

namespace Transform {

glm::dmat4 translate(const double x, const double y, const double z) {
    return glm::transpose(glm::dmat4(
        1, 0, 0, x,
        0, 1, 0, y, 
        0, 0, 1, z,
        0, 0, 0, 1
    ));
}

glm::dmat4 scale(const double x, const double y, const double z) {
    return glm::transpose(glm::dmat4(
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1
    ));
}

// Rodriguez rotation formula
glm::dmat4 rotate(const glm::dvec3 axis, const double theta) {
    const glm::dmat3 I = glm::dmat3(1.0);
    const glm::dmat3 crossMat = glm::transpose(glm::dmat3(
        0, -axis[2], axis[1],
        axis[2], 0, -axis[0],
        -axis[1], axis[0], 0
    ));
    const glm::dmat3 dotMat = glm::transpose(glm::dmat3(
        axis[0]*axis[0], axis[0]*axis[1], axis[0]*axis[2],
        axis[1]*axis[0], axis[1]*axis[1], axis[1]*axis[2],
        axis[2]*axis[0], axis[2]*axis[1], axis[2]*axis[2]
    ));
    return (I*cos(theta)) + (crossMat*sin(theta)) + (dotMat * (1.0 - cos(theta)));
}


} // namespace Transform

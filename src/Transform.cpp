#include<cmath>

#include<glm/glm.hpp>

// TODO
#include "Transform.hpp"

namespace Transform {

glm::mat4 translate(const float x, const float y, const float z) {
    return glm::transpose(glm::mat4(
        1, 0, 0, x,
        0, 1, 0, y, 
        0, 0, 1, z,
        0, 0, 0, 1
    ));
}

glm::mat4 scale(const float x, const float y, const float z) {
    return glm::transpose(glm::mat4(
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1
    ));
}

// Rodriguez rotation formula
glm::mat4 rotate(const glm::vec3 axis, const float theta) {
    glm::mat3 I = glm::mat3(1.0f);
    glm::mat3 crossMat = glm::transpose(glm::mat3(
        0, -axis[2], axis[1],
        axis[2], 0, -axis[0],
        -axis[1], axis[0], 0
    ));
    glm::mat3 dotMat = glm::transpose(glm::mat3(
        axis[0]*axis[0], axis[0]*axis[1], axis[0]*axis[2],
        axis[1]*axis[0], axis[1]*axis[1], axis[1]*axis[2],
        axis[2]*axis[0], axis[2]*axis[1], axis[2]*axis[2]
    ));
    return (I*cos(theta)) + (crossMat*sin(theta)) + (dotMat * (1.0f - cos(theta)));
}


} // namespace Transform

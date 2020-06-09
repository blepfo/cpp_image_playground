#pragma once

#include<glm/glm.hpp>

namespace Transform {
    glm::mat4 translate(const float x, const float y, const float z); 
    glm::mat4 scale(const float x, const float y, const float z);
    glm::mat4 rotate(const glm::vec3 axis, const float theta);

} // namespace Transform

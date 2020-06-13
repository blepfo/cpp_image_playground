#pragma once

#include<functional>

#include<glm/glm.hpp>

namespace PixelDraw {

typedef std::function<glm::dvec3 (int, int)> PixelFunc;

/**
 * Apply a function to each pixel in an input image.
 * PixelFunc is passed coordinates in [0, 1] with the origin
 * at bottom left of the image.
 * @param image - 2D array of glm::vec3 representing image RGB
 * @param width - Image width (axis 1 of image)
 * @param height - Image height (axis 0 of image)
 * @param f - Function (uvX, uvY) -> pixelRgb
 * @param numThreads - Number of OMP threads to execute draw loop with
 */
void pixelShade(
    glm::dvec3** image,
    const int width,
    const int height,
    const PixelFunc f,
    const int numThreads=8
);

} // namespace PixelDraw

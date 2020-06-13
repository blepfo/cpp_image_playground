#include <iostream>

#include <omp.h>

#include <glm/glm.hpp>

// TODO
#include "PixelDraw.hpp"

namespace PixelDraw {

void pixelShade(
    glm::dvec3 **image,
    const int width, 
    const int height, 
    const PixelDraw::PixelFunc f,
    const int numThreads
) {
    // Set threads
    omp_set_num_threads(numThreads);
    // Execute draw loop in parallel
    #pragma omp parallel 
    {
        // To get_num_threads, need to be inside parallel region
        #pragma omp single
        {
            printf("PixelDraw::pixelShade start with numThreads=%d\n", omp_get_num_threads());
        }
        #pragma omp for
        {
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    image[y][x] = f(x, y);
                }
            }
        }
    }
}

} // PixelDraw

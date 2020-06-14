#include<chrono>
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
    // Print starttime
    std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
    std::time_t startTime_t = std::chrono::system_clock::to_time_t(startTime);
    std::cout << "PixelDraw::pixelShade start time: " << std::ctime(&startTime_t) << std::endl;

    int numRowsFinished = 0;
    float previousPrintPercent = 0.0f;
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
                #pragma omp atomic
                numRowsFinished++;
                float percentDone = ((float)numRowsFinished / (float)height);
                #pragma omp critical
                {
                    if (percentDone > previousPrintPercent + .05f) {
                        // Print percent + elapsed time
                        std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();
                        std::chrono::duration<float> seconds = currentTime - startTime;
                        float secondsCount = seconds.count();
                        float estRemaining = (secondsCount / percentDone) - secondsCount;
                        printf("PixelDraw::pixelShade Progress - %.1f%% after %.1f seconds -- Est. remaining: %.1f\n", percentDone*100.0f, secondsCount, estRemaining);
                        previousPrintPercent = percentDone;
                    }
                }
            }
        }
    }
    std::chrono::time_point<std::chrono::system_clock> endTime = std::chrono::system_clock::now();
    std::chrono::duration<float> durationSeconds = endTime - startTime;
    std::time_t endTime_t = std::chrono::system_clock::to_time_t(endTime);

    std::cout << "PixelDraw::pixelShade end time: " << std::ctime(&endTime_t) << std::endl;
    std::cout << "PixelDraw::pixelShade duration (seconds): " << durationSeconds.count() << std::endl;





}

} // PixelDraw

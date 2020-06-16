#include<array>
#include<cmath>
#include<iostream>
#include<string>
#include<vector>

#include <glm/glm.hpp>

#include <FreeImage.h>

// TODO
#include "PixelDraw.hpp"
#include "Raytracing.hpp"
#include "SaveUtils.hpp"
#include "Transform.hpp"

#include "SimpleGraphics/Camera.hpp"
#include "SimpleGraphics/Lights.hpp"
#include "SimpleGraphics/Materials.hpp"
#include "SimpleGraphics/Objects.hpp"
#include "SimpleGraphics/Scene.hpp"

int main() {
    int width = 800;
    int height = 600;
    int maxBounces = 5;

    // Initialize image
    std::cout << "Initialize image with shape (" << width << ", " << height << ")" << std::endl;
    glm::dvec3** image;
    image = new glm::dvec3*[height];
    for (int i = 0; i < height; i++) {
        image[i] = new glm::dvec3[width];
    }

    // CAMERA
    SimpleGraphics::Camera camera = SimpleGraphics::Camera(
        glm::dvec3(-5.0, 2.5, 5.0), 
        glm::dvec3(0.0, 1.5, 0.0),
        glm::dvec3(0.0, 1.0, 0.0),
        50.0,
        (double)width / (double)height
    );

    // CREATE SCENE
    SimpleGraphics::Scene scene = SimpleGraphics::Scene();

    // MATERIALS
    SimpleGraphics::StaticMaterial redMat = {
        glm::dvec3(0.9, 0.0, 0.0),    // Diffuse
        glm::dvec3(0.4),                // Specular
        glm::dvec3(0.1, 0.0, 0.0),    // Ambient
        glm::dvec3(0.0, 0.0, 0.0),    // Emission
        128.0                           // Shiny
    };

    SimpleGraphics::StaticMaterial blueMat = {
        glm::dvec3(0.0, 0.1, 0.9),    // Diffuse
        glm::dvec3(0.6),                // Specular
        glm::dvec3(0.1, 0.0, 0.0),    // Ambient
        glm::dvec3(0.0, 0.0, 0.0),    // Emission
        256.0                           // Shiny
    };
    
    SimpleGraphics::StaticMaterial greenMat = {
        glm::dvec3(0.0, 0.8, 0.2),    // Diffuse
        glm::dvec3(0.6),                // Specular
        glm::dvec3(0.1, 0.0, 0.0),    // Ambient
        glm::dvec3(0.0, 0.0, 0.0),    // Emission
        256.0                           // Shiny
    };

    SimpleGraphics::CheckerboardXZ planeMat = {
        2.0, 2.0,         // scaleX, scaleY
        0.1,               // ambientMultiplier
        glm::dvec3(0.9),    // oddColor
        glm::dvec3(0.1),    // evenColor
        glm::dvec3(0.2),   // Specular
        64.0               // Shiny
    };
    
    int redMaterial = scene.addMaterial(&redMat);
    int blueMaterial = scene.addMaterial(&blueMat);
    int greenMaterial = scene.addMaterial(&greenMat);
    int planeMaterial = scene.addMaterial(&planeMat);
    int lightMaterial = scene.addMaterial(&SimpleGraphics::LightMaterial);


    // LIGHTS
    SimpleGraphics::PointLight pointLightRight = {
        glm::dvec3(1.5, 3.4, 0.5),   // Origin
        glm::dvec3(0.0, 0.0, 1.0),    // Attenuation
        glm::dvec3(5.0),                // Intensity
        glm::dvec3(0.0, 0.0, 0.0),    // Ambient
    };
    SimpleGraphics::PointLight pointLightLeft = {
        glm::dvec3(-2.5, 3.0, -0.5),   // Origin
        glm::dvec3(0.0, 0.0, 1.0),     // Attenuation
        glm::dvec3(5.0),                // Intensity
        glm::dvec3(0.0, 0.0, 0.0),    // Ambient
    };

    scene.addVisiblePointLight(&pointLightRight, 0.2, lightMaterial);
    scene.addVisiblePointLight(&pointLightLeft, 0.2, lightMaterial);


    // OBJECTS
    SimpleGraphics::Sphere sphereRightRed = { 
        0.8,                           // Radius
        glm::dvec3(0.7, 1.0, 2.5),    // Center
        redMaterial,                    // Material
    };

    SimpleGraphics::Sphere sphereLeftRed = { 
        0.6,                           // Radius
        glm::dvec3(-1.5, 1.0, -2.0),    // Center
        redMaterial,                    // Material
    };

    SimpleGraphics::Sphere sphereLeftGreen = { 
        0.7,                           // Radius
        glm::dvec3(-2.0, 0.9, 0.75),    // Center
        greenMaterial,                    // Material
    };

    SimpleGraphics::Sphere sphereRightBlue = {
        1.15,
        glm::dvec3(2.0, 1.2, 0.0),
        blueMaterial
    };

    glm::dvec3 fr = glm::dvec3(5.0, 0.0, 5.0);
    glm::dvec3 br = glm::dvec3(5.0, 0.0, -5.0);
    glm::dvec3 fl = glm::dvec3(-5.0, 0.0, 5.0);
    glm::dvec3 bl = glm::dvec3(-5.0, 0.0, -5.0);

    SimpleGraphics::RefTriangle tri1 = {
        &fr, &br, &fl,
        planeMaterial
    };
    SimpleGraphics::RefTriangle tri2 = {
        &fl, &br, &bl,
        planeMaterial
    };

    scene.addObject(&sphereLeftRed);
    scene.addObject(&sphereLeftGreen);
    scene.addObject(&sphereRightRed);
    scene.addObject(&sphereRightBlue);
    scene.addObject(&tri1);
    scene.addObject(&tri2);

    std::cout << scene.toString() << std::endl;

    // Define PixelFunc that raytraces the scene
    const double halfWidth = (double)(width - 1) / 2.0;
    const double halfHeight = (double)(height - 1) / 2.0;
    const double invHalfWidth = 1.0 / halfWidth;
    const double invHalfHeight  = 1.0 / halfHeight;
    PixelDraw::PixelFunc traceFunc = 
        [&camera, &scene, halfWidth, invHalfWidth, halfHeight, invHalfHeight, maxBounces]
        (int x, int y) {
        // Map pixel coords into bi-unit square
        double uvX = ((double)x + 0.5 - halfWidth) * invHalfWidth;
        double uvY = ((double)halfHeight - y + 0.5) * invHalfHeight;
        Raytracing::Ray viewRay = camera.viewRay(uvX, uvY);

        return SimpleGraphics::whittedRayTrace(
            viewRay, 
            scene,
            glm::dvec3(0.0, 0.0, 0.2),
            maxBounces
        );       
    };

    PixelDraw::pixelShade(image, width, height, traceFunc, 8);

    // Save output PPM
    SaveUtils::savePpm(image, width, height, "./shinyball.ppm");
    // Save output PNG
    FreeImage_Initialise();
    SaveUtils::savePng(image, width, height, "./shinyball.png");
    FreeImage_DeInitialise();

    return 0;
}

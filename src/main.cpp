#include<array>
#include<cmath>
#include<iostream>
#include<string>
#include<vector>

#include <glm/glm.hpp>

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

const char* OUTPUT_FILE_PATH = "./test.ppm";

/** 
 *  Assumes uvX and uvY are in bi-unit square
 */
glm::vec3 whittedRayTracePixelFunc(
    const SimpleGraphics::Camera& camera, 
    const SimpleGraphics::Scene& scene,
    const int maxBounces,
    double uvX,
    double uvY
) {
    Raytracing::Ray viewRay = camera.viewRay(uvX, uvY);

    return SimpleGraphics::whittedRayTrace(
        viewRay, 
        scene,
        glm::dvec3(0.0, 0.0, 0.2),
        maxBounces
    );
}


int main() {
    int width = 400;
    int height = 300;
    int maxBounces = 1;

    // Initialize image
    std::cout << "Initialize image with shape (" << width << ", " << height << ")" << std::endl;
    glm::dvec3** image;
    image = new glm::dvec3*[height];
    for (int i = 0; i < height; i++) {
        image[i] = new glm::dvec3[width];
    }

    // CAMERA
    SimpleGraphics::Camera camera = SimpleGraphics::Camera(
        glm::dvec3(-1.5, 0.5, 2.0), 
        glm::dvec3(0.0, 0.0, 0.0),
        glm::dvec3(0.0, 1.0, 0.0),
        60.0,
        (double)width / (double)height
    );

    // CREATE SCENE
    SimpleGraphics::Scene scene = SimpleGraphics::Scene();

    // LIGHTS
    SimpleGraphics::PointLight pointLight1 = {
        glm::dvec3(1.5, 2.0, 0.5),   // Origin
        glm::dvec3(0.0, 0.0, 1.0),    // Attenuation
        glm::dvec3(7.5),                // Intensity
        glm::dvec3(0.0, 0.0, 0.0),    // Ambient
    };
    SimpleGraphics::PointLight pointLight2 = {
        glm::dvec3(-2.5, 3.0, -0.5),   // Origin
        glm::dvec3(0.0, 0.0, 1.0),     // Attenuation
        glm::dvec3(10.0),                // Intensity
        glm::dvec3(0.0, 0.0, 0.0),    // Ambient
    };

    scene.addLight(&pointLight1);
    scene.addLight(&pointLight2);

    // MATERIALS
    SimpleGraphics::StaticMaterial redMat = {
        glm::dvec3(0.9, 0.0, 0.0),    // Diffuse
        glm::dvec3(0.5),                // Specular
        glm::dvec3(0.1, 0.0, 0.0),    // Ambient
        glm::dvec3(0.0, 0.0, 0.0),    // Emission
        128.0                           // Shiny
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
    int planeMaterial = scene.addMaterial(&planeMat);

    // OBJECTS
    SimpleGraphics::Sphere sphere1 = { 
        0.5,                           // Radius
        glm::dvec3(0.0, 0.0, 0.0),    // Center
        redMaterial,                    // Material
        Transform::scale(1.0, 2.0, 1.0)
    };

    SimpleGraphics::Sphere sphere2 = { 
        0.5,                           // Radius
        glm::dvec3(0.0, 0.0, 0.0),    // Center
        redMaterial,                    // Material
        Transform::translate(-0.5, 0.5, -0.7) 
    };

    glm::dmat4 planeTransform = Transform::translate(-2.5, -0.5, 1.0);

    glm::dvec3 fr = glm::dvec3(5.0, 0.0, 0.0);
    glm::dvec3 br = glm::dvec3(5.0, 0.0, -5.0);
    glm::dvec3 fl = glm::dvec3(0.0, 0.0, 0.0);
    glm::dvec3 bl = glm::dvec3(0.0, 0.0, -5.0);

    SimpleGraphics::RefTriangle tri1 = {
        &fr, &br, &fl,
        planeMaterial,
        planeTransform
    };
    SimpleGraphics::RefTriangle tri2 = {
        &fl, &br, &bl,
        planeMaterial,
        planeTransform
    };

    scene.addObject(&sphere1);
    scene.addObject(&sphere2);
    scene.addObject(&tri1);
    scene.addObject(&tri2);

    std::cout << scene.toString() << std::endl;

    const double halfWidth = (double)(width - 1) / 2.0;
    const double halfHeight = (double)(height - 1) / 2.0;
    const double invHalfWidth = 1.0 / halfWidth;
    const double invHalfHeight  = 1.0 / halfHeight;
    PixelDraw::PixelFunc traceFunc = 
        [&camera, &scene, halfWidth, invHalfWidth, halfHeight, invHalfHeight, maxBounces]
        //[&camera, &scene, maxBounces, height, width]
        //(double x, double y) {
        (int x, int y) {
        double uvX = ((double)x + 0.5 - halfWidth) * invHalfWidth;
        double uvY = ((double)halfHeight - y + 0.5) * invHalfHeight;

        return whittedRayTracePixelFunc(
            camera, 
            scene, 
            maxBounces, 
            uvX,
            uvY 
        );
    };

    PixelDraw::pixelShade(image, width, height, traceFunc, 8);

    // Save output image
    std::string ppmString = SaveUtils::rgbToPpm(image, width, height);
    SaveUtils::writePpm(OUTPUT_FILE_PATH, ppmString);
    return 0;
}

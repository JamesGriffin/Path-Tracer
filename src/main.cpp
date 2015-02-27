/*
*  Simple monte carlo path tracer in C++
*
*  Compilation:
*      $ mkdir build
*      $ cd build
*      $ cmake ..
*      $ make
*
*  Usage:
*      $ ./pathtracer <number of samples>
*/

#include <stdio.h>
#include <stdlib.h>

#include "vector.h"
#include "material.h"
#include "objects.h"
#include "camera.h"
#include "scene.h"
#include "renderer.h"


int main(int argc, char *argv[]) {

    time_t start, stop;
    time(&start);               // Start execution timer
    int samples = 4;            // Default samples per pixel

    if (argc == 2) samples = atoi(argv[1]);

    Camera camera = Camera(Vec(0, -5, 2.5), Vec(0,0,1), 1280, 720);     // Create camera
    Scene scene = Scene();                                              // Create scene

    // Add objects to scene
    scene.add( dynamic_cast<Object*>(new Sphere(Vec(0,0,-1000), 1000, Material())) );
    scene.add( dynamic_cast<Object*>(new Sphere(Vec(-1004,0,0), 1000, Material(DIFF, Vec(0.85,0.4,0.4)))) );
    scene.add( dynamic_cast<Object*>(new Sphere(Vec(1004,0,0), 1000, Material(DIFF, Vec(0.4,0.4,0.85)))) );
    scene.add( dynamic_cast<Object*>(new Sphere(Vec(0,1006,0), 1000, Material())) );
    scene.add( dynamic_cast<Object*>(new Sphere(Vec(0,0,110), 100, Material(EMIT, Vec(1,1,1), Vec(2.2,2.2,2.2)))) );
    scene.add( dynamic_cast<Object*>(new Mesh(Vec(), "../obj/dragon_full.obj", Material(DIFF, Vec(0.9, 0.9, 0.9)))) );


    Renderer renderer = Renderer(&scene, &camera);
    renderer.render(samples);
    renderer.save_image("render.ppm");

    // Print duration information
    time(&stop);
    double diff = difftime(stop, start);
    int hrs = (int)diff/3600;
    int mins = ((int)diff/60)-(hrs*60);
    int secs = (int)diff-(hrs*3600)-(mins*60);
    printf("\rRendering (%i samples): Complete!\nTime Taken: %i hrs, %i mins, %i secs\n\n", samples, hrs, mins, secs);
    return 0;
}
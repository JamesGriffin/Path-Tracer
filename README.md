# Path Tracer

Simple monte carlo path tracer in C++.

## Features
 - Mesh Rendering
 - Obj Model Import
 - Textures (PNG only at the moment)
 - Simple naive KD tree for meshes.
 - Diffuse, specular, and emission material types
 
## Compiling and Running
Requires cmake and OpenMP for multithreading.

Compile:
```
$ mkdir build
$ cd build
$ cmake ..
$ make
```
Run:
```
$ ./pathtracer <number of samples>
```
This will render the Stanford Dragon scene which is included.
Take a look at src/main.cpp to see how to create a scene amd import objs.

## Sample Images
![Dabrovic Sponza](doc/example_renders/sponza.png?raw=true "Dabrovic Sponza")

Dabrovic Sponza - http://hdri.cgtechniques.com/~sponza/files/


![Stanford Dragon](doc/example_renders/dragon.png?raw=true "Stanford Dragon")

Stanford Dragon - http://graphics.stanford.edu/data/3Dscanrep


![Stanford Lucy](doc/example_renders/lucy.png?raw=true "Stanford Lucy")

Stanford Lucy  - http://graphics.stanford.edu/data/3Dscanrep

![Cornell Box](doc/example_renders/cornell.png?raw=true "Cornell Box")

Cornell Box

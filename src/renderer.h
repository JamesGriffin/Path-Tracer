#ifndef RENDERER_H
#define RENDERER_H

#include "vector.h"
#include "scene.h"
#include "camera.h"

class Renderer {

private:
    Scene *m_scene;
    Camera *m_camera;
    Vec *m_pixel_buffer;

public:
    Renderer(Scene *scene, Camera *camera);
    void render(int samples=4);
    void save_image(const char * file_path);

};

#endif //RENDERER_H
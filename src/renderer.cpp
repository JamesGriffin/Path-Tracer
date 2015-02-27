#include "renderer.h"

// Clamp double to min/max of 0/1
inline double clamp(double x){ return x<0 ? 0 : x>1 ? 1 : x; }
// Clamp to between 0-255
inline int toInt(double x){ return int(clamp(x)*255+.5); }

Renderer::Renderer(Scene *scene, Camera *camera) {
    m_scene = scene;
    m_camera = camera;
    m_pixel_buffer = new Vec[m_camera->get_width()*m_camera->get_height()];
}

void Renderer::render(int samples) {
    int width = m_camera->get_width();
    int height = m_camera->get_height();
    double samples_recp = 1./samples;

    // Main Loop
    #pragma omp parallel for schedule(dynamic, 1)       // OpenMP
    for (unsigned short y=0; y<height; y++){
        unsigned short Xi[3]={0,0,y*y*y};               // Stores seed for erand48

        fprintf(stderr, "\rRendering (%i samples): %.2f%% ",      // Prints
                samples, (double)y/height*100);                   // progress

        for (int x=0; x<width; x++){
            Vec col = Vec();

            for (int a=0; a<samples; a++){
                Ray ray = m_camera->get_ray(x, y, a>0, Xi);
                col = col + m_scene->trace_ray(ray,0,Xi);
            }

            m_pixel_buffer[(y)*width + x] = col * samples_recp;
        }
    }
}

void Renderer::save_image(const char *file_path) {
    int width = m_camera->get_width();
    int height = m_camera->get_height();

    FILE *f = fopen(file_path, "w");
    fprintf(f, "P3\n%d %d\n%d\n", width, height, 255);
    for (int i=0; i<width*height; i++)
        fprintf(f,"%d %d %d ", toInt(m_pixel_buffer[i].x), toInt(m_pixel_buffer[i].y), toInt(m_pixel_buffer[i].z));
}

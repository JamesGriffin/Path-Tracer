#include "scene.h"
#include "objects.h"

void Scene::add(Object *object) {
    m_objects.push_back( object );
}

ObjectIntersection Scene::intersect(const Ray &ray) {
    ObjectIntersection isct = ObjectIntersection();
    ObjectIntersection temp;
    long size = m_objects.size();

    for (int i=0; i<size; i++){
        temp = m_objects.at((unsigned)i)->get_intersection(ray);

        if (temp.hit) {
            if (isct.u == 0 || temp.u < isct.u) isct = temp;
        }
    }
    return isct;
}

Vec Scene::trace_ray(const Ray &ray, int depth, unsigned short*Xi) {

    ObjectIntersection isct = intersect(ray);

    // If no hit, return world colour
    if (!isct.hit) return Vec();
    /*if (!isct.hit){
        double u, v;
        v = (acos(Vec(0,0,1).dot(ray.direction))/M_PI);
        u = (acos(ray.direction.y)/ M_PI);
        return bg.get_pixel(fabs(u), fabs(v))*1.2;
    }*/

    if (isct.m.get_type() == EMIT) return isct.m.get_emission();
    //Vec x = ray.origin + ray.direction * isct.u;

    Vec colour = isct.m.get_colour();
    //return colour * isct.n.dot((Vec(1,-3,8)-x).norm());

    // Calculate max reflection
    double p = colour.x>colour.y && colour.x>colour.z ? colour.x : colour.y>colour.z ? colour.y : colour.z;

    // Russian roulette termination.
    // If random number between 0 and 1 is > p, terminate and return hit object's emmission
    double rnd = erand48(Xi);
    if (++depth>5){
        if (rnd<p*0.9) { // Multiply by 0.9 to avoid infinite loop with colours of 1.0
            colour=colour*(0.9/p);
        }
        else {
            return isct.m.get_emission();
        }
    }

    Vec x = ray.origin + ray.direction * isct.u;
    Ray reflected = isct.m.get_reflected_ray(ray, x, isct.n, Xi);

    return colour.mult( trace_ray(reflected, depth, Xi) );
}

#include <stdlib.h>

#include "vector.h"
#include "ray.h"
#include "material.h"

Material::Material(MaterialType t, Vec c, Vec e, Texture tex) {
	m_type=t, m_colour=c, m_emission=e;
    m_texture = tex;
}

MaterialType Material::get_type() const { return m_type; }
Vec Material::get_colour() const { return m_colour; }

// Get colour at UV coordinates u,v
Vec Material::get_colour_at(double u, double v) const {
    if (m_texture.is_loaded())
        return m_texture.get_pixel(u, v);

    return m_colour;
}
Vec Material::get_emission() const { return m_emission; }

Ray Material::get_reflected_ray(const Ray &r, Vec &p, const Vec &n,	unsigned short *Xi) const {
	// Ideal specular reflection
	if (m_type == SPEC) {
        double roughness = 0.8;
        Vec reflected = r.direction - n * 2 * n.dot(r.direction);
        reflected = Vec(
            reflected.x + (erand48(Xi)-0.5)*roughness,
            reflected.y + (erand48(Xi)-0.5)*roughness,
            reflected.z + (erand48(Xi)-0.5)*roughness
        ).norm();

        return Ray(p, reflected);
		//return Ray(p, r.direction - n * 2 * n.dot(r.direction));
	}
	// Ideal diffuse reflection
	if (m_type == DIFF) {
		Vec nl=n.dot(r.direction)<0?n:n*-1;
		double r1=2*M_PI*erand48(Xi), r2=erand48(Xi), r2s=sqrt(r2);
        Vec w=nl, u=((fabs(w.x)>.1?Vec(0,1):Vec(1))%w).norm(), v=w%u;
        Vec d = (u*cos(r1)*r2s + v*sin(r1)*r2s + w*sqrt(1-r2)).norm();

        return Ray(p, d);
	}
}
#ifndef AABBOX_H
#define AABBOX_H

#include <cfloat>
#include <algorithm>

#include "vector.h"
#include "ray.h"

// Axis-aligned bounding box
struct AABBox {
    Vec bl;     // Bottom left (min)
    Vec tr;     // Top right   (max)

    AABBox (Vec bl_=Vec(), Vec tr_=Vec()){
        bl=bl_, tr=tr_;
    }

    // Expand to fit box
    void expand(const AABBox &box) {
        if (box.bl.x < bl.x) bl.x = box.bl.x ;
        if (box.bl.y < bl.y) bl.y = box.bl.y;
        if (box.bl.z < bl.z) bl.z = box.bl.z;

        if (box.tr.x > tr.x) tr.x = box.tr.x;
        if (box.tr.y > tr.y) tr.y = box.tr.y;
        if (box.tr.z > tr.z) tr.z = box.tr.z ;
    }

    // Expand to fit point
    void expand(const Vec &vec) {
        if (vec.x < bl.x) bl.x = vec.x ;
        if (vec.y < bl.y) bl.y = vec.y;
        if (vec.z < bl.z) bl.z = vec.z;
    }

    // Returns longest axis: 0, 1, 2 for x, y, z respectively
    int get_longest_axis() {
        Vec diff = tr - bl;
        if (diff.x > diff.y && diff.x > diff.z) return 0;
        if (diff.y > diff.x && diff.y > diff.z) return 1;
        return 2;
    }

    // Check if ray intersects with box. Returns true/false and stores distance in t
    bool intersection(const Ray &r, double &t) {
        double tx1 = (bl.x - r.origin.x)*r.direction_inv.x;
        double tx2 = (tr.x - r.origin.x)*r.direction_inv.x;

        double tmin = std::min(tx1, tx2);
        double tmax = std::max(tx1, tx2);

        double ty1 = (bl.y - r.origin.y)*r.direction_inv.y;
        double ty2 = (tr.y - r.origin.y)*r.direction_inv.y;

        tmin = std::max(tmin, std::min(ty1, ty2));
        tmax = std::min(tmax, std::max(ty1, ty2));

        double tz1 = (bl.z - r.origin.z)*r.direction_inv.z;
        double tz2 = (tr.z - r.origin.z)*r.direction_inv.z;

        tmin = std::max(tmin, std::min(tz1, tz2));
        tmax = std::min(tmax, std::max(tz1, tz2));
        t = tmin;

        return tmax >= tmin;
    }
};

#endif // AABBOX_H
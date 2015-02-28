#include <vector>

#include "vector.h"
#include "ray.h"
#include "triangle.h"
#include "kdtree.h"

// Build KD tree for tris
KDNode* KDNode::build(std::vector<Triangle*> &tris, int depth){
    KDNode* node = new KDNode();
    node->leaf = false;
    node->triangles = std::vector<Triangle*>();
    node->left = NULL;
    node->right = NULL;
    node->box = AABBox();

    if (tris.size() == 0) return node;

    if (depth > 25 || tris.size() <= 6) {
        node->triangles = tris;
        node->leaf = true;
        node->box = tris[0]->get_bounding_box();

        for (long i=1; i<tris.size(); i++) {
            node->box.expand(tris[i]->get_bounding_box());
        }

        node->left = new KDNode();
        node->right = new KDNode();
        node->left->triangles = std::vector<Triangle*>();
        node->right->triangles = std::vector<Triangle*>();

        return node;
    }

    node->box = tris[0]->get_bounding_box();
    Vec midpt = Vec();
    double tris_recp = 1.0/tris.size();

    for (long i=1; i<tris.size(); i++) {
        node->box.expand(tris[i]->get_bounding_box());
        midpt = midpt + (tris[i]->get_midpoint() * tris_recp);
    }

    std::vector<Triangle*> left_tris;
    std::vector<Triangle*> right_tris;
    int axis = node->box.get_longest_axis();

    for (long i=0; i<tris.size(); i++) {
        switch (axis) {
            case 0:
                midpt.x >= tris[i]->get_midpoint().x ? right_tris.push_back(tris[i]) : left_tris.push_back(tris[i]);
                break;
            case 1:
                midpt.y >= tris[i]->get_midpoint().y ? right_tris.push_back(tris[i]) : left_tris.push_back(tris[i]);
                break;
            case 2:
                midpt.z >= tris[i]->get_midpoint().z ? right_tris.push_back(tris[i]) : left_tris.push_back(tris[i]);
                break;
        }
    }

    if (tris.size() == left_tris.size() || tris.size() == right_tris.size()) {
        node->triangles = tris;
        node->leaf = true;
        node->box = tris[0]->get_bounding_box();

        for (long i=1; i<tris.size(); i++) {
            node->box.expand(tris[i]->get_bounding_box());
        }

        node->left = new KDNode();
        node->right = new KDNode();
        node->left->triangles = std::vector<Triangle*>();
        node->right->triangles = std::vector<Triangle*>();

        return node;
    }

    node->left = build(left_tris, depth+1);
    node->right = build(right_tris, depth+1);

    return node;
}

// Finds nearest triangle in kd tree that intersects with ray.
bool KDNode::hit(KDNode *node, const Ray &ray, double &t, double &tmin, Vec &normal, Vec &c) {
    double dist;
    if (node->box.intersection(ray, dist)){
        if (dist > tmin) return false;

        bool hit_tri = false;
        bool hit_left = false;
        bool hit_right = false;
        long tri_idx;

        if (!node->leaf) {
            //if ( node->left->triangles.size() > 0 )
                hit_left = hit(node->left, ray, t, tmin, normal, c);

            //if ( node->right->triangles.size() > 0 )
                hit_right = hit(node->right, ray, t, tmin, normal, c);

            return hit_left || hit_right;
        }
        else {
            long triangles_size = node->triangles.size();
            for (long i=0; i<triangles_size; i++) {
                if (node->triangles[i]->intersect(ray, t, tmin, normal)){
                    hit_tri = true;
                    tmin = t;
                    tri_idx = i;
                }
            }
            if (hit_tri) {
                Vec p = ray.origin + ray.direction * tmin;
                c = node->triangles[tri_idx]->get_colour_at(p);
                return true;
            }
        }
    }
    return false;
}
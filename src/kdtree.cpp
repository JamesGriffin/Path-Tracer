#include "stdlib.h"
#include <vector>

#include "vector.h"
#include "ray.h"
#include "triangle.h"
#include "kdtree.h"


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

        //printf("%i\n", tris.size());

        return node;
    }

    node->box = tris[0]->get_bounding_box();
    Vec midpt = Vec();
    double tris_recp = 1.0/tris.size();

    for (long i=1; i<tris.size(); i++) {
        node->box.expand(tris[i]->get_bounding_box());
        midpt = midpt + (tris[i]->get_midpoint() * tris_recp);
    }

    //printf("%f %f %f | %f %f %f", node->box.bl.x, node->box.bl.y, node->box.bl.x, node->box.tr.x, node->box.tr.y, node->box.tr.z);

    /*Vec midpt = Vec();
    for (long i=0; i<tris.size(); i++) {
        midpt = midpt + (tris[i]->get_midpoint() * (1.0/tris.size()));
    }*/

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

        //printf("%i\n", tris.size());

        return node;
    }

    node->left = build(left_tris, depth+1);
    node->right = build(right_tris, depth+1);

    //if (left_tris.size() == 0 && right_tris.size() > 0) left_tris = right_tris;
    //if (right_tris.size() == 0 && left_tris.size() > 0) right_tris = left_tris;

    /*int matches = 0;
    for (long i=0; i<left_tris.size(); i++) {
        for (long j=0; j<right_tris.size(); j++) {
            if (left_tris[i] == right_tris[j])
                matches++;
        }
    }*/


    //if (depth < 20 /*|| ((float)matches / left_tris.size() < 0.5 && (float)matches / right_tris.size() < 0.5)*/) {
    // ^ Old termination criteria ^

    /*if (true && (left_tris.size() > 2 || right_tris.size() > 2)) {    // TODO: improve termination criteria
        node->left = build(left_tris, depth+1);
        node->right = build(right_tris, depth+1);
    }
    else {
        node->triangles = tris;
        printf("%i\n", node->triangles.size());
        node->leaf = true;
        node->left = new KDNode();
        node->right = new KDNode();
        node->left->triangles = std::vector<Triangle*>();
        node->right->triangles = std::vector<Triangle*>();
        //printf("left tris: %i right tris: %i \n", left_tris.size(), right_tris.size());
    }*/

    return node;
}

bool KDNode::hit(KDNode *node, const Ray &ray, double &t, double &tmin, Vec &normal, Vec &c) {
    double dist;
    if (node->box.intersection(ray, dist)){
        if (dist > tmin) return false;

        bool hit_tri = false;
        bool hit_left = false;
        bool hit_right = false;
        long tri_idx;

        /*if ( node->left->triangles.size() > 0 || node->right->triangles.size() > 0 ) {*/
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
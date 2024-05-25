/*
 * @path src/octree/tree.cpp
 * @file tree.cpp
*/

#include "../include/octree/tree.hpp"

Tree::Tree(Bound bound) : root(bound, 0) {}

Tree::~Tree() {}

void Tree::insert(Body* body) {
    root.insert(body);
}

void Tree::calculate_center_of_mass() {
    root.calculate_center_of_mass();
}

void Tree::calculate_force(Body& body, float theta, float gravity, const double softening_factor) const {
    root.calculate_force(body, theta, gravity, softening_factor);
}

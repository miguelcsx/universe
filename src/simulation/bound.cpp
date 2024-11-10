/*
 * @path src/simulation/bound.cpp
 * @file bound.cpp
*/

#include "simulation/bound.hpp"

Bound::Bound(glm::vec3 center, float half_width)
    : center(center), half_width(half_width) {}


bool Bound::contains(const Body& body) const {
    return body.position.x >= center.x - half_width && body.position.x <= center.x + half_width &&
           body.position.y >= center.y - half_width && body.position.y <= center.y + half_width &&
           body.position.z >= center.z - half_width && body.position.z <= center.z + half_width;
}

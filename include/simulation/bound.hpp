/*
 * @path include/simulation/bound.hpp
 * @file bound.hpp
*/

#ifndef BOUND_HPP
#define BOUND_HPP

#include <glm/glm.hpp>
#include "body.hpp"

struct Bound {
    const glm::vec3 center;
    const float half_width;

    Bound(glm::vec3 center, float half_width);

    [[nodiscard]] bool contains(const Body& body) const;
};

#endif // BOUND_HPP
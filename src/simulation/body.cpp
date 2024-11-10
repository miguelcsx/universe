/*
 * @path src/simulation/body.cpp
 * @file body.cpp
*/

#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include "simulation/body.hpp"

Body::Body(u_int id, float mass, const glm::vec3 position, const glm::vec3 velocity, const glm::vec3 color)
    : id(id), mass(mass), position(position), velocity(velocity), color(color) {}


void Body::apply_force(glm::vec3 new_position, float new_mass, float gravity, float softening) {
    const glm::vec3 r = new_position - position;
    const float r_length = glm::length(r);
    const float r_squared = r_length * r_length + softening * softening;

    // Calculate the gravitational force
    const float magnitude = (gravity * (mass * new_mass)) / r_squared;
    const float inv_r_squared = 1.0f / r_squared;

    glm::vec3 force = magnitude * r / r_length;

    forces += force;
}

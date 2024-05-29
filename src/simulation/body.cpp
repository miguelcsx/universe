/*
 * @path src/simulation/body.cpp
 * @file body.cpp
*/

#include "../include/simulation/body.hpp"


Body::Body(u_int id, float mass, const glm::vec3 position, const glm::vec3 velocity, const glm::vec3 color)
    : id(id), mass(mass), position(position), velocity(velocity), color(color) {}


void Body::apply_force(glm::vec3 new_position, float new_mass, float gravity, float softening) {
    const glm::vec3 r = new_position - position;
    const float r_length = glm::length(r);
    const float r_squared = r_length * r_length;

    // Prevent division by zero and handle softening
    const float denom = r_squared + softening * softening;

    // Calculate the gravitational force
    const float maginitude = (gravity * mass * new_mass) / denom;

    const glm::vec3 force = (maginitude / r_length) * r;

    forces += force;
}

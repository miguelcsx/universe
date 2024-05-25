/*
 * @path src/simulation/body.cpp
 * @file body.cpp
*/

#include "../include/simulation/body.hpp"


Body::Body(u_int id, float mass, const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& color)
    : id(id), mass(mass), position(position), velocity(velocity), color(color) {}


void Body::apply_force(glm::vec3 new_position, float new_mass, float gravity, float softening) {
    const glm::vec3 r = new_position - position;
    const float r_length = glm::length(r);
    float r_length_sqrt = r_length * r_length;
    float force_magnitude = (gravity * mass * new_mass) / (r_length_sqrt + softening);
    glm::vec3 force = force_magnitude * glm::normalize(r);
    forces += force;
}

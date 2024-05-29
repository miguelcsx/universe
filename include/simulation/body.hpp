/*
 * @path include/simulation/body.hpp
 * @file body.hpp 
*/

#ifndef BODY_HPP
#define BODY_HPP

#include <glm/glm.hpp>

constexpr int MAX_DEPTH = 1000;
constexpr int CAPACITY = 1;

struct Body {
    const u_int id;
    float mass;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 forces;
    glm::vec3 color;

    explicit Body(u_int id): id(id), mass(1.0f), position(glm::vec3(0.0f)), velocity(glm::vec3(0.0f)), forces(glm::vec3(0.0f)), color(glm::vec3(1.0f, 1.0f, 1.0f)) {}

    Body(u_int id, float mass, const glm::vec3 position, const glm::vec3 velocity, const glm::vec3 color);

    void apply_force(glm::vec3 new_position, float new_mass, float gravity, float softening);
};

#endif // BODY_HPP

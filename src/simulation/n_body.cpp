/*
 * @path src/simulation/n_body.cpp
 * @file n_body.cpp
*/

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "simulation/n_body.hpp"

void NBody::update_model_matrix() {
    model_matrix = glm::mat4(1.0f);
    model_matrix = glm::translate(model_matrix, position);
    model_matrix = glm::rotate(model_matrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model_matrix = glm::rotate(model_matrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model_matrix = glm::rotate(model_matrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model_matrix = glm::scale(model_matrix, scale);
}

void NBody::set_paused(bool paused) {
    this->paused = paused;
}

[[maybe_unused]] bool NBody::is_paused() const {
    return paused;
}

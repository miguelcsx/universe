/*
 * @path include/simulation/n_body.hpp
 * @file n_body.hpp
*/

#ifndef N_BODY_HPP
#define N_BODY_HPP

#include <vector>
#include <string>
#include <random>
#include <glm/glm.hpp>
#include "../include/graphics/shader.hpp"

class NBody {
    public:
        float gravity = 1.0f;
        float softening_factor = 10.0f;
        float radius = 3.0f;
        float theta = 1.0f;
        float interaction_percentage = 1.0f;
        float damping = 0.995f;
        float mass = 1.5f;

        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

        NBody() = default;
        NBody(const NBody&) = delete;
        NBody(NBody&&) = delete;
        NBody& operator=(const NBody&) = delete;
        NBody& operator=(NBody&&) noexcept = delete;

        virtual ~NBody() = default;

        virtual void update(const float& delta_time) = 0;
        virtual void render(glm::mat4 view, glm::mat4 view_projection) = 0;
        virtual void reset() = 0;
        void update_model_matrix();

        void set_paused(bool paused);
        [[nodiscard]] bool is_paused() const;

        [[nodiscard]] virtual size_t get_body_count() const = 0;
        virtual void set_body_count(const size_t& body_count) = 0;

    protected:
        glm::mat4 model_matrix;
        bool paused = false;
};

#endif // N_BODY_HPP

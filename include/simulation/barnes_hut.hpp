/*
 * @path include/simulation/barnes_hut.hpp
 * @file barnes_hut.hpp
*/

#ifndef BARNES_HUT_HPP
#define BARNES_HUT_HPP

#include<vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "../octree/tree.hpp"
#include "../simulation/n_body.hpp"


class BarnesHut : public NBody {
    public:
        explicit BarnesHut(int bodies_count = 10000);
        BarnesHut(const BarnesHut&) = delete;
        BarnesHut(BarnesHut&&) = delete;
        BarnesHut& operator=(const BarnesHut&) = delete;
        BarnesHut& operator=(BarnesHut&&) = delete;
        ~BarnesHut() override;

        void update(const float& delta_time) final;
        void render(glm::mat4 view, glm::mat4 view_projection) final;
        void reset() final;
        void clear();
        void randomize();

        [[nodiscard]] size_t get_body_count() const final;
        void set_body_count(const size_t& body_count) final;

    private:
        std::vector<Body> bodies;

        static const char* const vertex_shader;
        static const char* const fragment_shader;

        Shader shader;
        GLuint vao, vbo;
};

#endif // BARNES_HUT_HPP
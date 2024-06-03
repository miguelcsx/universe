/*
 * @path src/simulation/barnes_hut.cpp
 * @file barnes_hut.cpp
*/

#include <random>
#include <iostream>
#include <openacc.h>
#include "../include/simulation/barnes_hut.hpp"

const char* const BarnesHut::vertex_shader = 
    R"(
        #version 300 es
        precision highp float;
        layout(location = 0) in vec3 a_position;
        layout(location = 1) in vec3 a_velocity;
        layout(location = 2) in vec3 a_color;

        uniform mat4 u_view_projection;
        out vec3 v_color;

        void main(void) {
            gl_Position = u_view_projection * vec4(a_position, 1.0);
            v_color = a_color;
            gl_PointSize = 2.0f;
        }
    )";

const char* const BarnesHut::fragment_shader = 
    R"(
        #version 300 es
        precision highp float;
        in vec3 v_color;
        out vec4 frag_color;

        void main(void) {
            frag_color = vec4(v_color, 1.0f);
        }
    )";

BarnesHut::BarnesHut(int bodies_count) :
    shader(vertex_shader, fragment_shader, false) {
        // Resize the bodies vector
        set_body_count(bodies_count);

        // Generate the VAO and VBO
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        // Bind the VAO and VBO
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(bodies.size() * sizeof(Body)), bodies.data(), GL_DYNAMIC_DRAW);

        // Set the vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Body), (void*)offsetof(Body, position));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Body), (void*)offsetof(Body, velocity));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Body), (void*)offsetof(Body, color));
        glEnableVertexAttribArray(2);

        // Unbind the VAO and VBO
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

BarnesHut::~BarnesHut() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void BarnesHut::update(const float& delta_time) {
    Tree octree(Bound(glm::vec3(0.0f), 10.0f));
    // Update the bodies
    #pragma acc parallel loop
    for (auto& body : bodies) {
        octree.insert(&body);
    }

    octree.calculate_center_of_mass();

    #pragma acc parallel loop
    for (auto& body : bodies) {
        octree.calculate_force(body, theta, gravity, softening_factor);
        const glm::vec3 acceleration = body.forces / body.mass;

        // Apply periodic boundary conditions
        body.position += body.velocity * delta_time;
        body.velocity += acceleration * delta_time;
        body.velocity *= damping;

        // Reset the forces
        body.forces = glm::vec3(0.0f);
    }
}

void BarnesHut::render(glm::mat4 view, glm::mat4 view_projection) {
    // Bind the vertex array and buffer
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Set the vbo data
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(bodies.size() * sizeof(Body)), bodies.data(), GL_DYNAMIC_DRAW);

    // Use the shader
    shader.use();
    shader.set_mat4("u_view_projection", view * view_projection);

    // Draw the bodies
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(bodies.size()));

    // Unbind the vertex array and buffer
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BarnesHut::reset() {
    randomize();
}

void BarnesHut::randomize() {
    // Initialize the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> random_angle(0.0f, static_cast<float>(2.0 * M_PI));
    std::uniform_real_distribution<float> random_color(0.0f, 1.0f);
    std::uniform_int_distribution<int> random_id(0, 100);

    // Initialize the bodies
    #pragma acc parallel loop
    for (auto& body : bodies) {
        const float phi = random_angle(gen);
        const float theta = random_angle(gen);
        int rad = random_id(gen);
        const float x = rad * std::cos(phi) * std::sin(theta);
        const float y = rad * std::sin(phi) * std::sin(theta);
        const float z = rad * std::cos(theta);
        body.position += glm::vec3(x, y, z); // update position
        body.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
        body.color = glm::vec3(random_color(gen), random_color(gen), random_color(gen));
        body.mass = mass;
    }
}

void BarnesHut::clear() {
    bodies.clear();
}

size_t BarnesHut::get_body_count() const {
    return bodies.size();
}

void BarnesHut::set_body_count(const size_t& body_count) {
    clear();

    #pragma acc parallel loop
    for (int i = 0; i < body_count; i++) {
        bodies.emplace_back(i);
    }

    randomize();
}

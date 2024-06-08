/*
 * @path src/octree/node.cpp
 * @file node.cpp
*/

#include <omp.h>
#include <openacc.h>
#include "../include/octree/node.hpp"

Node::Node(Bound bound, int depth) :
    children({}),
    bodies({}),
    bound(bound),
    is_leaf(true),
    depth(depth),
    center_of_mass(),
    total_mass(0.0f) {}


Node::~Node() {
    #pragma acc parallel loop
    for (auto* child : children) {
            delete child;
    }
}

bool Node::insert(Body* body) {
    if (!bound.contains(*body)) {
        return false;
    }

    if (is_leaf) {
        if (bodies.size() < CAPACITY || depth >= MAX_DEPTH) {
            bodies.push_back(body);
            return true;
        }
        subdivide();
    }

    #pragma acc parallel loop
    for (auto* child : children) {
        #pragma acc loop seq
        for (int i = 0; i < 8; ++i) {
            if (child->insert(body)) {
                return true;
            }
        }
    }

    return false;
}

void Node::subdivide() {
    is_leaf = false;
    float new_half_width = bound.half_width * 0.5f;
    const auto& center = bound.center;

    std::array<glm::vec3, 8> offsets = {
        glm::vec3(-new_half_width, -new_half_width, -new_half_width),
        glm::vec3(new_half_width, -new_half_width, -new_half_width),
        glm::vec3(-new_half_width, new_half_width, -new_half_width),
        glm::vec3(new_half_width, new_half_width, -new_half_width),
        glm::vec3(-new_half_width, -new_half_width, new_half_width),
        glm::vec3(new_half_width, -new_half_width, new_half_width),
        glm::vec3(-new_half_width, new_half_width, new_half_width),
        glm::vec3(new_half_width, new_half_width, new_half_width)
    };

    #pragma acc parallel loop
    for (int i = 0; i < 8; ++i) {
        children[i] = new Node(Bound(center + offsets[i], new_half_width), depth + 1);
    }

    std::vector<Body*> bodies_to_reinsert = bodies;
    bodies.clear();

    #pragma acc parallel loop
    for (Body* body : bodies_to_reinsert) {
        insert(body);
    }
}


void Node::calculate_center_of_mass() {
    if (!is_leaf) {
        center_of_mass = glm::vec3(0.0f);
        total_mass = 0.0f;

        #pragma acc parallel loop reduction(+:center_of_mass,total_mass)
        for (Node* child : children) {
            if (child) {
                child->calculate_center_of_mass();
                center_of_mass += child->center_of_mass * child->total_mass;
                total_mass += child->total_mass;
            }
        }

        if (total_mass > 0) {
            center_of_mass /= total_mass;
        }
    } else if (!bodies.empty()) {
        center_of_mass = glm::vec3(0.0f);
        total_mass = 0.0f;

        #pragma acc parallel loop reduction(+:center_of_mass,total_mass)
        for (Body* body : bodies) {
            center_of_mass += body->position * body->mass;
            total_mass += body->mass;
        }

        if (total_mass > 0) {
            center_of_mass /= total_mass;
        }
    } else {
        center_of_mass = glm::vec3(0.0f);
        total_mass = 0.0f;
    }
}

void Node::calculate_force(Body& body, float theta, float gravity, const double softening_factor) const {
    glm::vec3 r = center_of_mass - body.position;
    float distance = glm::length(r);
    float size = bound.half_width * 2.0f;

    if (is_leaf && bodies.size() == 1 && bodies[0]->id == body.id) {
        return; // Do not calculate force on itself
    }

    if (size / distance < theta) {
        body.apply_force(center_of_mass, total_mass, gravity, softening_factor);
    } else if (!is_leaf) {
        #pragma acc parallel loop present(children[:8])
        for (int i = 0; i < 8; ++i) {
            Node* child = children[i];
            if (child) {
            child->calculate_force(body, theta, gravity, softening_factor);
            }
        }
    } else {
        #pragma acc parallel loop present(bodies[:bodies.size()])
        for (int i = 0; i < bodies.size(); ++i) {
            const Body* other = bodies[i];
            if (body.id != other->id) {
                body.apply_force(other->position, other->mass, gravity, softening_factor);
            }
        }
    }
}

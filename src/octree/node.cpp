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
        if (this->bodies.size() < CAPACITY || depth >= MAX_DEPTH) {
            this->bodies.push_back(body);
            return true;
        }
        this->subdivide();
    }

    return (
        children[0]->insert(body) ||
        children[1]->insert(body) ||
        children[2]->insert(body) ||
        children[3]->insert(body) ||
        children[4]->insert(body) ||
        children[5]->insert(body) ||
        children[6]->insert(body) ||
        children[7]->insert(body)
    );
}

void Node::subdivide() {
    is_leaf = false;
    float new_half_width = bound.half_width / 2.0f;
    const auto x = bound.center.x;
    const auto y = bound.center.y;
    const auto z = bound.center.z;

    std::array<glm::vec3, 8> offsets = {
        glm::vec3(-new_half_width, -new_half_width, -new_half_width),
        glm::vec3(new_half_width, -new_half_width, -new_half_width),
        glm::vec3(-new_half_width, new_half_width, -new_half_width),
        glm::vec3(-new_half_width, -new_half_width, new_half_width),
        glm::vec3(new_half_width, -new_half_width, new_half_width),
        glm::vec3(-new_half_width, new_half_width, new_half_width),
        glm::vec3(new_half_width, new_half_width, -new_half_width),
        glm::vec3(new_half_width, new_half_width, new_half_width)
    };

    #pragma acc parallel loop
    for (int i = 0; i < 8; ++i) {
        children[i] = new Node(Bound(glm::vec3(x + offsets[i].x, y + offsets[i].y, z + offsets[i].z), new_half_width), depth + 1);
    }

    #pragma acc parallel loop collapse(2)
    for (Body* body : bodies) {
        for (auto* child : children) {
            child->insert(body);
        }
    }

    bodies.clear();

    this->is_leaf = false;
}


void Node::calculate_center_of_mass() {
    if (!this->is_leaf) {
        #pragma acc parallel loop
        for (Node* child : children) {
            child->calculate_center_of_mass();
            this->center_of_mass += child->center_of_mass * child->total_mass;
            this->total_mass += child->total_mass;
        }
        this->center_of_mass /= this->total_mass;
    }
    else if (!this->bodies.empty()) {
        #pragma acc parallel loop
        for (Body* b : bodies) {
            this->total_mass += b->mass;
            this->center_of_mass += b->position;
        }
        this->center_of_mass /= this->bodies.size();
    }
    else {
        this->center_of_mass = glm::vec3(0.0f);
        this->total_mass = 0.0f;
    }
}

void Node::calculate_force(Body& body, float theta, float gravity, const double softening_factor, float cutoff_threshold) const {
    const float distance = glm::distance(body.position, bound.center);
    const float size = bound.half_width * 2.0f;

    if (size / distance < theta) {
        body.apply_force(this->bound.center, this->total_mass, gravity, softening_factor);
    }
    else {
        if (!this->is_leaf) {
            #pragma acc parallel loop
            for (const Node* child : children) {
                child->calculate_force(body, theta, gravity, softening_factor, cutoff_threshold);
            }
        }
        else {
            #pragma acc parallel loop
            for (const Body* other : this->bodies) {
                if (body.id != other->id) {
                    body.apply_force(other->position, other->mass, gravity, softening_factor);
                }
            }
        }
    }
}

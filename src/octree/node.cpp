/*
 * @path src/octree/node.cpp
 * @file node.cpp
*/

#include <omp.h>
#include <openacc.h>
#include "../include/octree/node.hpp"

Node::Node(Bound bound, int depth) :
    children({nullptr}),
    bodies({}),
    bound(bound),
    is_leaf(true),
    depth(depth),
    center_of_mass(),
    total_mass(0.0f) {}


Node::~Node() {
    for (Node* child : children) {
        if (child != nullptr) {
            delete child;
        }
        child = nullptr;
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

    #pragma acc parallel loop
    for (int i = 0; i < 8; ++i) {
        if (children[i]->insert(body)) {
            return true;
        }
    }

    return false;
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

    #pragma omp parallel for
    for (int i = 0; i < 8; ++i) {
        children[i] = new Node(Bound(glm::vec3(x + offsets[i].x, y + offsets[i].y, z + offsets[i].z), new_half_width), depth + 1);
    }

    #pragma omp parallel for
    for (Body* body : bodies) {
        for (Node* child : children) {
            if(child->bound.contains(*body)) {
                child->insert(body);
                break;
            }

        }
    }

    bodies.clear();

    this->is_leaf = false;
}


void Node::calculate_center_of_mass() {
    if (!this->is_leaf) {
        #pragma omp parallel for
        for (Node* child : children) {
            child->calculate_center_of_mass();
            this->center_of_mass += child->center_of_mass * child->total_mass;
            this->total_mass += child->total_mass;
        }
    }
    else if (!this->bodies.empty()) {
        #pragma omp parallel for
        for (Body* b : bodies) {
            center_of_mass += b->position * b->mass;
            total_mass += b->mass;
        }
        this->center_of_mass /= this->bodies.size();
    }
    else {
        center_of_mass = glm::vec3(0.0f);
        total_mass = 0.0f;
    }
}

void Node::calculate_force(Body& body, float theta, float gravity, const double softening_factor, float cutoff_threshold) const {
    const float distance = glm::distance(body.position, bound.center);
    const float size = bound.half_width * 2.0f;

    if (size / distance < theta || (size / distance) < cutoff_threshold) {
        body.apply_force(this->bound.center, this->total_mass, gravity, softening_factor);
    }
    else {
        if (!this->is_leaf) {
            #pragma omp parallel for
            for (const Node* child : children) {
                child->calculate_force(body, theta, gravity, softening_factor, cutoff_threshold);
            }
        }
        else {
            #pragma omp parallel for
            for (const Body* other : bodies) {
                if (body.id != other->id) {
                    body.apply_force(other->position, other->mass, gravity, softening_factor);
                }
            }
        }
    }
}

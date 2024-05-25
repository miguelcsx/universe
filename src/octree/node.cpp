/*
 * @path src/octree/node.cpp
 * @file node.cpp
*/

#include "../include/octree/node.hpp"

Node::Node(Bound bound, int depth) :
    children({}),
    bodies({}),
    bound(bound),
    is_leaf(true),
    depth(depth),
    center_of_mass(glm::vec3(0.0f)),
    total_mass(0.0f) {}


Node::~Node() {
    for (Node* child : children) {
        delete child;
    }
}

bool Node::insert(Body* body) {
    if (!bound.contains(*body)) {
        return false;
    }

    if (is_leaf) {
        if (bodies.size() < 2) {
            bodies.push_back(body);
            return true;
        } else {
            subdivide();
            is_leaf = false;
        }
    }

    for (Node* child : children) {
        if (child->insert(body)) {
            return true;
        }
    }

    return false;
}

void Node::subdivide() {
    float half_width = bound.half_width / 2.0f;
    glm::vec3 center = bound.center;

    children[0] = new Node(Bound(center + glm::vec3(-half_width, half_width, -half_width), half_width), depth + 1);
    children[1] = new Node(Bound(center + glm::vec3(half_width, half_width, -half_width), half_width), depth + 1);
    children[2] = new Node(Bound(center + glm::vec3(-half_width, half_width, half_width), half_width), depth + 1);
    children[3] = new Node(Bound(center + glm::vec3(half_width, half_width, half_width), half_width), depth + 1);
    children[4] = new Node(Bound(center + glm::vec3(-half_width, -half_width, -half_width), half_width), depth + 1);
    children[5] = new Node(Bound(center + glm::vec3(half_width, -half_width, -half_width), half_width), depth + 1);
    children[6] = new Node(Bound(center + glm::vec3(-half_width, -half_width, half_width), half_width), depth + 1);
    children[7] = new Node(Bound(center + glm::vec3(half_width, -half_width, half_width), half_width), depth + 1);

    for (Body* body : bodies) {
        for (Node* child : children) {
            child->insert(body);
        }
    }

    bodies.clear();

    this->is_leaf = false;
}


void Node::calculate_center_of_mass() {
    if (!is_leaf) {
        for (Node* child : children) {
            child->calculate_center_of_mass();
            center_of_mass += child->center_of_mass;
            total_mass += child->total_mass;
        }
    }
    else if (!bodies.empty()) {
        for (Body* body : bodies) {
            center_of_mass += body->position * body->mass;
            total_mass += body->mass;
        }
        center_of_mass /= bodies.size();
    }
    else {
        center_of_mass = glm::vec3(0.0f);
        total_mass = 0.0f;
    }
}

void Node::calculate_force(Body& body, float theta, float gravity, const double softening_factor) const {
    const float distance = glm::distance(body.position, bound.center);
    const float size = bound.half_width * 2.0f;

    if (size / distance < theta) {
        body.apply_force(center_of_mass, total_mass, gravity, softening_factor);
    }

    else {
        if (!is_leaf) {
            for (const Node* child : children) {
                child->calculate_force(body, theta, gravity, softening_factor);
            }
        }
        else {
            for (const Body* other : bodies) {
                if (body.id != other->id) {
                    body.apply_force(other->position, other->mass, gravity, softening_factor);
                }
            }
        }
    }
}

/*
 * @path include/octree/node.hpp
 * @file node.hpp
*/

#ifndef NODE_HPP
#define NODE_HPP

#include <array>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "../simulation/bound.hpp"
#include "../simulation/body.hpp"

struct Node {
    public:
        explicit Node(Bound bound, int depth);

        Node(const Node& node) = delete;
        Node(Node&& node) noexcept = delete;
        Node& operator=(const Node& node) = delete;
        Node& operator=(Node&& node) noexcept = delete;

        ~Node();

        bool insert(Body* body);
        void subdivide();
        void calculate_center_of_mass();
        void calculate_force(Body& body, float theta, float gravity, const double softening_factor, float threshold) const;

    private:
        std::array<Node*, 8> children;
        std::vector<Body*> bodies;
        const Bound bound;
        bool is_leaf;
        u_int depth;

        glm::vec3 center_of_mass;
        float total_mass;

        friend class Tree;
};

#endif // NODE_HPP

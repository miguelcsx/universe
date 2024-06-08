/*
 * @path include/octree/tree.hpp
 * @file tree.hpp
*/

#ifndef TREE_HPP
#define TREE_HPP

#include <vector>
#include "node.hpp"

class Tree {
    public:
        explicit Tree(Bound bound);

        void insert(Body* body);
        void calculate_center_of_mass();
        void calculate_force(Body& body, float theta, float gravity, const double softening_factor) const;

    private:
        Node root;
};

#endif // TREE_HPP

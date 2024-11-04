#ifndef __TREE_FIXER_HPP__
#define __TREE_FIXER_HPP__

#include "expression_node.hpp"

#include <memory>

class TreeFixer
{   
    typedef std::shared_ptr<ExpressionNode> nodePtr;
public:

    static void checkTree(nodePtr node);
    
    static void checkChildren(nodePtr node);
    static nodePtr simplify(nodePtr node);
    static bool treesEqual(const std::shared_ptr<ExpressionNode>& tree1, 
                            const std::shared_ptr<ExpressionNode>& tree2);
};

#endif // __TREE_FIXER_HPP__
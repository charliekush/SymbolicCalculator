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
};

#endif // __TREE_FIXER_HPP__
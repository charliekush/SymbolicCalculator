#ifndef __TREE_MODIFIER_HPP__
#define __TREE_MODIFIER_HPP__
#include "expression_node.hpp"

#include <memory>

class TreeModifier
{
    typedef std::shared_ptr<ExpressionNode> nodePtr;
    static void removeNode(nodePtr& node);
public:
    
    void swapNodes(nodePtr node1, nodePtr node2);
    static nodePtr expandNegative(nodePtr node);
    static void replaceWithLeftChild(nodePtr& node);
    static void replaceWithRightChild(nodePtr& node);
    static void replaceWithRightChild(nodePtr& parent, nodePtr& node);
};
#endif // __TREE_MODIFIER_HPP__
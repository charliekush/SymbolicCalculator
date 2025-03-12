#ifndef __OPERATION_HPP__
#define __OPERATION_HPP__

#include "expression_node.hpp"

#include <memory>

class Operation
{
    typedef std::shared_ptr<ExpressionNode> nodePtr;
    
public:
    static nodePtr times(nodePtr left, nodePtr right);
    static nodePtr divide(nodePtr left, nodePtr right);
    static nodePtr add(nodePtr left, nodePtr right);
    static nodePtr subtract(nodePtr left, nodePtr right);
    static nodePtr power(nodePtr left, nodePtr right);
};
#endif // __OPERATION_HPP__
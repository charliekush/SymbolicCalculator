#ifndef __CALCULATOR_HPP__
#define __CALCULATOR_HPP__

#include "token.hpp"
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

class Arithmetic
{
    typedef std::shared_ptr<ExpressionNode> nodePtr;
    typedef std::shared_ptr<Number> numPtr;
    typedef std::function<double(double, double)> operation;
public:
    static numPtr performOperation(const Operation& op, numPtr left, 
                                numPtr right, bool isDivision);
    static numPtr power(nodePtr operatorNode, numPtr left, numPtr right);
    static numPtr multiply(nodePtr operatorNode, numPtr left, numPtr right);
    static numPtr divide(nodePtr operatorNode, numPtr left, numPtr right);
    static numPtr add(nodePtr operatorNode, numPtr left, numPtr right);
    static numPtr subtract(nodePtr operatorNode, numPtr left, numPtr right);
    static void simplify(nodePtr operatorNode, numPtr left, numPtr right);
    
    static void simplifyExponent(nodePtr operatorNode);
    static void simplifyMultiplication(nodePtr operatorNode);
    static void simplifyDivision(nodePtr operatorNode);
    static void simplifyAddition(nodePtr operatorNode);
    static void simplifySubtraction(nodePtr operatorNode);

    static numPtr getNumberToken(const nodePtr& node);
    static void setNodeToZero(nodePtr operatorNode);
    static void setNodeToOne(nodePtr operatorNode);
    
    
};




class TreeFixer
{   
    typedef std::shared_ptr<ExpressionNode> nodePtr;
public:
    static void checkTree(nodePtr node);
    static nodePtr expandNegative(nodePtr node);
    static void checkChildren(nodePtr node);
};
#endif // __CALCULATOR_HPP__
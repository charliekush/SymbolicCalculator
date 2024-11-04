#ifndef __CALCULATOR_HPP__
#define __CALCULATOR_HPP__

#include "token.hpp"
#include "expression_node.hpp"

#include <memory>
#include <functional>

class Arithmetic
{
    typedef std::shared_ptr<ExpressionNode> nodePtr;
    typedef std::shared_ptr<Number> numPtr;
    typedef std::function<double(double, double)> operation;
    
public:
    static bool floatSimplification;
    static numPtr performOperation(const operation& op, numPtr left, 
                                numPtr right, bool isDivision);
    static numPtr power(nodePtr operatorNode, numPtr left, numPtr right);
    static numPtr multiply(nodePtr operatorNode, numPtr left, numPtr right);
    static numPtr divide(nodePtr operatorNode, numPtr left, numPtr right);
    static numPtr add(nodePtr operatorNode, numPtr left, numPtr right);
    static numPtr subtract(nodePtr operatorNode, numPtr left, numPtr right);
    static void simplify(nodePtr operatorNode);
    
    static void simplifyExponent(nodePtr& operatorNode);
    static void simplifyMultiplication(nodePtr& operatorNode);
    static void simplifyDivision(nodePtr& operatorNode);
    static void simplifyAddition(nodePtr& operatorNode);
    static void simplifySubtraction(nodePtr& operatorNode);

    static numPtr getNumberToken(const nodePtr& node);
    static void setNodeToZero(nodePtr& operatorNode);
    static void setNodeToOne(nodePtr& operatorNode);

};





#endif // __CALCULATOR_HPP__
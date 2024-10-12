#ifndef __DERIVATIVE_HPP__
#define __DERIVATIVE_HPP__

#include "expression_tree.hpp"
#include "expression_node.hpp"

#include <memory>
class Derivative
{
    typedef std::shared_ptr<ExpressionNode> nodePtr;
private:
    ExpressionTree tree;
    std::shared_ptr<Variable> diffVar;
public:
    Derivative(std::string input, std::string wrt);

    /**
     * @brief calculates the derivative of the entire tree
     * 
     */
    void solve();

    /**
     * @brief calculates the derivative from a node
     * 
     */
    nodePtr solve(nodePtr node);


    /**
     * @brief takes a power rule of a node
     * 
     */
    nodePtr powerRule(nodePtr root);
    nodePtr productRule(nodePtr root);
    nodePtr quotientRule(nodePtr root);
    nodePtr chainRule(nodePtr root);
    nodePtr simplify(nodePtr node);

    nodePtr times(nodePtr left, nodePtr right);
    nodePtr divide(nodePtr left, nodePtr right);
    nodePtr add(nodePtr left, nodePtr right);
    nodePtr subtract(nodePtr left, nodePtr right);
    nodePtr power(nodePtr left, nodePtr right);


    void checkChildren(nodePtr node);
    void solveChildren(nodePtr node);

    
};




#endif // __DERIVATIVE_HPP__
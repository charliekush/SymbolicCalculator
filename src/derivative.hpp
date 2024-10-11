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
    void solve();
    nodePtr solve(nodePtr node);
    nodePtr powerRule(nodePtr root);
    nodePtr productRule(nodePtr root);
    nodePtr quotientRule(nodePtr root);
    nodePtr chainRule(nodePtr root);
    nodePtr simplify(nodePtr node);
    void checkChildren(nodePtr node);

    
};




#endif // __DERIVATIVE_HPP__
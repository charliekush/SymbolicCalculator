#ifndef __DERIVATIVE_HPP__
#define __DERIVATIVE_HPP__


#include "expression_node.hpp"
#include "log.hpp"

#include <memory>
class Derivative
{
    typedef std::shared_ptr<ExpressionNode> nodePtr;
private:
    nodePtr root;
    std::shared_ptr<Variable> diffVar;
public:
    Logger log;
    Derivative(std::string input, std::string wrt);
    Derivative(nodePtr root, std::shared_ptr<Variable> wrt);
    /**
     * @brief calculates the derivative of the entire tree
     * 
     */
    nodePtr solve();

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
    

    


    void checkChildren(nodePtr node);
    void solveChildren(nodePtr node);

    
};




#endif // __DERIVATIVE_HPP__
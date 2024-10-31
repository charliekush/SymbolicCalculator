#ifndef __APPROX_HPP__
#define __APPROX_HPP__

#include "token.hpp"
#include "expression_node.hpp"

#include <memory>
#include <string>
#include <utility>
class Approx
{
private:
    typedef std::shared_ptr<ExpressionNode> nodePtr;
    typedef std::shared_ptr<Number> numPtr;
    nodePtr root;
    nodePtr derivative;
    double value;
    std::shared_ptr<Variable> diffVar;
    std::shared_ptr<Token> replaceToken(nodePtr node);
public:
    Approx(std::string raw_input, std::string diffVar, double value);
    
    std::pair<double,double> approximate();
    double approximate(nodePtr root);
    
};


#endif //                

#include "derivative.hpp"
#include "token.hpp"
#include "tokenizer.hpp"
#include "postfix.hpp"
#include "lookup.hpp"
#include "calculator.hpp"

#include <stdexcept>
#include <cmath>

Derivative::Derivative(std::string input, std::string wrt)
{
    Tokenizer parser(input);
    Tokenizer diffVarParser(wrt);
    auto diffVarParsed = parser.tokenize();
    if (diffVarParsed.size() != 1)
    {
        std::string errMsg = "Invalid differentiating variable, ";
        errMsg += "must be one variable, but parsed output has size ";
        errMsg += std::to_string(diffVarParsed.size()) + ": " +
            diffVarParsed.toString() + "\n";
        std::runtime_error(errMsg.c_str());
    }
    this->diffVar = std::dynamic_pointer_cast<Variable>(diffVarParsed[0]);
    if (diffVar->getType() != TokenType::VARIABLE)
    {
        std::string errMsg = "Invalid differentiating variable, ";
        errMsg += "input must be a variable, but parsed token has type ";
        errMsg += Lookup::getTokenType(diffVar->getType()) + ": " +
            diffVar->getFullStr() + "\n";
        std::runtime_error(errMsg.c_str());
    }

    ShuntingYard converter(parser.tokenize());
    this->tree = ExpressionTree(converter.getPostfix());

}


void Derivative::solve()
{
    this->solve(this->tree.root);
}

std::shared_ptr<ExpressionNode> Derivative::solve(
                nodePtr node)
{
    if (!node->hasVariable(this->diffVar))
    {
        node->removeLeftChild();
        node->removeRightChild();
        node->setDerivative(std::make_shared<ExpressionNode>(
                                std::make_shared<Number>("0",0)));
    }

    return 0;
}

void Derivative::checkChildren(nodePtr node)
{
    if (!node->getLeft())
    {
        std::string errMsg = "The node " + node->getToken()->getFullStr() +
            "has no left child\n";
        std::runtime_error(errMsg.c_str());
    }
    if (!node->getRight())
    {
        std::string errMsg = "The node " + node->getToken()->getFullStr() +
            "has no right child\n";
        std::runtime_error(errMsg.c_str());
    }
    if (node->getRight()->getType() < node->getLeft()->getType() && 
        node->getType() == TokenType::OPERATOR )
    {
        if (node->getToken()->isCommutative())
        {
            node->swapChildren();
        }
    }
    
}




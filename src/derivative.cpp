
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
    this->simplify(this->tree.root);
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

std::shared_ptr<ExpressionNode> Derivative::simplify(nodePtr node)
{
    if (node->getType()  == TokenType::OPERATOR)
    {
        this->simplify(node->getLeft());
        this->simplify(node->getRight());
        if (node->getStr() == "^")
        {
            Arithmetic::simplifyExponent(node);
        }
        else if (node->getStr() == "*")
        {
            Arithmetic::simplifyMultiplication(node);
        }
        else if (node->getStr() == "/")
        {
            Arithmetic::simplifyDivision(node);
        }
        else if (node->getStr() == "+")
        {
            Arithmetic::simplifyAddition(node);
        }
        else if (node->getStr() == "-")
        {
            Arithmetic::simplifySubtraction(node);
        }
    }
    if (node->getType()  == TokenType::FUNCTION)
    {
        auto func = std::dynamic_pointer_cast<Function>(node->getToken());
        nodePtr newRoot = this->simplify(func->getSubExprTree()->root);
        auto funcTree = std::make_shared<ExpressionTree>(newRoot);
        func->setSubExprTree(funcTree);
    }
    return node;
}


std::shared_ptr<ExpressionNode> Derivative::times(nodePtr left, nodePtr right)
{
    auto opToken = std::make_shared<Operator>("*");
    auto node = std::make_shared<ExpressionNode>(opToken);
    node->setLeft(left);
    node->setRight(right);
    this->checkChildren(node);
    return node;
}
std::shared_ptr<ExpressionNode> Derivative::divide(nodePtr left, nodePtr right)
{
    auto opToken = std::make_shared<Operator>("/");
    auto node = std::make_shared<ExpressionNode>(opToken);
    node->setLeft(left);
    node->setRight(right);
    this->checkChildren(node);
    return node;
}
std::shared_ptr<ExpressionNode> Derivative::add(nodePtr left, nodePtr right)
{
    auto opToken = std::make_shared<Operator>("+");
    auto node = std::make_shared<ExpressionNode>(opToken);
    node->setLeft(left);
    node->setRight(right);
    this->checkChildren(node);
    return node;
}
std::shared_ptr<ExpressionNode> Derivative::subtract(nodePtr left, 
                                                            nodePtr right)
{
    auto opToken = std::make_shared<Operator>("-");
    auto node = std::make_shared<ExpressionNode>(opToken);
    node->setLeft(left);
    node->setRight(right);
    this->checkChildren(node);
    return node;
}
std::shared_ptr<ExpressionNode> Derivative::power(nodePtr left, 
                                                            nodePtr right)
{
    auto opToken = std::make_shared<Operator>("^");
    auto node = std::make_shared<ExpressionNode>(opToken);
    node->setLeft(left);
    node->setRight(right);
    this->checkChildren(node);
    return node;
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


void Derivative::solveChildren(nodePtr node)
{
    
    solve(node->getLeft());
    solve(node->getRight()); 
}
std::shared_ptr<ExpressionNode> Derivative::powerRule(nodePtr node)
{
    this->checkChildren(node);
    this->solveChildren(node);  

    nodePtr base = node->getLeft();
    nodePtr exponent = node->getRight();

    bool baseContainsVar = base->hasVariable(this->diffVar);
    bool exponentContainsVar = exponent->hasVariable(this->diffVar);

    // Case 1: Base contains variable, exponent is constant
    if (baseContainsVar && !exponentContainsVar)
    {
        // Apply the basic power rule: d/dx [f(x)^a] = a * f(x)^(a-1) * f'(x)
        nodePtr one = std::make_shared<ExpressionNode>(
                    std::make_shared<Number>("1", 1)); \
        nodePtr exponentMinusOne = subtract(exponent, one);

        
        node->setDerivative(times(exponent, power(base, exponentMinusOne)));
    }
    // Case 2: Base is constant, exponent contains variable
    else if (!baseContainsVar && exponentContainsVar)
    {
        // Chain rule: d/dx [a^f(x)] = a^f(x) * ln(a) * f'(x)

        auto lnFunc = std::make_shared<Function>("ln");
        lnFunc->setSubExprTree(base);
        auto lnBase = std::make_shared<ExpressionNode>(lnFunc);  

        // Set the derivative using chain rule
        node->setDerivative(times(power(base, exponent), 
                        times(lnBase, exponent->getDerivative())));
    }
    // Case 3: Both base and exponent contain the variable
    else if (baseContainsVar && exponentContainsVar)
    {
        // Generalized power rule: d/dx [f(x)^g(x)]
        // = f(x)^g(x) * [g'(x) * ln(f(x)) + f'(x) * g(x) / f(x)]
        auto lnFunc = std::make_shared<Function>("ln");
        lnFunc->setSubExprTree(base);
        auto lnBase = std::make_shared<ExpressionNode>(lnFunc);

         // f'(x) * g(x) / f(x)
        nodePtr baseDerivativeTerm = divide(times(base->getDerivative(), 
                                    exponent), base); 
        // g'(x) * ln(f(x))
        nodePtr exponentDerivativeTerm = times(exponent->getDerivative(), 
                                                                    lnBase);  

        nodePtr totalDerivative = add(baseDerivativeTerm, 
                                        exponentDerivativeTerm);
        node->setDerivative(times(power(base, exponent), totalDerivative));
    }
    return node->getDerivative();
}

void Derivative::solveChildren(nodePtr node)
{
    
    solve(node->getLeft());
    solve(node->getRight()); 
}


std::shared_ptr<ExpressionNode> Derivative::productRule(nodePtr node)
{
    this->checkChildren(node);
    this->solveChildren(node);

    // Starting with d/dx u*v
    nodePtr u = node->getLeft();
    nodePtr v = node->getRight();

    bool uContainsVar = u->hasVariable(this->diffVar);
    bool vContainsVar = v->hasVariable(this->diffVar);
    
    if (uContainsVar && vContainsVar)
    {
        node->setDerivative(add(times(u,v->getDerivative()),
                                times(u->getDerivative(),v)));
    }
    else if (uContainsVar)
    {
        node->setDerivative(times(u->getDerivative(), v));
    }
    else if (vContainsVar)
    {
        node->setDerivative(times(u, v->getDerivative()));
    }
    return node->getDerivative();
}

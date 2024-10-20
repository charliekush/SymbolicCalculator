
#include "derivative.hpp"
#include "token.hpp"
#include "tokenizer.hpp"
#include "postfix.hpp"
#include "lookup.hpp"
#include "calculator.hpp"
#include "lookup.hpp"
#include "latex_converter.hpp"
#include <iostream>
#include <stdexcept>
#include <cmath>

Derivative::Derivative(std::string input, std::string wrt)
{
    Tokenizer parser(input);
    Tokenizer diffVarParser(wrt);
    auto diffVarParsed = diffVarParser.tokenize();
    if (diffVarParsed.size() != 1)
    {
        std::string errMsg = "Invalid differentiating variable, ";
        errMsg += "must be one variable, but parsed output has size ";
        errMsg += std::to_string(diffVarParsed.size()) + ": " +
            diffVarParsed.toString() + "\n";
        throw std::runtime_error(errMsg.c_str());
    }
    this->diffVar = std::dynamic_pointer_cast<Variable>(diffVarParsed[0]);
    if (diffVar->getType() != TokenType::VARIABLE)
    {
        std::string errMsg = "Invalid differentiating variable, ";
        errMsg += "input must be a variable, but parsed token has type ";
        errMsg += Lookup::getTokenType(diffVar->getType()) + ": " +
            diffVar->getFullStr() + "\n";
        throw std::runtime_error(errMsg.c_str());
    }

    ShuntingYard converter(parser.tokenize());
    this->root = ExpressionNode::buildTree(converter.getPostfix());
    TreeFixer::checkTree(this->root);
}




std::shared_ptr<ExpressionNode> Derivative::solve()
{
    TreeFixer::checkTree(this->root);
    this->simplify(this->root);
    auto derivative = this->solve(this->root);
    return this->simplify(derivative);
}

std::shared_ptr<ExpressionNode> Derivative::solve(nodePtr node)
{
    if (node->getDerivative())
    {
        return node->getDerivative();
    }
    if (!node->hasVariable(this->diffVar))
    {
        node->setDerivative(std::make_shared<ExpressionNode>(
            std::make_shared<Number>("0", 0)));
    }
    else if (node->getType() == TokenType::VARIABLE)
    {
        node->setDerivative(std::make_shared<ExpressionNode>(
            std::make_shared<Number>("1", 1)));
    }
    else if (node->getType() == TokenType::FUNCTION)
    {
        auto original = std::dynamic_pointer_cast<Function>(node->getToken());
        auto subExprDerivative = this->solve(
                original->getSubExprTree()->copyTree());
        node->setDerivative(subExprDerivative);
        auto funcIter = Lookup::functionLookup.find(node->getStr());
        if (funcIter != Lookup::functionLookup.end())
        {
            auto func = funcIter->second;

            // Set up the function with the expression node
            func->update(node);

            // Example usage: get the derivative and evaluate
            //std::string nodeStr = LaTeXConverter::convertToLaTeX(node);
            auto deriv = func->getDerivative();
            TreeFixer::checkTree(deriv);
            node->setDerivative(deriv);
            //std::string derivStr = LaTeXConverter::convertToLaTeX(deriv);
            //std::cout << "Derivative of " << nodeStr << ": " << derivStr<< "\n\n";
            
        }
        
    }
    else if (node->getType() == TokenType::OPERATOR)
    {
        this->solveChildren(node);
        if (node->getStr() == "^")
        {
            this->powerRule(node);
        }
        else if (node->getStr() == "*")
        {
            this->productRule(node);
        }
        else if (node->getStr() == "/")
        {
            this->quotientRule(node);
        }
        else if (node->getStr() == "+")
        {
            node->setDerivative(Operation::add(node->getLeft()->getDerivative(),
                node->getRight()->getDerivative()));
        }
        else if (node->getStr() == "-")
        {
            node->setDerivative(Operation::subtract(
                node->getLeft()->getDerivative(),
                node->getRight()->getDerivative()));
        }
        TreeFixer::checkTree(node->getDerivative());
    }
    return node->getDerivative();
}

std::shared_ptr<ExpressionNode> Derivative::simplify(nodePtr node)
{
    if (node->getType() == TokenType::OPERATOR)
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
    if (node->getType() == TokenType::FUNCTION)
    {
        auto func = std::dynamic_pointer_cast<Function>(node->getToken());
        nodePtr newSubRoot = this->simplify(func->getSubExprTree());

        func->setSubExprTree(newSubRoot);
    }
    return node;
}






void Derivative::solveChildren(nodePtr node)
{
    if (node->getLeft())
    {
        solve(node->getLeft());
    }
    if (node->getRight())
    {
        solve(node->getRight());
    }
    
}

std::shared_ptr<ExpressionNode> Derivative::powerRule(nodePtr node)
{
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
            nodePtr exponentMinusOne = Operation::subtract(exponent, one);


        node->setDerivative(Operation::times(exponent, Operation::power(
            base, exponentMinusOne)));
    }
    // Case 2: Base is constant, exponent contains variable
    else if (!baseContainsVar && exponentContainsVar)
    {
        // Chain rule: d/dx [a^f(x)] = a^f(x) * ln(a) * f'(x)

        auto lnFunc = std::make_shared<Function>("ln");
        lnFunc->setSubExprTree(base);
        auto lnBase = std::make_shared<ExpressionNode>(lnFunc);

        // Set the derivative using chain rule
        node->setDerivative(Operation::times(Operation::power(base, exponent),
            Operation::times(lnBase, exponent->getDerivative())));
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
        nodePtr baseDerivativeTerm = Operation::divide(Operation::times(
            base->getDerivative(), exponent), base);
        // g'(x) * ln(f(x))
        nodePtr exponentDerivativeTerm = Operation::times(
                                    exponent->getDerivative(), lnBase);

        nodePtr totalDerivative = Operation::add(baseDerivativeTerm,
                                        exponentDerivativeTerm);
        node->setDerivative(Operation::times(Operation::power(base, exponent),
            totalDerivative));
    }
    return node->getDerivative();
}




std::shared_ptr<ExpressionNode> Derivative::productRule(nodePtr node)
{
    this->solveChildren(node);

    // Starting with d/dx u*v
    nodePtr u = node->getLeft();
    nodePtr v = node->getRight();

    bool uContainsVar = u->hasVariable(this->diffVar);
    bool vContainsVar = v->hasVariable(this->diffVar);

    if (uContainsVar && vContainsVar)
    {
        node->setDerivative(Operation::add(Operation::times(u, v->getDerivative()),
            Operation::times(u->getDerivative(), v)));
    }
    else if (uContainsVar)
    {
        node->setDerivative(Operation::times(u->getDerivative(), v));
    }
    else if (vContainsVar)
    {
        node->setDerivative(Operation::times(u, v->getDerivative()));
    }
    return node->getDerivative();
}


std::shared_ptr<ExpressionNode> Derivative::quotientRule(nodePtr node)
{
    this->solveChildren(node);

    // Starting with d/dx [u/v]
    nodePtr u = node->getLeft();
    nodePtr v = node->getRight();

    bool uContainsVar = u->hasVariable(this->diffVar);
    bool vContainsVar = v->hasVariable(this->diffVar);
    // v * u' - u * v'
    nodePtr numerator = Operation::subtract(Operation::times(v,
        u->getDerivative()), Operation::times(u, v->getDerivative()));
    // v^2
    nodePtr denominator = Operation::power(v,
        std::make_shared<ExpressionNode>(std::make_shared<Number>("2", 2)));

    // d/dx [u/v] = (v * u' - u * v') / (v^2)
    node->setDerivative(Operation::divide(numerator, denominator));
    return node->getDerivative();
}


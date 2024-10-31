
#include "derivative.hpp"
#include "token.hpp"
#include "tokenizer.hpp"
#include "postfix.hpp"
#include "arithmetic.hpp"
#include "lookup.hpp"
#include "latex_converter.hpp"
#include "operation.hpp"
#include "tree_fixer.hpp"

#include <iostream>
#include <stdexcept>
#include <cmath>

Derivative::Derivative(std::string input, std::string wrt) : log(false)
{
    log.setInput(input);
    log.setMode("Derivative");
    
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
    auto parsed = parser.tokenize();
    ShuntingYard converter(parsed);
    auto postfix = converter.getPostfix();
    this->root = ExpressionNode::buildTree(postfix);
    TreeFixer::checkTree(this->root);
    TreeFixer::simplify(this->root);
}

Derivative::Derivative(nodePtr root, std::shared_ptr<Variable> wrt) : log(false)
{
    this->diffVar = wrt;
    this->root = root->copyTree();
    TreeFixer::checkTree(this->root);
    TreeFixer::simplify(this->root);
}



std::shared_ptr<ExpressionNode> Derivative::solve()
{
    TreeFixer::checkTree(this->root);
    TreeFixer::simplify(this->root);
    //this->root->printTree();
    auto derivative = this->solve(this->root);
    
    TreeFixer::simplify(derivative);
    //derivative->printTree();
    log.setOutput(derivative);
    return derivative;
}

std::shared_ptr<ExpressionNode> Derivative::solve(nodePtr node)
{
    if (node->getDerivative())
    {
        
        return node->getDerivative();
    }
    if (!node->hasVariable(this->diffVar))
    {
        //std::cout << "The expression " <<  LaTeXConverter::convertToLaTeX(node)
        //<< " does not contain the variable, setting derivative to zero\n";
        node->setDerivative(std::make_shared<ExpressionNode>(
            std::make_shared<Number>("0", 0)));
        
    }
    else if (node->getType() == TokenType::VARIABLE)
    {
        node->setDerivative(std::make_shared<ExpressionNode>(
            std::make_shared<Number>("1", 1)));
        //std::cout << "The expression " <<  LaTeXConverter::convertToLaTeX(node)
        //<< " is just the variable wrt, setting derivative to 1\n";
    }
    else if (node->getType() == TokenType::FUNCTION)
    {
        auto original = std::dynamic_pointer_cast<Function>(node->getToken());
        auto subTree = original->getSubExprTree()->copyTree();
        auto subExprDerivative = this->solve(subTree);
        node->setDerivative(subExprDerivative->copyTree());
        auto funcIter = Lookup::functionLookup.find(node->getStr());
        if (funcIter != Lookup::functionLookup.end())
        {
            auto func = funcIter->second;

             
            func->update(node);

            
            auto deriv = func->getDerivative();
            TreeFixer::checkTree(deriv);
            node->setDerivative(deriv);
            
            
        }
        log.logChainRule(node, subExprDerivative);
        /*std::cout << "\nDerivative of "
            << LaTeXConverter::convertToLaTeX(node)
            << " using chain rule is "
            << LaTeXConverter::convertToLaTeX(node->getDerivative()) 
            << "\n";*/
        
    }
    else if (node->getType() == TokenType::OPERATOR)
    {
        this->solveChildren(node);
        if (node->getStr() == "^")
        {
            
            this->powerRule(node);
            log.logPowerRule(node);
                
        }
        else if (node->getStr() == "*")
        {
            this->productRule(node);
            log.logProductRule(node);
            /*std::cout << "\nDerivative of "
                << LaTeXConverter::convertToLaTeX(node)
                << " using productRule is "
                << LaTeXConverter::convertToLaTeX(node->getDerivative()) 
                << "\n";*/
        }
        else if (node->getStr() == "/")
        {
            this->quotientRule(node);
            log.logQuotientRule(node);
            /*std::cout << "\nDerivative of "
                << LaTeXConverter::convertToLaTeX(node)
                << " using quotientRule is "
                << LaTeXConverter::convertToLaTeX(node->getDerivative()) 
                << "\n";*/
        }
        else if (node->getStr() == "+")
        {
            node->setDerivative(Operation::add(node->getLeft()->getDerivative(),
                node->getRight()->getDerivative()));
            log.logAddition(node);
        }
        else if (node->getStr() == "-")
        {
            node->setDerivative(Operation::subtract(
                node->getLeft()->getDerivative(),
                node->getRight()->getDerivative()));
            log.logSubtraction(node);
        }
        TreeFixer::checkTree(node->getDerivative());
        TreeFixer::simplify(node->getDerivative());
    }
    return node->getDerivative();
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
    

    nodePtr base = node->getLeft();
    nodePtr exponent = node->getRight();

    bool baseContainsVar = base->hasVariable(this->diffVar);
    bool exponentContainsVar = exponent->hasVariable(this->diffVar);
    nodePtr derivative;
    // Case 1: Base contains variable, exponent is constant
    if (baseContainsVar && !exponentContainsVar)
    {
        // Apply the basic power rule: d/dx [f(x)^a] = a * f(x)^(a-1) * f'(x)
        nodePtr one = std::make_shared<ExpressionNode>(
                    std::make_shared<Number>("1", 1)); \
            nodePtr exponentMinusOne = Operation::subtract(exponent, one);

        derivative = Operation::times(exponent, 
                                    Operation::power(base, exponentMinusOne));
        
    }
    // Case 2: Base is constant, exponent contains variable
    else if (!baseContainsVar && exponentContainsVar)
    {
        // Chain rule: d/dx [a^f(x)] = a^f(x) * ln(a) * f'(x)

        auto lnFunc = std::make_shared<Function>("ln");
        lnFunc->setSubExprTree(base);
        auto lnBase = std::make_shared<ExpressionNode>(lnFunc);

        // Set the derivative using chain rule
        derivative = Operation::times(Operation::power(base, exponent),
            Operation::times(lnBase, exponent->getDerivative()));
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
        derivative = Operation::times(Operation::power(base, exponent),
            Operation::times(lnBase, exponent->getDerivative()));
    }
    node->setDerivative(derivative);
    TreeFixer::simplify(node->getDerivative());
    return node->getDerivative();
}




std::shared_ptr<ExpressionNode> Derivative::productRule(nodePtr node)
{
    // Starting with d/dx u*v
    nodePtr u = node->getLeft();
    nodePtr v = node->getRight();
    nodePtr du_dx = u->getDerivative();
    nodePtr dv_dx = v->getDerivative();

    bool uContainsVar = u->hasVariable(this->diffVar);
    bool vContainsVar = v->hasVariable(this->diffVar);

    // Log the terms and derivatives


    if (uContainsVar && vContainsVar)
    {
        node->setDerivative(Operation::add(Operation::times(u, dv_dx),
                                           Operation::times(du_dx, v)));
       
    }
    else if (uContainsVar)
    {
        node->setDerivative(Operation::times(du_dx, v));
        
    }
    else if (vContainsVar)
    {
        node->setDerivative(Operation::times(u, dv_dx));
        
    }

    TreeFixer::simplify(node->getDerivative());
    return node->getDerivative();
}


std::shared_ptr<ExpressionNode> Derivative::quotientRule(nodePtr node)
{


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


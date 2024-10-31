#include "approx.hpp"
#include "derivative.hpp"
#include "tokenizer.hpp"
#include "lookup.hpp"
#include "tree_fixer.hpp"
#include "arithmetic.hpp"
#include "postfix.hpp"
#include "text_converter.hpp"

#include <exception>
#include <cfloat>
#include <iostream>
#include <stack>


Approx::Approx(std::string raw_input, std::string diffVar, double value)
{
    this->value = value;
    this->diffVar = std::make_shared<Variable>(diffVar);
    Tokenizer parser(raw_input);
    auto parsed = parser.tokenize();
    ShuntingYard converter(parsed);
    auto postfix = converter.getPostfix();
    this->root = ExpressionNode::buildTree(postfix);

    this->derivative = Derivative(raw_input,"x").solve();
}
std::pair<double,double> Approx::approximate()
{
    
    
    double originalApprox = approximate(this->root, this->diffVar, this->value);
    
    double derivativeApprox = approximate(this->derivative, this->diffVar, 
                                                                this->value);
    return std::make_pair(originalApprox, derivativeApprox);
}
std::shared_ptr<Token> Approx::replaceToken(nodePtr node, 
                                            std::shared_ptr<Variable> wrt, 
                                            double value)
{
    if (!node)
    {
        return nullptr;
    }
    std::shared_ptr<Token> outToken = node->getToken();
    if (node->getType() == TokenType::VARIABLE)
    {
        std::shared_ptr<Number> num;
        if (wrt->equals(node->getToken()))
        {

            outToken = std::make_shared<Number>(std::to_string(value),  value);
        }
        else
        {
            outToken = std::make_shared<Number>("1.0",1.0);
        }
        
    }
    else if (node->getType() == TokenType::FUNCTION)
    {
        auto func = std::dynamic_pointer_cast<Function>(node->getToken());
        auto subExpr = func->getSubExprTree();
        auto funcIter = Lookup::functionLookup.find(node->getStr());
        
        auto subApprox = approximate(subExpr, wrt, value);
        if (subApprox == DBL_MAX)
        {
            func->getSubExprTree()->printTree();
            throw std::runtime_error("Function subexpr not a number");
        }
        auto newSubRoot = std::make_shared<Number>(std::to_string(subApprox),
                                                    subApprox);
        func->setSubExprTree(std::make_shared<ExpressionNode>(newSubRoot));
        outToken = func;

        
    }
    return outToken;
}
double Approx::approximate(nodePtr root, std::shared_ptr<Variable> wrt, 
                                                                double value)
{
    auto rootCopy = root->copyTree();
    auto leaves = ExpressionNode::getLeaves(rootCopy);
    std::stack<nodePtr> stack;
    nodePtr current = rootCopy;
    while (current != nullptr || !stack.empty()) {
         
        while (current != nullptr) {
             
            stack.push(current);
            current = current->getLeft();
        }
 
        current = stack.top();
        stack.pop();
        auto replacement = replaceToken(current, wrt, value);
        current->setToken(replacement);
        current = current->getRight();
 
    }
    
    bool tempBool = Arithmetic::floatSimplification;
    Arithmetic::floatSimplification = true;
    TreeFixer::simplify(rootCopy);
    Arithmetic::floatSimplification = tempBool;
    if (rootCopy->getType() != TokenType::NUMBER)
    {
        rootCopy->printTree();
        throw std::runtime_error("tree root not a number");
        return DBL_MAX;
    }
    auto out = std::dynamic_pointer_cast<Number>(rootCopy->getToken());
    if (out->isInt())
    {
        return out->getInt() * 1.0;
    }
    else
    {
        return out->getDouble();
    }
}
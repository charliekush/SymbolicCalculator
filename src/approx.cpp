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
    std::cout << "original: " << TextConverter::convertToText(this->root) 
                                                                    << "\n";
    
    double originalApprox = approximate(this->root);
    std::cout << "\n\nDerivative: " << 
            TextConverter::convertToText(this->derivative) << "\n";
    double derivativeApprox = approximate(this->derivative);
    return std::make_pair(originalApprox, derivativeApprox);
}
std::shared_ptr<Token> Approx::replaceToken(nodePtr node)
{
    if (!node)
    {
        return nullptr;
    }
    //std::cout << "\nToken: " << node->getStr() << "\n";
    //std::cout << "Type: " << Lookup::getTokenType(node->getType()) << "\n";
    std::shared_ptr<Token> outToken = node->getToken();
    if (node->getType() == TokenType::VARIABLE)
    {
        std::shared_ptr<Number> num;
        if (diffVar->equals(node->getToken()))
        {
            //std::cout << "Token matches diffVar: " << node->getStr() << "\n";
            //std::cout << "Updating token to: " << this->value << "\n";

            outToken = std::make_shared<Number>(std::to_string(this->value),
                                            value);
        }
        else
        {
            //std::cout << "Token does not match diffVar: " << node->getStr() << "\n";
            outToken = std::make_shared<Number>("1.0",1.0);
        }
        
    }
    else if (node->getType() == TokenType::FUNCTION)
    {
        auto func = std::dynamic_pointer_cast<Function>(node->getToken());
        auto subExpr = func->getSubExprTree();
        auto funcIter = Lookup::functionLookup.find(node->getStr());
        
        auto subApprox = approximate(subExpr);
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
double Approx::approximate(nodePtr root)
{
    auto rootCopy = root->copyTree();
    auto leaves = ExpressionNode::getLeaves(rootCopy);
    //std::cout << "Before replace: " << TextConverter::convertToText(rootCopy) << "\n";
    std::stack<nodePtr> stack;
    nodePtr current = rootCopy;
    while (current != nullptr || !stack.empty()) {
         
        // Reach the left most Node of the
        // curr Node
        while (current != nullptr) {
             
            // Place pointer to a tree node on
            // the stack before traversing
            // the node's left subtree
            stack.push(current);
            current = current->getLeft();
        }
 
        // Current must be NULL at this point
        current = stack.top();
        stack.pop();
        auto replacement = this->replaceToken(current);
        current->setToken(replacement);
        //std::cout << "token: " << current->getStr() << "\n";
        // we have visited the node and its
        // left subtree.  Now, it's right
        // subtree's turn
        current = current->getRight();
 
    }
    
    //std::cout << "After replace: " << TextConverter::convertToText(rootCopy) << "\n";
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
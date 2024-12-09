#include "tree_fixer.hpp"
#include "tree_modifier.hpp"
#include "token.hpp"
#include "arithmetic.hpp"
#include "text_converter.hpp"
#include "lookup.hpp"

#include <iostream>
#include <cmath>
#include <stdexcept>




void TreeFixer::checkTree(nodePtr node)
{
    
    if (!node)
    {
        std::string msg = "Node is nullptr parent " +
            node->getParent().lock()->getStr();
        throw std::runtime_error(msg.c_str());
    }
    if (node->getType() == TokenType::FUNCTION)
    {
        auto function = std::dynamic_pointer_cast<Function>(node->getToken());
        TreeFixer::checkTree(function->getSubExprTree());
    }

    if (node->getType() != TokenType::NUMBER && node->getToken()->isNegative())
    {

        nodePtr expanded = TreeModifier::expandNegative(node);
        node->setToken(expanded->getToken());
        node->setDerivative(expanded->getDerivative());
        node->setLeft(expanded->getLeft());
        node->setRight(expanded->getRight());

        
    }
    if (node->getType() == TokenType::OPERATOR)
    {
        TreeFixer::checkChildren(node);
        if (!node->getLeft())
        {
            std::string msg = "Operator " + node->getStr() + 
            " is missing left child";
            throw std::runtime_error(msg.c_str());
        }
        TreeFixer::checkTree(node->getLeft());
        if (!node->getRight())
        {
            std::string msg = "Operator " + node->getStr() + 
            " is missing right child";
            throw std::runtime_error(msg.c_str());
        }
        TreeFixer::checkTree(node->getRight());
    }

}

void TreeFixer::checkChildren(nodePtr node)
{
    
    auto left = node->getLeft();
    auto right = node->getRight();

    if (!left)
    {
        std::string errMsg = "The node " + node->getToken()->getFullStr() +
            "has no left child\n";
        throw std::runtime_error(errMsg.c_str());
    }
    if (!right)
    {
        std::string errMsg = "The node " + node->getToken()->getFullStr() +
            "has no right child\n";
        throw std::runtime_error(errMsg.c_str());
    }
    
}




std::shared_ptr<ExpressionNode> TreeFixer::simplify(nodePtr node)
{
    ////std::cout << "\ninput: " << TextConverter::convertToText(node) << "\n";
    auto original = node->copyTree();
    auto left = node->getLeft();
    auto right = node->getRight();
    if (node->getType() == TokenType::OPERATOR)
    {
        if (node->getRight()->getPrecedence() < 
                node->getLeft()->getPrecedence())
        {
            if (node->getToken()->isCommutative())
            {
                node->swapChildren();
                
            }
        }
        if (left)
        {
            node->setLeft(simplify(left));
        }
        if (right)
        {
            node->setRight(simplify(right));
        }
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
        auto funcToken = std::dynamic_pointer_cast<Function>(node->getToken());
        nodePtr newSubRoot = TreeFixer::simplify(funcToken->getSubExprTree());
        
        funcToken->setSubExprTree(newSubRoot);
        auto funcIter = Lookup::functionLookup.find(node->getStr());
        if (funcIter != Lookup::functionLookup.end())
        {
            auto func = funcIter->second;

            
            func->update(node);
            
            if (newSubRoot->getType() == TokenType::NUMBER)
            {
                auto arg = std::dynamic_pointer_cast<Number>(
                                            newSubRoot->getToken());
                double result;
                double argNum;
                //std::cout << funcToken->getStr() << "(";
                if (arg->isInt())
                {
                    argNum = arg->getInt() * 1.0;
                    //std::cout << arg->getInt() * 1.0 << ")";
                }
                else
                {
                    argNum = arg->getDouble();
                    //std::cout << arg->getDouble() << ")";
                }
                result = func->evaluate(argNum);
                
                //std::cout << " = " << result << "\n";
                if (std::fmod(result, 1) == 0 || 
                            Arithmetic::floatSimplification)
                {
                    
                    node->setToken(std::make_shared<Number>(
                                        std::to_string(result),
                                        result));
                }
            }
            
            
            
        }
        
    }
    if (!TreeFixer::treesEqual(node, original))
    {
        node = simplify(node);
    }
    return node;
}

bool TreeFixer::treesEqual(const std::shared_ptr<ExpressionNode>& tree1, 
                            const std::shared_ptr<ExpressionNode>& tree2)
{
    if (!tree1 && !tree2)
    {
        return true;
    }
    if (!tree1 || !tree2)
    {
        return false;
    }
    TokenType type1 = tree1->getType();
    TokenType type2 = tree2->getType();

    if (type1 != type2)
    {
        return false;
    }
    if (type1 == TokenType::FUNCTION)
    {
        auto func1 = std::dynamic_pointer_cast<Function>(tree1->getToken());
        auto func2 = std::dynamic_pointer_cast<Function>(tree2->getToken());

        if (func1->getStr() != func1->getStr())
        {
            return false;
        }
        if (func1->getSubscript() != func1->getSubscript())
        {
            return false;
        }
        return treesEqual(func1->getSubExprTree(), func2->getSubExprTree());
    }
    if (type1 == TokenType::VARIABLE)
    {
        auto var1 = std::dynamic_pointer_cast<Variable>(tree1->getToken());
        auto var2 = std::dynamic_pointer_cast<Variable>(tree2->getToken());
        return var1->equals(var2);
    }
    if (tree1->getStr() != tree2->getStr())
    {
        return false;
    }
    return treesEqual(tree1->getLeft(), tree2->getLeft()) &&
           treesEqual(tree1->getRight(), tree2->getRight());
}
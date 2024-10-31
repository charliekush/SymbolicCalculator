/**
 * @file expression_node.cpp
 * @author Charles Kushelevsky (charliekushelevsky@gmail.com)
 * @brief contains definitions for @see expression_node.hpp
 * @version 0.1
 * @date 2024-09-29
 */
#include "expression_node.hpp"
#include "token.hpp"
#include "token_queue.hpp"
#include "tree_fixer.hpp"
#include "latex_converter.hpp"

#include <memory>
#include <string>
#include <iostream>

 /**
  * @brief Default constructor for ExpressionNode.
  *
  * Initializes the node with no token and no children.
  */
ExpressionNode::ExpressionNode()
{
    this->token = nullptr;
    this->leftChild = nullptr;
    this->rightChild = nullptr;
    this->derivative = nullptr;
}

/**
 * @brief Constructs a node from the given token.
 *
 * @param token The token that the node represents.
 */
ExpressionNode::ExpressionNode(std::shared_ptr<Token> token)
{
    this->token = token;
    this->leftChild = nullptr;
    this->rightChild = nullptr;
}


/**
 * @brief Gets the parent of the node.
 *
 * @return A weak pointer to the parent node.
 */
std::weak_ptr<ExpressionNode> ExpressionNode::getParent()
{
    return this->parent;
}

/**
 * @brief Removes the reference to the parent node.
 */
void ExpressionNode::removeParent()
{
    this->parent.reset();
}

/**
 * @brief Gets the right child of the node.
 *
 * @return A shared pointer to the right child node.
 */
std::shared_ptr<ExpressionNode> ExpressionNode::getRight()
{
    return this->rightChild;
}

/**
 * @brief Gets the left child of the node.
 *
 * @return A shared pointer to the left child node.
 */
std::shared_ptr<ExpressionNode> ExpressionNode::getLeft()
{
    return this->leftChild;
}

/**
 * @brief Set the Token object
 *
 * @param token the token to change to.
 */
void ExpressionNode::setToken(std::shared_ptr<Token> token)
{
    this->token = token;
}

/**
 * @brief Gets the token represented by this node.
 *
 * @return A shared pointer to the token.
 */
std::shared_ptr<Token> ExpressionNode::getToken()
{
    return this->token;
}

/**
 * @brief Gets the type of the token represented by this node.
 *
 * @return The TokenType of the node's token.
 */
TokenType ExpressionNode::getType()
{
    return this->token->getType();
}

int ExpressionNode::getPrecedence()
{
    return this->token->getPrecedence();
}
/**
 * @brief Gets the associativity of the token represented by this node.
 *
 * @return The associativity of the node's token.
 */
Associativity ExpressionNode::getAssociativity()
{
    return this->token->getAssociativity();
}

/**
 * @brief Gets the string representation of the token.
 *
 * @return A string representing the token.
 */
std::string ExpressionNode::getStr()
{
    return this->token->getStr();
}

/**
 * @brief Sets the parent node of this node.
 *
 * @param parent A weak pointer to the parent node.
 */
void ExpressionNode::setParent(std::weak_ptr<ExpressionNode> parent)
{
    if(this)
    {
        this->parent = parent;
    }
}

/**
 * @brief Removes the left child of this node.
 *
 * @return A shared pointer to the removed left child node.
 */
std::shared_ptr<ExpressionNode> ExpressionNode::removeLeftChild()
{
    std::shared_ptr<ExpressionNode> child = this->leftChild;
    this->leftChild->removeParent();
    this->leftChild = nullptr;
    return child;
}

/**
 * @brief Removes the right child of this node.
 *
 * @return A shared pointer to the removed right child node.
 */
std::shared_ptr<ExpressionNode> ExpressionNode::removeRightChild()
{
    std::shared_ptr<ExpressionNode> child = this->rightChild;
    this->rightChild->removeParent();
    this->rightChild = nullptr;
    return child;
}

/**
 * @brief Sets the left child of this node.
 *
 * @details If the left child is currently null, the provided node is set as
 * the left child.
 *
 * @param node A shared pointer to the node to set as the left child.
 * @return A shared pointer to the newly set left child.
 * @return nullptr if a left child already exists.
 */
std::shared_ptr<ExpressionNode> ExpressionNode::setLeft(
                        std::shared_ptr<ExpressionNode> node)
{
    this->leftChild = node;
    this->leftChild->setParent(weak_from_this());
    return this->leftChild;
}

/**
 * @brief Sets the right child of this node.
 *
 * @details If the right child is currently null, the provided node is set as
 * the right child.
 *
 * @param node A shared pointer to the node to set as the right child.
 * @return A shared pointer to the newly set right child.
 * @return nullptr if a right child already exists.
 */
std::shared_ptr<ExpressionNode> ExpressionNode::setRight(
                            std::shared_ptr<ExpressionNode> node)
{
   this->rightChild = node;
    this->rightChild->setParent(weak_from_this());
    return this->rightChild;
}

/**
 * @brief Adds a child node to this node.
 *
 * @details Attempts to set the provided node as the left child. If the left
 * child is already set, it attempts to set the node as the right child.
 *
 * @param node A shared pointer to the node to add as a child.
 * @return A shared pointer to the newly set child.
 *  @return nullptr if both left and right children are already set.
 */
std::shared_ptr<ExpressionNode> ExpressionNode::addChild(
                        std::shared_ptr<ExpressionNode> node)
{
    if (this->setLeft(node) != nullptr)
    {
        return this->leftChild;
    }
    if (this->setRight(node) != nullptr)
    {
        return this->rightChild;
    }
    return nullptr;
}

/**
 * @brief swaps the left and right children
 *
 */
void ExpressionNode::swapChildren()
{
    std::swap(this->leftChild, this->rightChild);
}

/**
     * @brief checks if subtree of node contains a given variable
     *
     * @param var the variable to be found
     * @return true if the variable is found
     * @return false otherwise
     */
bool ExpressionNode::hasVariable(const std::shared_ptr<Variable> var)
{
    if (this->getType() == TokenType::VARIABLE)
    {
        if (var->equals(this->getToken()))
        {
            return true;
        }
    }
    if (this->getLeft())
    {
        if (this->getLeft()->hasVariable(var))
        {
            return true;
        }
    }
    if (this->getRight())
    {
        if (this->getRight()->hasVariable(var))
        {
            return true;
        }
    }
    if (this->getType() == TokenType::FUNCTION)
    {
        auto func = std::dynamic_pointer_cast<Function>(this->getToken());
        if (func->getSubExprTree()->hasVariable(var))
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief Sets the derivative of this node.
 *
 * @param node A shared pointer to the node to set as the derivative root.
 * @return A shared pointer to the newly set derivative root.
 */
std::shared_ptr<ExpressionNode> ExpressionNode::setDerivative(
                            std::shared_ptr<ExpressionNode> node)
{
    this->derivative = node;//TreeFixer::simplify(node);
    return this->derivative;
}

/**
 * @brief Gets the derivative root of the node.
 *
 * @return A shared pointer to the derivative root of the node.
 * @return nullptr if it does not exist
 */
std::shared_ptr<ExpressionNode> ExpressionNode::getDerivative()
{
    if(this->derivative)
    {
        return this->derivative;
    }
    return nullptr;
}

/**
     * @brief checks if the node is a leaf node
     *
     * @return true if the node has no children
     * @return false otherwise
     */
bool ExpressionNode::isLeaf()
{
    return (!this->getLeft() && !this->getRight());
}






std::string ExpressionNode::getFullStr()
{
    if (this->getType() == TokenType::FUNCTION)
    {
        return LaTeXConverter::convertToLaTeX(weak_from_this().lock());
    }
    if (this->getType() == TokenType::VARIABLE)
    {
        return std::dynamic_pointer_cast<Variable>
                        (this->getToken())->getFullStr();
    }
    return this->getToken()->getFullStr();
}


std::shared_ptr<ExpressionNode> ExpressionNode::copyTree()
{
    auto copy = std::make_shared<ExpressionNode>(this->token);
    if (this->getLeft())
    {
        copy->setLeft(this->getLeft()->copyTree());
    }
    if (this->getRight())
    {
        copy->setRight(this->getRight()->copyTree());
    }
    
    if (this->getDerivative())
    {
        copy->setDerivative(this->getDerivative());
    }
    return copy;
}
void ExpressionNode::copyNode(std::shared_ptr<ExpressionNode> src)
{
    this->setToken(src->getToken());
    this->setDerivative(src->getDerivative());
    this->setLeft(src->getLeft());
    this->setRight(src->getRight());

    
}
void ExpressionNode::printFuncTree(std::shared_ptr<Function> func, int depth)
{
    std::string indent(depth * 6, ' ');  // Indentation for the function and operator

    // Get the subexpression tree (e.g., x + 2 or another function like cos(x))
    auto subTree = func->getSubExprTree();

    if (subTree)
    {
        // Check if the subtree root is another function
        if (subTree->getType() == TokenType::FUNCTION)
        {
            auto nestedFunc = std::dynamic_pointer_cast<Function>(subTree->getToken());
            if (nestedFunc)
            {
                // Recursively print the nested function tree with increased depth
                this->printFuncTree(nestedFunc, depth + 1);
            }
        }
        else
        {
            // Regular handling for non-function subtrees (e.g., x + 2)
            auto rightChild = subTree->getRight();
            auto leftChild = subTree->getLeft();

            // Print the right child first (which will appear above the function/operator)
            if (rightChild)
            {
                rightChild->printTree(depth + 1);  // Print right child slightly indented
            }

            // Print the function name along with the operator below it
            std::cout << indent << func->getStr() << "     " << subTree->getFullStr() << std::endl;

            // Print the left child after the operator and function (which appears below)
            if (leftChild)
            {
                leftChild->printTree(depth + 1);  // Print left child slightly indented
            }
        }
    }
}



void ExpressionNode::printTree(int depth)
{
    std::string indent(depth * 6, ' ');

    // Check if the node represents a function
    if (this->getType() == TokenType::FUNCTION)
    {
        auto func = std::dynamic_pointer_cast<Function>(this->getToken());
        if (func)
        {
            // Call printFuncTree to handle function tokens
            this->printFuncTree(func, depth);
        }
    }
    else
    {
        // Increase the indentation for deeper levels
        auto rightChild = this->getRight();
        auto leftChild = this->getLeft();

        // Print the right child first (appears on top)
        if (rightChild)
        {
            rightChild->printTree(depth + 1);
        }

        // Print the current node
        std::cout << indent << getFullStr() << std::endl;

        // Print the left child last (appears on the bottom)
        if (leftChild)
        {
            leftChild->printTree(depth + 1);
        }
    }

    // Print a separator when finished printing the whole tree (depth = 0)
    if (depth == 0)
    {
        std::cout << "\n\n";
    }
}
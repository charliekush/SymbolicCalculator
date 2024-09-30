/**
 * @file expression_node.cpp
 * @author Charles Kushelevsky (charliekushelevsky@gmail.com)
 * @brief contains definitions for @see expression_node.hpp
 * @version 0.1
 * @date 2024-09-29
 */
#include "expression_node.hpp"
#include "token.hpp"

#include <memory>
#include <string>

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
    this->parent = parent;
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
    if (this->leftChild == nullptr)
    {
        this->leftChild = node;
        this->leftChild->setParent(weak_from_this());
        return this->leftChild;
    }
    return nullptr;
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
    if (this->rightChild == nullptr)
    {
        this->rightChild = node;
        this->rightChild->setParent(weak_from_this());
        return this->rightChild;
    }
    return nullptr;
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
    std::swap(this->leftChild,this->rightChild);
}

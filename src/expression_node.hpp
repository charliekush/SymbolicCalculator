/**
 * @file expression_node.hpp
 * @author Charles Kushelevsky (charliekushelevsky@gmail.com)
 * @brief Contains class declaration for nodes in the expression tree.
 * @version 0.1
 * @date 2024-09-29
 */
#ifndef __EXPRESSION_NODE_HPP__
#define __EXPRESSION_NODE_HPP__

#include "token.hpp"

#include <memory>

class ExpressionNode : public std::enable_shared_from_this<ExpressionNode>
{
public:
    /**
     * @brief Default constructor for ExpressionNode.
     *
     * Initializes the node with no token and no children.
     */
    ExpressionNode();

    /**
     * @brief Constructs a node from the given token.
     *
     * @param token The token that the node represents.
     */
    ExpressionNode(std::shared_ptr<Token> token);

    /**
     * @brief Adds a child node to this node.
     *
     * @details Attempts to set the provided node as the left child. If the
     * left child is already set, it attempts to set the node as the
     * right child.
     *
     * @param node A shared pointer to the node to add as a child.
     * @return A shared pointer to the newly set child.
     * @return nullptr if both left and right children are already set.
     */
    std::shared_ptr<ExpressionNode> addChild(
                            std::shared_ptr<ExpressionNode> node);
    /**
     * @brief Sets the left child of this node.
     *
     * @details If the left child is currently null, the provided node is set
     * as the left child.
     *
     * @param node A shared pointer to the node to set as the left child.
     * @return A shared pointer to the newly set left child.
     * @return nullptr if a left child already exists.
     */
    std::shared_ptr<ExpressionNode> setLeft(
                            std::shared_ptr<ExpressionNode> node);

    /**
     * @brief Sets the right child of this node.
     *
     * @details If the right child is currently null, the provided node is set
     * as the right child.
     *
     * @param node A shared pointer to the node to set as the right child.
     * @return A shared pointer to the newly set right child.
     * @return nullptr if a right child already exists.
     */
    std::shared_ptr<ExpressionNode> setRight(
                            std::shared_ptr<ExpressionNode> node);

    /**
     * @brief swaps the left and right children
     *
     */
    void swapChildren();

    /**
     * @brief Sets the parent node of this node.
     *
     * @param parent A weak pointer to the parent node.
     */
    void setParent(std::weak_ptr<ExpressionNode> parent);

    /**
     * @brief Removes the reference to the parent node.
     */
    void removeParent();

    /**
     * @brief Removes the left child of this node.
     *
     * @return A shared pointer to the removed left child node.
     */
    std::shared_ptr<ExpressionNode> removeLeftChild();

    /**
     * @brief Removes the right child of this node.
     *
     * @return A shared pointer to the removed right child node.
     */
    std::shared_ptr<ExpressionNode> removeRightChild();

    /**
     * @brief Gets the parent of the node.
     *
     * @return A weak pointer to the parent node.
     */
    std::weak_ptr<ExpressionNode> getParent();

    /**
     * @brief Gets the right child of the node.
     *
     * @return A shared pointer to the right child node.
     */
    std::shared_ptr<ExpressionNode> getRight();

    /**
     * @brief Gets the left child of the node.
     *
     * @return A shared pointer to the left child node.
     */
    std::shared_ptr<ExpressionNode> getLeft();

    /**
     * @brief Gets the token represented by this node.
     *
     * @return A shared pointer to the token.
     */
    std::shared_ptr<Token> getToken();

    /**
     * @brief Gets the type of the token represented by this node.
     *
     * @return The TokenType of the node's token.
     */
    TokenType getType();

    /**
     * @brief Gets the associativity of the token represented by this node.
     *
     * @return The associativity of the node's token.
     */
    Associativity getAssociativity();
    /**
     * @brief Gets the string representation of the token.
     *
     * @return A string representing the token.
     */
    std::string getStr();

protected:
    std::shared_ptr<Token> token;
    std::weak_ptr<ExpressionNode> parent;
    std::shared_ptr<ExpressionNode> leftChild;
    std::shared_ptr<ExpressionNode> rightChild;

};
#endif // __EXPRESSION_NODE_HPP__
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
#include <vector>

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
     * @brief creates expression tree from a postfix input
     * 
     * @param queue 
     * @return std::shared_ptr<ExpressionNode> the root of the tree
     */
    static std::shared_ptr<ExpressionNode> buildTree(TokenQueue queue);

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
     * @details Sets the provided node as the left child.
     *
     * @param node A shared pointer to the node to set as the left child.
     * @return A shared pointer to the newly set left child.
     */
    std::shared_ptr<ExpressionNode> setLeft(
                            std::shared_ptr<ExpressionNode> node);

    /**
     * @brief Sets the right child of this node.
     *
     * @details Sets the provided node as the right child.
     *
     * @param node A shared pointer to the node to set as the right child.
     * @return A shared pointer to the newly set right child.
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
     * @brief Set the Token object
     * 
     * @param token the token to change to.
     */
    void setToken(std::shared_ptr<Token> token);
    /**
     * @brief Gets the token represented by this node.
     *
     * @return A shared pointer to the token.
     */
    std::shared_ptr<Token> getToken();

    /**
     * @brief Gets the precedence of the token represented by this node.
     *
     * @return The SymbolProperties of the node's token.
     */
    int getPrecedence();
    

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
    
    /**
     * @brief checks if subtree of node contains a given variable
     * 
     * @param var the variable to be found
     * @return true if the variable is found
     * @return false otherwise
     */
    bool hasVariable(const std::shared_ptr<Variable> var);

    /**
     * @brief Sets the derivative of this node.
     *
     * @param node A shared pointer to the node to set as the derivative root.
     * @return A shared pointer to the newly set derivative root.
     */
    std::shared_ptr<ExpressionNode> setDerivative(
                            std::shared_ptr<ExpressionNode> node);
    /**
     * @brief Sets the derivative of this node when it is a single token.
     *
     * @param node A shared pointer to the token to set as the derivative root.
     * @return A shared pointer to the newly set derivative root.
     */
    std::shared_ptr<ExpressionNode> setDerivative(
                            std::shared_ptr<Token> token);                      
    /**
     * @brief Gets the derivative root of the node.
     *
     * @return A shared pointer to the derivative root of the node.
     * @return nullptr if it does not exist
     */
    std::shared_ptr<ExpressionNode> getDerivative();
    
    /**
     * @brief checks if the node is a leaf node
     * 
     * @return true if the node has no children
     * @return false otherwise
     */
    bool isLeaf();
    
    /**
     * @brief Get the fullStr of the token.
     * 
     * @return std::string 
     */
    std::string getFullStr();
    
    /**
     * @brief deep copy of subtree
     * 
     *
     * @return root of the copied tree
     */
    std::shared_ptr<ExpressionNode> copyTree();
    /**
     * @brief deep copy of node
     * 
     *
     * @return the copied node
     */
    void copyNode(std::shared_ptr<ExpressionNode> src);
    /**
     * @brief prints horizontal tree
     * 
     * @param depth indentation level
     */
    void printTree(int depth = 0);
    /**
     * @brief prints function within tree
     * 
     * @param func 
     * @param depth 
     */
    void printFuncTree(std::shared_ptr<Function> func, int depth);
    
protected:
    std::shared_ptr<Token> token;
    std::weak_ptr<ExpressionNode> parent;
    std::shared_ptr<ExpressionNode> leftChild;
    std::shared_ptr<ExpressionNode> rightChild;
    std::shared_ptr<ExpressionNode> derivative;
    
    
    
}; 
#endif // __EXPRESSION_NODE_HPP__
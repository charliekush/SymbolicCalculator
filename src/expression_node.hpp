#ifndef __EXPRESSION_NODE_HPP__
#define __EXPRESSION_NODE_HPP__

#include "token.hpp"

#include <memory>
class ExpressionNode : public std::enable_shared_from_this<ExpressionNode>
{
public:
    ExpressionNode();
    ExpressionNode(std::shared_ptr<Token> token);
    Token getToken();
    std::shared_ptr<ExpressionNode> addChild(
                            std::shared_ptr<ExpressionNode> node);
    std::shared_ptr<ExpressionNode> setLeft(
                            std::shared_ptr<ExpressionNode> node);
    std::shared_ptr<ExpressionNode> setRight(
                            std::shared_ptr<ExpressionNode> node);
    void swapChildren();
    void setParent(std::weak_ptr<ExpressionNode> parent);
    void removeParent();
    std::shared_ptr<ExpressionNode> removeLeftChild();
    std::shared_ptr<ExpressionNode> removeRightChild();
    std::weak_ptr<ExpressionNode> getParent();
    std::shared_ptr<ExpressionNode> getRight();
    std::shared_ptr<ExpressionNode> getLeft();
    TokenType getType();
    Associativity getAssociativity();
    std::string getStr();

private:
    std::shared_ptr<Token> token;
    std::weak_ptr<ExpressionNode> parent;
    std::shared_ptr<ExpressionNode> leftChild;
    std::shared_ptr<ExpressionNode> rightChild;

};
#endif // __EXPRESSION_NODE_HPP__
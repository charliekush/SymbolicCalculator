#include "expression_node.hpp"
#include "token.hpp"

#include <memory>
#include <string>

ExpressionNode::ExpressionNode()
{
    this->token = nullptr;
    this->leftChild = nullptr;
    this->rightChild = nullptr;
}

ExpressionNode::ExpressionNode(std::shared_ptr<Token> token)
{
    this->token = token;
    this->leftChild = nullptr;
    this->rightChild = nullptr;
}

std::weak_ptr<ExpressionNode> ExpressionNode::getParent()
{
    return this->parent;
}
void ExpressionNode::removeParent()
{
    this->parent.reset();
}
std::shared_ptr<ExpressionNode> ExpressionNode::getRight()
{
    return this->rightChild;
}
std::shared_ptr<ExpressionNode> ExpressionNode::getLeft()
{
    return this->leftChild;
}
TokenType ExpressionNode::getType()
{
    return this->token->getType();
}
Associativity ExpressionNode::getAssociativity()
{
    return this->token->getAssociativity();
}
std::string ExpressionNode::getStr()
{
    return this->token->getStr();
}

void ExpressionNode::setParent(std::weak_ptr<ExpressionNode> parent)
{
    this->parent = parent;
}

std::shared_ptr<ExpressionNode> ExpressionNode::removeLeftChild()
{
    std::shared_ptr<ExpressionNode> child = this->leftChild;
    this->leftChild->removeParent();
    this->leftChild = nullptr;
    return child;
}
std::shared_ptr<ExpressionNode> ExpressionNode::removeRightChild()
{
    std::shared_ptr<ExpressionNode> child = this->rightChild;
    this->rightChild->removeParent();
    this->rightChild = nullptr;
    return child;
}


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


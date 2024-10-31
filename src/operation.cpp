#include "operation.hpp"
#include "token.hpp"

std::shared_ptr<ExpressionNode> Operation::times(nodePtr left, nodePtr right)
{
    auto opToken = std::make_shared<Operator>("*");
    auto node = std::make_shared<ExpressionNode>(opToken);
    node->setLeft(left);
    node->setRight(right);

    return node;
}
std::shared_ptr<ExpressionNode> Operation::divide(nodePtr left, nodePtr right)
{
    auto opToken = std::make_shared<Operator>("/");
    auto node = std::make_shared<ExpressionNode>(opToken);
    node->setLeft(left);
    node->setRight(right);

    return node;
}
std::shared_ptr<ExpressionNode> Operation::add(nodePtr left, nodePtr right)
{
    auto opToken = std::make_shared<Operator>("+");
    auto node = std::make_shared<ExpressionNode>(opToken);
    node->setLeft(left);
    node->setRight(right);

    return node;
}
std::shared_ptr<ExpressionNode> Operation::subtract(nodePtr left,
                                                            nodePtr right)
{
    auto opToken = std::make_shared<Operator>("-");
    auto node = std::make_shared<ExpressionNode>(opToken);
    node->setLeft(left);
    node->setRight(right);

    return node;
}
std::shared_ptr<ExpressionNode> Operation::power(nodePtr left,
                                                            nodePtr right)
{
    auto opToken = std::make_shared<Operator>("^");
    auto node = std::make_shared<ExpressionNode>(opToken);
    node->setLeft(left);
    node->setRight(right);

    return node;
}
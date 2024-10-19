#include "calculator.hpp"

#include <cmath>

std::shared_ptr<Number> Arithmetic::performOperation(const operation& op,
                            numPtr left, numPtr right, bool isDivision = false)
{
    // Handle division separately
    if (isDivision)
    {
        // If integer division
        if (left->isInt() && right->isInt())
        {
            // If evenly divisible
            if (left->getInt() % right->getInt() == 0)
            {
                int result = left->getInt() / right->getInt();
                // return int
                return std::make_shared<Number>(std::to_string(result), result);
            }
            else
            {

                double result = static_cast<double>(left->getInt()) / right->getInt();
                // Return a float
                return std::make_shared<Number>(std::to_string(result), result);

            }
        }
    }

    // Handle cases where both operands are integers for non-division operations
    if (left->isInt() && right->isInt())
    {
        int result = static_cast<int>(op(left->getInt(), right->getInt()));
        // Return an int
        return std::make_shared<Number>(std::to_string(result), result);
    }

    // Handle mixed types or doubles
    double result;
    if (left->isInt() && right->isDouble())
    {
        result = op(left->getInt(), right->getDouble());
    }
    else if (left->isDouble() && right->isInt())
    {
        result = op(left->getDouble(), right->getInt());
    }
    else if (left->isDouble() && right->isDouble())
    {
        result = op(left->getDouble(), right->getDouble());
    }
    else
    {
        return nullptr;
    }

    double floatPart = std::modf(result, &floatPart);
    if (floatPart == 0.0)
    {
        return std::make_shared<Number>(std::to_string((int)result),
                                                                (int)result);
    }
    return std::make_shared<Number>(std::to_string(result), result);
}

std::shared_ptr<Number> Arithmetic::divide(nodePtr node, numPtr left,
                                                                numPtr right)
{
    if (right->equals(0))
    {
        throw std::runtime_error("Undefined arithmetic: divide by 0");
    }

    auto divideOp = [](double a, double b) { return a / b; };
    return performOperation(divideOp, left, right, true);
}

std::shared_ptr<Number> Arithmetic::add(nodePtr node, numPtr left, numPtr right)
{
    auto addOp = [](double a, double b) { return a + b; };
    return performOperation(addOp, left, right);
}

std::shared_ptr<Number> Arithmetic::subtract(nodePtr node, numPtr left,
                                                                numPtr right)
{
    auto subtractOp = [](double a, double b) { return a - b; };
    return performOperation(subtractOp, left, right);
}

std::shared_ptr<Number>Arithmetic::multiply(nodePtr node, numPtr left,
                                                                numPtr right)
{
    auto multiplyOp = [](double a, double b) { return a * b; };
    return performOperation(multiplyOp, left, right);
}

std::shared_ptr<Number> Arithmetic::power(nodePtr node, numPtr left,
                                                                numPtr right)
{
    if (left->equals(0) && right->equals(0))
    {
        throw std::runtime_error("Undefined arithmetic: 0^0");
    }

    auto powerOp = [](double a, double b) { return std::pow(a, b); };
    return performOperation(powerOp, left, right);
}




std::shared_ptr<Number> Arithmetic::getNumberToken(const nodePtr& node) {
    if (node->getType() == TokenType::NUMBER)
    {
        return std::dynamic_pointer_cast<Number>(node->getToken());
    }
    return nullptr;
}

void Arithmetic::setNodeToZero(nodePtr operatorNode) {
    operatorNode->removeLeftChild();
    operatorNode->removeRightChild();
    operatorNode->setToken(std::make_shared<Number>("0", 0));
    operatorNode->setDerivative(std::make_shared<ExpressionNode>(
                                    std::make_shared<Number>("0", 0)));
}

void Arithmetic::setNodeToOne(nodePtr operatorNode) {
    operatorNode->removeLeftChild();
    operatorNode->removeRightChild();
    operatorNode->setToken(std::make_shared<Number>("1", 1));
    operatorNode->setDerivative(std::make_shared<ExpressionNode>(
                                    std::make_shared<Number>("0", 0)));
}

void Arithmetic::simplify(nodePtr node, numPtr left, numPtr right)
{
    if (node->getStr() == "^")
    {
        simplifyExponent(node);
    }
    else if (node->getStr() == "*")
    {
        simplifyMultiplication(node);
    }
    else if (node->getStr() == "/")
    {
        simplifyDivision(node);
    }
    else if (node->getStr() == "+")
    {
        simplifyAddition(node);
    }
    else if (node->getStr() == "-")
    {
        simplifySubtraction(node);
    }
}

void Arithmetic::simplifyExponent(nodePtr operatorNode)
{
    numPtr leftNum = getNumberToken(operatorNode->getLeft());
    numPtr rightNum = getNumberToken(operatorNode->getRight());
    if (leftNum && rightNum)
    {
        auto value = Arithmetic::power(operatorNode, leftNum, rightNum);
        operatorNode->removeLeftChild();
        operatorNode->removeRightChild();
        operatorNode->setToken(value);
        operatorNode->setDerivative(std::make_shared<ExpressionNode>(
                                        std::make_shared<Number>("0", 0)));
    }
    else if (leftNum)
    {
        if (leftNum->equals(0))
        {
            setNodeToZero(operatorNode);
        }
        else if (leftNum->equals(1))
        {
            operatorNode->removeLeftChild();
            operatorNode->removeRightChild();
            operatorNode->setToken(std::make_shared<Number>("1", 1));
            operatorNode->setDerivative(std::make_shared<ExpressionNode>(
                                            std::make_shared<Number>("0", 0)));
        }
    }
    else if (rightNum)
    {
        if (rightNum->equals(0))
        {
            operatorNode->removeLeftChild();
            operatorNode->removeRightChild();
            operatorNode->setToken(std::make_shared<Number>("1", 1));
            operatorNode->setDerivative(std::make_shared<ExpressionNode>(
                                        std::make_shared<Number>("0", 0)));
        }
        else if (rightNum->equals(1))
        {
            operatorNode->replaceWithLeftChild();
        }
    }
}

void Arithmetic::simplifyMultiplication(nodePtr operatorNode)
{
    auto leftNum = getNumberToken(operatorNode->getLeft());
    auto rightNum = getNumberToken(operatorNode->getRight());
    if (leftNum && rightNum)
    {
        auto value = Arithmetic::multiply(operatorNode, leftNum, rightNum);
        operatorNode->removeLeftChild();
        operatorNode->removeRightChild();
        operatorNode->setToken(value);
        operatorNode->setDerivative(std::make_shared<ExpressionNode>(
                                        std::make_shared<Number>("0", 0)));
    }
    else if (leftNum)
    {
        if (leftNum->equals(0))
        {
            setNodeToZero(operatorNode);
        }
        else if (leftNum->equals(1))
        {
            operatorNode->replaceWithRightChild();
        }
    }
    else if (rightNum)
    {
        if (rightNum->equals(0))
        {
            setNodeToZero(operatorNode);
        }
        else if (rightNum->equals(1))
        {
            operatorNode->replaceWithLeftChild();
        }
    }
}


void Arithmetic::simplifyDivision(nodePtr operatorNode)
{
    auto leftNum = getNumberToken(operatorNode->getLeft());
    auto rightNum = getNumberToken(operatorNode->getRight());
    if (leftNum && rightNum)
    {
        auto value = Arithmetic::multiply(operatorNode, leftNum, rightNum);
        operatorNode->removeLeftChild();
        operatorNode->removeRightChild();
        operatorNode->setToken(value);
        operatorNode->setDerivative(std::make_shared<ExpressionNode>(
                                        std::make_shared<Number>("0", 0)));
    }
    else if (leftNum)
    {
        if (leftNum->equals(0))
        {
            setNodeToZero(operatorNode);
        }
    }
    else if (rightNum)
    {
        if (rightNum->equals(0))
        {
            throw std::runtime_error("Undefined arithmetic: divide by 0");
        }
        else if (rightNum->equals(1))
        {
            operatorNode->replaceWithLeftChild();
        }
    }
}

void Arithmetic::simplifyAddition(nodePtr operatorNode)
{
    auto leftNum = getNumberToken(operatorNode->getLeft());
    auto rightNum = getNumberToken(operatorNode->getRight());
    if (leftNum && rightNum)
    {
        auto value = Arithmetic::add(operatorNode, leftNum, rightNum);
        operatorNode->removeLeftChild();
        operatorNode->removeRightChild();
        operatorNode->setToken(value);
        operatorNode->setDerivative(std::make_shared<ExpressionNode>(
                                        std::make_shared<Number>("0", 0)));
    }
    else if (leftNum)
    {
        if (leftNum->equals(0))
        {
            operatorNode->replaceWithRightChild();
        }
    }
    else if (rightNum)
    {
        if (rightNum->equals(0))
        {
            operatorNode->replaceWithLeftChild();
        }
    }
}


void Arithmetic::simplifySubtraction(nodePtr operatorNode)
{
    auto leftNum = getNumberToken(operatorNode->getLeft());
    auto rightNum = getNumberToken(operatorNode->getRight());
    if (leftNum && rightNum)
    {
        auto value = Arithmetic::subtract(operatorNode, leftNum, rightNum);
        operatorNode->removeLeftChild();
        operatorNode->removeRightChild();
        operatorNode->setToken(value);
        operatorNode->setDerivative(std::make_shared<ExpressionNode>(
                                        std::make_shared<Number>("0", 0)));
    }
    else if (leftNum)
    {
        if (leftNum->equals(0))
        {
            operatorNode->replaceWithRightChild();
            operatorNode->getToken()->flipSign();
        }
    }
    else if (rightNum)
    {
        if (rightNum->equals(0))
        {
            operatorNode->replaceWithLeftChild();
        }
    }
}







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

void TreeFixer::checkTree(nodePtr node)
{
    if (node->getType() == TokenType::FUNCTION)
    {
        auto function = std::dynamic_pointer_cast<Function>(node->getToken());
        TreeFixer::checkTree(function->getSubExprTree());
    }
    
    if (node->getType() != TokenType::NUMBER && node->getToken()->isNegative())
    {
        TreeFixer::expandNegative(node);
    }
    if(node->getType() == TokenType::OPERATOR)
    {
        TreeFixer::checkChildren(node);
        TreeFixer::checkTree(node->getLeft());
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
    if (right->getType() < left->getType())
    {
        if (node->getToken()->isCommutative())
        {
            node->swapChildren();
            left = node->getLeft();
            right = node->getRight();
        }
    }
}

std::shared_ptr<ExpressionNode> 
    TreeFixer::expandNegative(std::shared_ptr<ExpressionNode> node)
{
    if (node->getType() != TokenType::NUMBER && node->getToken()->isNegative())
    {
        // Flip the sign of the original node's token to make it positive
        node->getToken()->flipSign();
        return node->makeNegative();
        
    }
    return node;
}
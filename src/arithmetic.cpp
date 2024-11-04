#include "arithmetic.hpp"
#include "latex_converter.hpp"
#include "tree_modifier.hpp"

#include <cmath>
#include <iostream>
#include "tree_fixer.hpp"


bool Arithmetic::floatSimplification = true;

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
                int result = (left->getInt()) / (right->getInt());
                // return int
                auto out = std::make_shared<Number>(std::to_string(result),
                                                                        result);
                out->setNegative(result < 0);
                return out;
            }
            else
            {

                double result = static_cast<double>(left->getInt()) /
                    ( right->getInt());
                // Return a float
                auto out = std::make_shared<Number>(std::to_string(result),
                                                                    result);
                out->setNegative(result < 0);
                if (std::fmod(result, 1) != 0 &&
                                !Arithmetic::floatSimplification)
                {
                    return nullptr;
                }
                return out;

            }
        }
    }

    // Handle cases where both operands are integers for non-division operations
    if (left->isInt() && right->isInt())
    {
        int result = static_cast<int>(op((left->getInt()), 
                                                 right->getInt()));
        // Return an int
        auto out = std::make_shared<Number>(std::to_string(result), result);
        out->setNegative(result < 0);

        return out;


    }

    // Handle mixed types or doubles
    double result;
    if (left->isInt() && right->isDouble())
    {
        result = op( left->getInt(),  right->getDouble());
    }
    else if (left->isDouble() && right->isInt())
    {
        result = op( left->getDouble(),  right->getInt());
    }
    else if (left->isDouble() && right->isDouble())
    {
        result = op( left->getDouble(), 
                         right->getDouble());
    }
    else
    {
        return nullptr;
    }

    double floatPart = std::modf(result, &floatPart);

    if (floatPart == 0.0)
    {
        auto out = std::make_shared<Number>(std::to_string((int)result),
                                                                (int)result);
        out->setNegative(result < 0);
        return out;
    }
    if (!Arithmetic::floatSimplification)
    {
        return nullptr;
    }
    auto out = std::make_shared<Number>(std::to_string(result), result);
    out->setNegative(result < 0);
    return out;
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

void Arithmetic::setNodeToZero(nodePtr& operatorNode) {
    operatorNode->removeLeftChild();
    operatorNode->removeRightChild();
    operatorNode->setToken(std::make_shared<Number>("0", 0));
    operatorNode->setDerivative(std::make_shared<ExpressionNode>(
        std::make_shared<Number>("0", 0)));
}

void Arithmetic::setNodeToOne(nodePtr& operatorNode) {
    operatorNode->removeLeftChild();
    operatorNode->removeRightChild();
    operatorNode->setToken(std::make_shared<Number>("1", 1));
    operatorNode->setDerivative(std::make_shared<ExpressionNode>(
        std::make_shared<Number>("0", 0)));
}

void Arithmetic::simplify(nodePtr node)
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

void Arithmetic::simplifyExponent(nodePtr& operatorNode)
{
    numPtr leftNum = getNumberToken(operatorNode->getLeft());
    numPtr rightNum = getNumberToken(operatorNode->getRight());
    if (leftNum && rightNum)
    {
        auto value = Arithmetic::power(operatorNode, leftNum, rightNum);
        if (value)
        {
            std::cout << leftNum->getFullStr() << "^" << rightNum->getFullStr() << " = " << value->getFullStr() << "\n";
            operatorNode->removeLeftChild();
            operatorNode->removeRightChild();
            operatorNode->setToken(value);
            operatorNode->setDerivative(std::make_shared<ExpressionNode>(
                std::make_shared<Number>("0", 0)));

            return;
        }
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
            TreeModifier::replaceWithLeftChild(operatorNode);
        }
    }
}

void Arithmetic::simplifyMultiplication(nodePtr& operatorNode)
{
    auto leftNum = getNumberToken(operatorNode->getLeft());
    auto rightNum = getNumberToken(operatorNode->getRight());

    if (leftNum && rightNum)
    {
        auto value = Arithmetic::multiply(operatorNode, leftNum, rightNum);
        if (value)
        {
            std::cout << leftNum->getFullStr() << "*" << rightNum->getFullStr() << " = " << value->getFullStr() << "\n";
            operatorNode->removeLeftChild();
            operatorNode->removeRightChild();
            operatorNode->setToken(value);
            operatorNode->setDerivative(std::make_shared<ExpressionNode>(
                std::make_shared<Number>("0", 0)));
            return;
        }
    }
    else if (leftNum)
    {
        if (leftNum->equals(0))
        {
            setNodeToZero(operatorNode);
        }
        else if (leftNum->equals(1))
        {

            TreeModifier::replaceWithRightChild(operatorNode);
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
            TreeModifier::replaceWithLeftChild(operatorNode);
        }
    }

    if (TreeFixer::treesEqual(operatorNode->getLeft(), operatorNode->getRight()))
    {
        operatorNode->setToken(std::make_shared<Operator>("^"));
        operatorNode->setLeft(std::make_shared<ExpressionNode>(
            std::make_shared<Number>("2", 2)));
    }
    if (operatorNode->getLeft()->getStr() == "^" &&
                TreeFixer::treesEqual(operatorNode->getLeft()->getLeft(),
                    operatorNode->getRight()))
    {
        operatorNode->swapChildren();
        operatorNode->setRight(operatorNode->getRight()->getLeft());
        Arithmetic::simplify(operatorNode->getRight());
    }
    if (operatorNode->getRight()->getStr() == "^" &&
                TreeFixer::treesEqual(operatorNode->getRight()->getLeft(),
                    operatorNode->getLeft()))
    {
       
        operatorNode->setRight(operatorNode->getRight()->getRight());
        Arithmetic::simplify(operatorNode->getRight());
    }
}


void Arithmetic::simplifyDivision(nodePtr& operatorNode)
{
    auto leftNum = getNumberToken(operatorNode->getLeft());
    auto rightNum = getNumberToken(operatorNode->getRight());
    if (leftNum && rightNum)
    {
        auto value = Arithmetic::divide(operatorNode, leftNum, rightNum);
        if (value)
        {
            std::cout << leftNum->getFullStr() << "/" << rightNum->getFullStr() << " = " << value->getFullStr() << "\n";
            operatorNode->removeLeftChild();
            operatorNode->removeRightChild();
            operatorNode->setToken(value);
            operatorNode->setDerivative(std::make_shared<ExpressionNode>(
                std::make_shared<Number>("0", 0)));
            return;
        }
    }
    if (leftNum)
    {
        if (leftNum->equals(0))
        {
            setNodeToZero(operatorNode);
            return;
        }
    }
    if (rightNum)
    {
        if (rightNum->equals(0))
        {
            throw std::runtime_error("Undefined arithmetic: divide by 0");
        }
        else if (rightNum->equals(1))
        {
            TreeModifier::replaceWithLeftChild(operatorNode);
        }
    }
}

void Arithmetic::simplifyAddition(nodePtr& operatorNode)
{
    auto leftNum = getNumberToken(operatorNode->getLeft());
    auto rightNum = getNumberToken(operatorNode->getRight());
    if (leftNum && rightNum)
    {
        auto value = Arithmetic::add(operatorNode, leftNum, rightNum);
        if (value)
        {
            std::cout << leftNum->getFullStr() << "+" << rightNum->getFullStr() << " = " << value->getFullStr() << "\n";
            operatorNode->removeLeftChild();
            operatorNode->removeRightChild();
            operatorNode->setToken(value);
            operatorNode->setDerivative(std::make_shared<ExpressionNode>(
                std::make_shared<Number>("0", 0)));
            return;
        }
    }
    else if (leftNum)
    {
        if (leftNum->equals(0))
        {
            TreeModifier::replaceWithRightChild(operatorNode);
        }
    }
    else if (rightNum)
    {
        if (rightNum->equals(0))
        {
            TreeModifier::replaceWithLeftChild(operatorNode);
        }
    }
}


void Arithmetic::simplifySubtraction(nodePtr& operatorNode)
{
    auto leftNum = getNumberToken(operatorNode->getLeft());
    auto rightNum = getNumberToken(operatorNode->getRight());
    if (leftNum && rightNum)
    {
        auto value = Arithmetic::subtract(operatorNode, leftNum, rightNum);
        if (value)
        {
            std::cout << leftNum->getFullStr() << "-" << rightNum->getFullStr() << " = " << value->getFullStr() << "\n";
            operatorNode->removeLeftChild();
            operatorNode->removeRightChild();
            operatorNode->setToken(value);
            operatorNode->setDerivative(std::make_shared<ExpressionNode>(
                std::make_shared<Number>("0", 0)));
            return;
        }
    }
    else if (leftNum)
    {
        if (leftNum->equals(0))
        {
            TreeModifier::replaceWithRightChild(operatorNode);
            operatorNode->getToken()->flipSign();
        }
    }
    else if (rightNum)
    {
        if (rightNum->equals(0))
        {
            TreeModifier::replaceWithLeftChild(operatorNode);
        }
    }
}










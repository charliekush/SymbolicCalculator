#include "arithmetic.hpp"
#include "latex_converter.hpp"
#include "text_converter.hpp"
#include "tree_modifier.hpp"

#include <cmath>
#include <iostream>
#include "tree_fixer.hpp"
#include "operation.hpp"


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
    std::cout << left->getFullStr() << "/" << right->getFullStr()  << " = ";
    auto out = performOperation(divideOp, left, right, true); 
    std::cout << out->getFullStr() << "\n";
    return out;
}

std::shared_ptr<Number> Arithmetic::add(nodePtr node, numPtr left, numPtr right)
{
    auto addOp = [](double a, double b) { return a + b; };
    auto divideOp = [](double a, double b) { return a / b; };
    std::cout << left->getFullStr() << "+" << right->getFullStr()  << " = ";
    auto out = performOperation(addOp, left, right); 
    std::cout << out->getFullStr() << "\n";
    return out;

}

std::shared_ptr<Number> Arithmetic::subtract(nodePtr node, numPtr left,
                                                                numPtr right)
{
    auto subtractOp = [](double a, double b) { return a - b; };

    std::cout << left->getFullStr() << "-" << right->getFullStr()  << " = ";
    auto out = performOperation(subtractOp, left, right); 
    std::cout << out->getFullStr() << "\n";

    return out;
}

std::shared_ptr<Number>Arithmetic::multiply(nodePtr node, numPtr left,
                                                                numPtr right)
{
    auto multiplyOp = [](double a, double b) { return a * b; };
    

    std::cout << left->getFullStr() << "*" << right->getFullStr()  << " = ";
    auto out = performOperation(multiplyOp, left, right); 
    std::cout << out->getFullStr() << "\n";

    
    return out;
}

std::shared_ptr<Number> Arithmetic::power(nodePtr node, numPtr left,
                                                                numPtr right)
{
    if (left->equals(0) && right->equals(0))
    {
        throw std::runtime_error("Undefined arithmetic: 0^0");
    }

    auto powerOp = [](double a, double b) { return std::pow(a, b); };
    std::cout << left->getFullStr() << "&" << right->getFullStr()  << " = ";
    auto out = performOperation(powerOp, left, right); 
    std::cout << out->getFullStr() << "\n";

    
    return out;
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
    //std::cout << "Original: " << TextConverter::convertToText(original) <<  "\n"
    //        << "Simplified: " << TextConverter::convertToText(node) << "\n";
    
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
            //std::cout << leftNum->getFullStr() << "^" << rightNum->getFullStr() << " = " << value->getFullStr() << "\n";
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
            //std::cout << leftNum->getFullStr() << "*" << rightNum->getFullStr() << " = " << value->getFullStr() << "\n";
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
            return;
        }
        else if (leftNum->equals(1))
        {

            TreeModifier::replaceWithRightChild(operatorNode);
            return;
        }
    }
    else if (rightNum)
    {
        if (rightNum->equals(0))
        {
            setNodeToZero(operatorNode);
            return;
        }
        else if (rightNum->equals(1))
        {
            TreeModifier::replaceWithLeftChild(operatorNode);
            return;
        }
    }
    // x * x = x ^ 2
    if (TreeFixer::treesEqual(operatorNode->getLeft(), operatorNode->getRight()))
    {
        operatorNode->setToken(std::make_shared<Operator>("^"));
        operatorNode->setRight(std::make_shared<ExpressionNode>(
            std::make_shared<Number>("2", 2)));
            
    }
    // x ^ a * x = x ^ (a + 1)
    else if (operatorNode->getLeft()->getStr() == "^" &&
                TreeFixer::treesEqual(operatorNode->getLeft()->getLeft(),
                    operatorNode->getRight()))
    {
        operatorNode->setToken(std::make_shared<Operator>("^"));
        operatorNode->setLeft(plusOne(operatorNode->getLeft()->getRight()));
        operatorNode->swapChildren();
        
    }
    // x * x ^ a = x ^ (a + 1)
    else if (operatorNode->getRight()->getStr() == "^" &&
                TreeFixer::treesEqual(operatorNode->getRight()->getLeft(),
                    operatorNode->getLeft()))
    {
        operatorNode->setToken(std::make_shared<Operator>("^"));
        operatorNode->setRight(plusOne(operatorNode->getRight()->getRight()));
        
    }
    //x ^ a * x ^ b = x ^ (a + b)
    else if (operatorNode->getLeft()->getStr() == "^" &&
                operatorNode->getRight()->getStr() == "^" && 
                TreeFixer::treesEqual(operatorNode->getLeft()->getLeft(),
                                        operatorNode->getRight()->getLeft()))
    {
        operatorNode->setToken(std::make_shared<Operator>("^"));
        auto exponentLeft = operatorNode->getLeft()->getRight();
        
        operatorNode->setLeft(operatorNode->getLeft()->getLeft());

        auto exponentRight = operatorNode->getRight()->getRight();
        auto sum = Operation::add(exponentLeft,exponentRight);
        operatorNode->setRight(sum);
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
            //std::cout << leftNum->getFullStr() << "/" << rightNum->getFullStr() << " = " << value->getFullStr() << "\n";
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
            //std::cout << leftNum->getFullStr() << "+" << rightNum->getFullStr() << " = " << value->getFullStr() << "\n";
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
            return;
        }
    }
    else if (rightNum)
    {
        if (rightNum->equals(0))
        {
            TreeModifier::replaceWithLeftChild(operatorNode);
            return;
        }
    }
    /*
    // x + x = 2 * x
    if (operatorNode->getLeft()TreeFixer::treesEqual(,operatorNode->getRight())) return new Tree(new Tree("2"), "*", operatorNode->getRight()); 
    // x * a + x = 2 * x
    if (!strcmp(operatorNode->getLeft()->getStr(), "*") && operatorNode->getLeft()->getLeft()TreeFixer::treesEqual(,operatorNode->getRight()))
        return new Tree(operatorNode->getRight(), "*", simplify(new Tree(operatorNode->getLeft()->getRight(), "+", new Tree("1")))); // 
    // x * a + x = x * (a + 1)
    if (!strcmp(operatorNode->getRight()->getStr(), "*") && operatorNode->getRight()->getLeft()TreeFixer::treesEqual(,operatorNode->getLeft()))
        return new Tree(operatorNode->getLeft(), "*", simplify(new Tree(operatorNode->getRight()->getRight(), "+", new Tree("1")))); // x + x * a = x * (1 + a)
    
    // Combine terms with common factors: x * a + x * b = x * (a + b)
    if (!strcmp(operatorNode->getLeft()->getStr(), "*") && !strcmp(operatorNode->getRight()->getStr(), "*") && operatorNode->getLeft()->getLeft()TreeFixer::treesEqual(,operatorNode->getRight()->getLeft()))
        return new Tree(operatorNode->getLeft()->getLeft(), "*", simplify(new Tree(operatorNode->getLeft()->getRight(), "+", operatorNode->getRight()->getRight())));
        */
    // x + x = 2 * x
    if (TreeFixer::treesEqual(operatorNode->getLeft(),operatorNode->getRight()))
    {
        operatorNode->setToken(std::make_shared<Operator>("*"));
        operatorNode->setRight(std::make_shared<ExpressionNode>(
                std::make_shared<Number>("2", 2)));
    }
    // x * a + x = x * (a + 1)
    else if (operatorNode->getLeft()->getStr() == "*" &&  
            TreeFixer::treesEqual(operatorNode->getLeft()->getLeft(),
                                                operatorNode->getRight()))
    {
        operatorNode->setToken(std::make_shared<Operator>("*"));
        operatorNode->setLeft(plusOne(operatorNode->getLeft()->getRight()));
    }
    // a * x + x = x * (a + 1)
    else if (operatorNode->getLeft()->getStr() == "*" &&  
            TreeFixer::treesEqual(operatorNode->getLeft()->getRight(),
                                                operatorNode->getRight()))
    {
        operatorNode->setToken(std::make_shared<Operator>("*"));
        operatorNode->setLeft(plusOne(operatorNode->getLeft()->getLeft()));
    }
    // x + x * a  = x * (a + 1)
    else if (operatorNode->getLeft()->getStr() == "*" &&  
            TreeFixer::treesEqual(operatorNode->getRight()->getLeft(),
                                                operatorNode->getLeft()))
    {
        operatorNode->setToken(std::make_shared<Operator>("*"));
        operatorNode->setRight(plusOne(operatorNode->getRight()->getRight()));
    }
    // x + a * x = x * (a + 1)
    else if (operatorNode->getLeft()->getStr() == "*" &&  
            TreeFixer::treesEqual(operatorNode->getRight()->getRight(),
                                                operatorNode->getLeft()))
    {
        operatorNode->setToken(std::make_shared<Operator>("*"));
        operatorNode->setRight(plusOne(operatorNode->getRight()->getLeft()));
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
            return;
        }
    }
    else if (rightNum)
    {
        if (rightNum->equals(0))
        {
            TreeModifier::replaceWithLeftChild(operatorNode);
            return;
        }
        
    }

    if (TreeFixer::treesEqual(operatorNode->getLeft(),operatorNode->getRight()))
    {
        setNodeToZero(operatorNode);
    }
    
    if (operatorNode->getLeft()->getStr() == "*")   
    {
        //(x * a) - x = x * (a - 1)
        if (TreeFixer::treesEqual(operatorNode->getLeft()->getLeft(),
                                        operatorNode->getRight()))
        {
            nodePtr newLeft = operatorNode->getRight()->copyTree();
            nodePtr newRight = Operation::subtract(
                            operatorNode->getRight()->getLeft()->copyTree(),
                            std::make_shared<ExpressionNode>(
                                        std::make_shared<Number>("1",1)));

            operatorNode->setToken(std::make_shared<Operator>("*"));
            operatorNode->setLeft(newLeft);
            operatorNode->setRight(newRight);
            return;
        }
        //(a * x) - x = x * (a - 1)
        if(TreeFixer::treesEqual(operatorNode->getLeft()->getRight(),
                                        operatorNode->getRight()))
        {
            nodePtr newLeft = operatorNode->getRight()->copyTree();
            nodePtr newRight = Operation::subtract(
                                operatorNode->getRight()->copyTree(),
                                std::make_shared<ExpressionNode>(
                                        std::make_shared<Number>("1",1)));

            operatorNode->setToken(std::make_shared<Operator>("*"));
            operatorNode->setLeft(newLeft);
            operatorNode->setRight(newRight);
            return;
        }
    }
    

    //(x * a) - x = (a - 1) * x 
    if (operatorNode->getLeft()->getStr() == "*" &&  
            TreeFixer::treesEqual(operatorNode->getLeft()->getLeft(),
                                                operatorNode->getRight()))
    {
        operatorNode->setToken(std::make_shared<Operator>("*"));
        operatorNode->setLeft(minusOne(operatorNode->getLeft()->getLeft()));
    }
    // (a * x) - x = (a - 1) * x 
    else if (operatorNode->getLeft()->getStr() == "*" &&  
            TreeFixer::treesEqual(operatorNode->getLeft()->getRight(),
                                                operatorNode->getRight()))
    {
        operatorNode->setToken(std::make_shared<Operator>("*"));
        operatorNode->setLeft(minusOne(operatorNode->getLeft()->getRight()));
    }
    //x - (x * a) = x * (1 - a)
    else if (operatorNode->getLeft()->getStr() == "*" &&  
            TreeFixer::treesEqual(operatorNode->getLeft()->getLeft(),
                                                operatorNode->getRight()))
    {
        operatorNode->setToken(std::make_shared<Operator>("*"));
        operatorNode->setRight(oneMinus(operatorNode->getLeft()->getLeft()));
    }
    //x  - (a * x) = x * (1 - a)
    else if (operatorNode->getRight()->getStr() == "*" &&  
            TreeFixer::treesEqual(operatorNode->getRight()->getRight(),
                                                operatorNode->getRight()))
    {
        operatorNode->setToken(std::make_shared<Operator>("*"));
        operatorNode->setRight(oneMinus(operatorNode->getLeft()->getRight()));
    }
    
    else if (operatorNode->getRight()->getStr() == "*" 
            && operatorNode->getLeft()->getStr() == "*")
    {
        auto term1 = operatorNode->getLeft()->getLeft()->copyTree();
        auto term2 = operatorNode->getLeft()->getRight()->copyTree();
        auto term3 = operatorNode->getRight()->getLeft()->copyTree();
        auto term4 = operatorNode->getRight()->getRight()->copyTree();
        // x * a - x * b = x * (a - b)
        if (TreeFixer::treesEqual(term1, term3))
        {
            operatorNode->setToken(std::make_shared<Operator>("*"));
            operatorNode->setLeft(term1);
            operatorNode->setRight(Operation::subtract(term2,term4));
        }
        // x * a - b * x = x * (a - b)
        else if (TreeFixer::treesEqual(term1, term4))
        {
            operatorNode->setToken(std::make_shared<Operator>("*"));
            operatorNode->setLeft(term1);
            operatorNode->setRight(Operation::subtract(term2,term3));
        }
        // a * x - x * b = x * (a - b)
        else if (TreeFixer::treesEqual(term2, term3))
        {
            operatorNode->setToken(std::make_shared<Operator>("*"));
            operatorNode->setLeft(term2);
            operatorNode->setRight(Operation::subtract(term1,term4));
        }
        // a * x - b * x = x * (a - b)
        else if (TreeFixer::treesEqual(term2, term4))
        {
            operatorNode->setToken(std::make_shared<Operator>("*"));
            operatorNode->setLeft(term2);
            operatorNode->setRight(Operation::subtract(term1,term3));
        }
        
    }
    
}


std::shared_ptr<ExpressionNode> Arithmetic::plusOne(nodePtr original)
{
    auto root = std::make_shared<ExpressionNode>(
                                std::make_shared<Operator>("+"));
    auto one = std::make_shared<ExpressionNode>(
                                std::make_shared<Number>("1",1));
    root->setLeft(original->copyTree());
    root->setRight(one);

    return root;
}
std::shared_ptr<ExpressionNode> Arithmetic::minusOne(nodePtr original)
{
    auto root = std::make_shared<ExpressionNode>(
                                std::make_shared<Operator>("-"));
    auto one = std::make_shared<ExpressionNode>(
                                std::make_shared<Number>("1",1));
    root->setLeft(original->copyTree());
    root->setRight(one);

    return root;
}

std::shared_ptr<ExpressionNode> Arithmetic::oneMinus(nodePtr original)
{
    auto root = std::make_shared<ExpressionNode>(
                                std::make_shared<Operator>("-"));
    auto one = std::make_shared<ExpressionNode>(
                                std::make_shared<Number>("1",1));
    root->setRight(original->copyTree());
    root->setLeft(one);

    return root;
}









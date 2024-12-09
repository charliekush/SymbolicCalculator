#include "arithmetic.hpp"
#include "operation.hpp"
#include "lookup.hpp"
#include "text_converter.hpp"
#include "tree_fixer.hpp"

#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <memory>

class SimplificationTests : public ::testing::Test
{
protected:
    typedef std::shared_ptr<ExpressionNode> nodePtr;
    std::shared_ptr<ExpressionNode> expected;
    std::shared_ptr<ExpressionNode> actual;

    void SetUp() override {
        actual = nullptr;
        expected = nullptr;
    }
    nodePtr varNode(std::string str)
    {
        return std::make_shared<ExpressionNode>(
                                    std::make_shared<Variable>(str));
    }

    nodePtr numNode(std::string str, int n)
    {
        return std::make_shared<ExpressionNode>(
                                    std::make_shared<Number>(str,n));
    }

    nodePtr numNode(std::string str, double d)
    {
        return std::make_shared<ExpressionNode>(
                                    std::make_shared<Number>(str,d));
    }
    std::string nodeDiff(nodePtr n1, nodePtr n2)
    {
        std::string out;
        if (n1)
        {
            out +=  "Expected: " + TextConverter::convertToText(n1);
            out += "\t(" + Lookup::getTokenType(n1->getType()) + ")\n";
        }
        else
        {
            out += "(nullptr)\n";
        }
        
        if (n2)
        {
            out +=  "Actual: " + TextConverter::convertToText(n2);
            out += "\t(" + Lookup::getTokenType(n2->getType()) + ")\n";
        }
        else
        {
            out += "(nullptr)\n";
        }
        return out;
    }
    void checkTrees(const nodePtr& tree1, const nodePtr& tree2)
    {
        if (!tree1 && !tree2)
        {
            return;
        }
        ASSERT_FALSE(!tree1 || !tree2) << nodeDiff(tree1, tree2);

        TokenType type1 = tree1->getType();
        TokenType type2 = tree2->getType();

        ASSERT_EQ(type1, type2) << nodeDiff(tree1, tree2);
        if (type1 == TokenType::FUNCTION)
        {
            auto func1 = std::dynamic_pointer_cast<Function>(tree1->getToken());
            auto func2 = std::dynamic_pointer_cast<Function>(tree2->getToken());
            ASSERT_NE(func1->getStr() , func1->getStr()) 
                                            << nodeDiff(tree1, tree2);

            ASSERT_NE(func1->getSubscript() , func1->getSubscript())
                                                    << nodeDiff(tree1, tree2);
            
            checkTrees(func1->getSubExprTree(), func2->getSubExprTree());
            return;
        }
        if (type1 == TokenType::VARIABLE)
        {
            auto var1 = std::dynamic_pointer_cast<Variable>(tree1->getToken());
            auto var2 = std::dynamic_pointer_cast<Variable>(tree2->getToken());
            ASSERT_TRUE(var1->equals(var2))
                                         << nodeDiff(tree1, tree2);
            return;
        }
        ASSERT_EQ(tree1->getStr() , tree2->getStr());
        checkTrees(tree1->getLeft(), tree2->getLeft());
        checkTrees(tree1->getRight(), tree2->getRight());
    }
};

// x * x = x ^ 2
TEST_F(SimplificationTests, multiplication1)
{
    actual = Operation::times(varNode("x"),varNode("x"));
    expected = Operation::power(varNode("x"),numNode("2",2));
    TreeFixer::simplify(actual);
    checkTrees(expected, actual);
    std::cout << nodeDiff(expected, actual);
}

// x ^ a * x = x ^ (a + 1)
TEST_F(SimplificationTests, multiplication2)
{
    actual = Operation::times(Operation::power(varNode("x"),numNode("2",2)),
                                varNode("x"));
    expected = Operation::power(varNode("x"),numNode("3",3));
    TreeFixer::simplify(actual);
    checkTrees(expected, actual);
    std::cout << nodeDiff(expected, actual);
}

// x * x ^ a = x ^ (a + 1)
TEST_F(SimplificationTests, multiplication3)
{
    actual = Operation::times(varNode("x"),
            Operation::power(varNode("x"),numNode("2",2)));
    expected = Operation::power(varNode("x"),numNode("3",3));
    TreeFixer::simplify(actual);
    checkTrees(expected, actual);
    std::cout << nodeDiff(expected, actual);
}

//x ^ a * x ^ b = x ^ (a + b)
TEST_F(SimplificationTests, multiplication4)
{
    actual = Operation::times(
            Operation::power(varNode("x"),numNode("4",4)),
            Operation::power(varNode("x"),numNode("2",2)));
    expected = Operation::power(varNode("x"),numNode("6",6));
    std::cout << nodeDiff(expected, actual);
    TreeFixer::simplify(actual);
    std::cout << nodeDiff(expected, actual);
    checkTrees(expected, actual);
}


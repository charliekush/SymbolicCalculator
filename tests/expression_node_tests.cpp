/**
 * @file tokenizer_tests.cpp
 * @author Charles Kushelevsky (charliekushelevsky@gmail.com)
 * @brief Google Tests for tokenizer.cpp
 * @version 0.1
 * @date 2024-09-04
 */

#include "token.hpp"
#include "expression_node.hpp"
#include "tree_modifier.hpp"

#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <memory>




class NodeTests : public ::testing::Test
{
    
protected:

    typedef std::shared_ptr<ExpressionNode> notdePtr;

    std::string printNode();
};

TEST_F(NodeTests, replaceRootWithRightTest)
{
    notdePtr root;
    auto times = std::make_shared<Operator>("*");
    auto xToken = std::make_shared<Variable>("x");
    auto one = std::make_shared<Number>("1",1);
    root = std::make_shared<ExpressionNode>(times);
    
    auto left = std::make_shared<ExpressionNode>(one);
    auto right = std::make_shared<ExpressionNode>(xToken);


    root->setLeft(left);
    root->setRight(right);
    ASSERT_EQ(root->getFullStr(), "*");
    ASSERT_EQ(root->getLeft()->getFullStr(), "1");
    ASSERT_EQ(root->getRight()->getFullStr(), "x");
    TreeModifier::replaceWithRightChild(root);
    ASSERT_EQ(root->getFullStr(), "x") << "Root: " <<root->getFullStr() << "\n";
    ASSERT_EQ(root->getLeft(),nullptr);
    ASSERT_EQ(root->getRight(),nullptr);
}

// simplify x*3+1*x*7 into x * 3 + 7*x
TEST_F(NodeTests, nestedReplaceRootWithRightTest)
{
    
    auto xToken1 = std::make_shared<ExpressionNode>(std::make_shared<Variable>("x"));
    auto xToken2 = std::make_shared<ExpressionNode>(std::make_shared<Variable>("x"));
    auto one = std::make_shared<ExpressionNode>(std::make_shared<Number>("1",1));
    auto seven = std::make_shared<ExpressionNode>(std::make_shared<Number>("7",7));
    auto three = std::make_shared<ExpressionNode>(std::make_shared<Number>("3",3));
    auto xpower7 = std::make_shared<ExpressionNode>(std::make_shared<Operator>("^"));
    auto xminus3 = std::make_shared<ExpressionNode>(std::make_shared<Operator>("-"));
    auto root = std::make_shared<ExpressionNode>(std::make_shared<Operator>("+"));
    
    auto xpower7times1= std::make_shared<ExpressionNode>(std::make_shared<Operator>("*"));
    xpower7->setLeft(xToken1);
    xpower7->setRight(seven);

    xminus3->setLeft(three);
    xminus3->setRight(xToken2);


    xpower7times1->setLeft(one);
    xpower7times1->setRight(xpower7);

    root->setLeft(xminus3);
    root->setRight(xpower7times1);
    
    ASSERT_EQ(root->getFullStr(), "+");

    ASSERT_EQ(root->getLeft()->getFullStr(), "-");
    ASSERT_EQ(root->getLeft()->getLeft()->getFullStr(), "3");
    ASSERT_EQ(root->getLeft()->getRight()->getFullStr(), "x");


    ASSERT_EQ(root->getRight()->getFullStr(), "*");
    ASSERT_EQ(root->getRight()->getLeft()->getFullStr(), "1");
    ASSERT_EQ(root->getRight()->getRight()->getFullStr(), "^");
    ASSERT_EQ(root->getRight()->getRight()->getLeft()->getFullStr(), "x");
    ASSERT_EQ(root->getRight()->getRight()->getRight()->getFullStr(), "7");
    
    auto replaceNode = root->getRight();
    TreeModifier::replaceWithRightChild(replaceNode);

    
    ASSERT_EQ(root->getFullStr(), "+");

    ASSERT_EQ(root->getLeft()->getFullStr(), "-");
    ASSERT_EQ(root->getLeft()->getLeft()->getFullStr(), "3");
    ASSERT_EQ(root->getLeft()->getRight()->getFullStr(), "x");


    ASSERT_EQ(root->getRight()->getFullStr(), "^");
    
    ASSERT_EQ(root->getRight()->getLeft()->getFullStr(), "x");
    ASSERT_EQ(root->getRight()->getRight()->getFullStr(), "7");
}






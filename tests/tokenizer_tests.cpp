/**
 * @file gtest.cpp
 * @brief Google Tests for tokenizer.hpp
 */

#include "token.hpp"
#include "tokenizer.hpp"

#include "test_definitions.hpp"

#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <memory>



class TokenizerTests : public ::testing::Test
{
protected:
    
    std::unique_ptr<Tokenizer> parser;
    std::string input;
    std::vector<std::pair<std::string, TokenType>> expected;

    std::vector<std::shared_ptr<Token>> tokens;
    
    void SetUp() override {

        parser = nullptr;
        
    }
    /**
    * @brief TearDown, runs after every test. Clears test log. 
    */
    void TearDown() override {
        parser.release();
        input.clear();
        expected.clear();
        tokens.clear();
    }

    

    void check()
    {
        parser = std::make_unique<Tokenizer>(input);
        tokens = parser->tokenize();

        //ASSERT_EQ(expected.size(), tokens.size());

        int size = expected.size() > tokens.size() ? 
            tokens.size():expected.size();
        
        for (int idx = 0; idx < size; idx++)
        {
            ASSERT_EQ(tokens[idx].get()->getStr(), expected[idx].first) 
            << "Expected: "
            << expected[idx].first << ", "
            << TokenTypeLookup[expected[idx].second] << "\n"
            << "Actual: " 
            << tokens[idx].get()->getStr() << ", "
            << TokenTypeLookup[tokens[idx].get()->getType()] << "\n\n"
            << "Input:\t" << input << "\n"
            << "Output:\t" << parser->listOutput() << "\n"; 

            ASSERT_EQ(TokenTypeLookup[tokens[idx].get()->getType()], 
                        TokenTypeLookup[expected[idx].second])
            << "Expected"
            << expected[idx].first << ", "
            << TokenTypeLookup[expected[idx].second] << "\n"
            << "Actual: " 
            << tokens[idx].get()->getStr() << ", "
            << TokenTypeLookup[tokens[idx].get()->getType()] << "\n";
        }

    }

};

TEST_F(TokenizerTests, EmptyInput)
{
    input = "";

    expected.clear(); // No tokens expected
    check();
}


TEST_F(TokenizerTests, SingleCharacter)
{
    input = "x";

    expected.emplace_back("x", VARIABLE);
    check();
}

TEST_F(TokenizerTests, SingleOperator)
{
    input = "+";

    expected.emplace_back("+", OPERATOR);
    check();
}

TEST_F(TokenizerTests, WhitespaceHandling)
{
    input = "  x  +  2  *  y ";

    expected.emplace_back("x", VARIABLE);
    expected.emplace_back("+", OPERATOR);
    expected.emplace_back("2", NUMBER);
    expected.emplace_back("*", OPERATOR);
    expected.emplace_back("y", VARIABLE);
    check();
}


TEST_F(TokenizerTests, MultipleOperators)
{
    input = "*+/-";

    expected.emplace_back("*", OPERATOR);
    expected.emplace_back("+", OPERATOR);
    expected.emplace_back("/", OPERATOR);
    expected.emplace_back("-", OPERATOR);
    check();
}
TEST_F(TokenizerTests, ParenthesesMatching)
{
    input = "(a + b) * c";

    expected.emplace_back("(", LEFTPAREN);
    expected.emplace_back("a", VARIABLE);
    expected.emplace_back("+", OPERATOR);
    expected.emplace_back("b", VARIABLE);
    expected.emplace_back(")", RIGHTPAREN);
    expected.emplace_back("*", OPERATOR);
    expected.emplace_back("c", VARIABLE);
    check();
}


TEST_F(TokenizerTests, NestedParentheses)
{
    input = "((a + b) * (c - d))";

    expected.emplace_back("(", LEFTPAREN);
    expected.emplace_back("(", LEFTPAREN);
    expected.emplace_back("a", VARIABLE);
    expected.emplace_back("+", OPERATOR);
    expected.emplace_back("b", VARIABLE);
    expected.emplace_back(")", RIGHTPAREN);
    expected.emplace_back("*", OPERATOR);
    expected.emplace_back("(", LEFTPAREN);
    expected.emplace_back("c", VARIABLE);
    expected.emplace_back("-", OPERATOR);
    expected.emplace_back("d", VARIABLE);
    expected.emplace_back(")", RIGHTPAREN);
    expected.emplace_back(")", RIGHTPAREN);
    check();
}

TEST_F(TokenizerTests, FunctionAndVariable)
{
    input = "sin(x) + log(y)";

    expected.emplace_back("sin", FUNCTION);
    expected.emplace_back("(", LEFTPAREN);
    expected.emplace_back("x", VARIABLE);
    expected.emplace_back(")", RIGHTPAREN);
    expected.emplace_back("+", OPERATOR);
    expected.emplace_back("log", FUNCTION);
    expected.emplace_back("(", LEFTPAREN);
    expected.emplace_back("y", VARIABLE);
    expected.emplace_back(")", RIGHTPAREN);
    check();
}




TEST_F(TokenizerTests, Exponentiation)
{
    input = "a^b^c";

    expected.emplace_back("a", VARIABLE);
    expected.emplace_back("^", OPERATOR);
    expected.emplace_back("b", VARIABLE);
    expected.emplace_back("^", OPERATOR);
    expected.emplace_back("c", VARIABLE);
    check();
}

TEST_F(TokenizerTests, UnaryMinusPlus)
{
    input = "-x + +y";

    expected.emplace_back("-", OPERATOR);
    expected.emplace_back("x", VARIABLE);
    expected.emplace_back("+", OPERATOR);
    expected.emplace_back("+", OPERATOR);
    expected.emplace_back("y", VARIABLE);
    check();
}


TEST_F(TokenizerTests, InvalidExpression)
{
    input = "a + * b";

    expected.emplace_back("a", VARIABLE);
    expected.emplace_back("+", OPERATOR);
    expected.emplace_back("*", OPERATOR); // This could be treated as an error or invalid token
    expected.emplace_back("b", VARIABLE);
    check();
}




TEST_F(TokenizerTests, ComplexExpression)
{
    input = "-3 (a + b^2) / sin(x)";

    expected.emplace_back("-3", NUMBER);
    expected.emplace_back("*", OPERATOR);
    expected.emplace_back("(", LEFTPAREN);
    expected.emplace_back("a", VARIABLE);
    expected.emplace_back("+", OPERATOR);
    expected.emplace_back("b", VARIABLE);
    expected.emplace_back("^", OPERATOR);
    expected.emplace_back("2", NUMBER);
    expected.emplace_back(")", RIGHTPAREN);
    expected.emplace_back("/", OPERATOR);
    expected.emplace_back("sin", FUNCTION);
    expected.emplace_back("(", LEFTPAREN);
    expected.emplace_back("x", VARIABLE);
    expected.emplace_back(")", RIGHTPAREN);
    check();
}








TEST_F(TokenizerTests, CotMixedExpression)
{
    input = "-3x^yzcot(m)";

    expected.emplace_back("-3", NUMBER);
    expected.emplace_back("*", OPERATOR);
    expected.emplace_back("x", VARIABLE);
    expected.emplace_back("^", OPERATOR);
    expected.emplace_back("y", VARIABLE);
    expected.emplace_back("*", OPERATOR);
    expected.emplace_back("z", VARIABLE);
    expected.emplace_back("*", OPERATOR);
    expected.emplace_back("cot", FUNCTION);
    expected.emplace_back("(", LEFTPAREN);
    expected.emplace_back("m", VARIABLE);
    expected.emplace_back(")", RIGHTPAREN);
    
    check();
}



    
    





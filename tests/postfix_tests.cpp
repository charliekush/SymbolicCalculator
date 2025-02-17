/**
 * @file tokenizer_tests.cpp
 * @author Charles Kushelevsky (charliekushelevsky@gmail.com)
 * @brief Google Tests for postfix.cpp
 * @version 0.1
 * @date 2024-09-04
 */

#include "token.hpp"
#include "tokenizer.hpp"
#include "postfix.hpp"
#include "lookup.hpp"


#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <memory>



class PostFixTests : public ::testing::Test
{
protected:
    std::unique_ptr<Tokenizer> parser;
    std::unique_ptr<ShuntingYard> postfixConverter;
    std::string input;
    std::vector<std::string> expected;
    
    TokenVector tokens;
    TokenVector postfix;
    
    void SetUp() override {

        parser = nullptr;
        
    }
    /**
    * @brief TearDown, runs after every test. Clears test log. 
    */
    void TearDown() override {
        parser.release();
        postfixConverter.release();
        input.clear();
        expected.clear();
        tokens.clear();
    }

    

    void check()
    {
        parser = std::make_unique<Tokenizer>(input);
        tokens = parser->tokenize();
        
        postfixConverter = std::make_unique<ShuntingYard>(tokens);
        std::cout << "tokens: \"" << parser->listOutput() << "\"\n";
        postfix = TokenVector(postfixConverter->getPostfix().getVector());
        


        int size = expected.size() > postfix.size() ? 
            postfix.size():expected.size();
        
        for (int idx = 0; idx < size; idx++)
        {
            ASSERT_EQ(postfix[idx].get()->getFullStr(), expected[idx]) 
            << "Expected: "
            << expected[idx] << "\n"
            << "Actual: " 
            << postfix[idx].get()->getFullStr() << ", "
            << Lookup::getTokenType(postfix[idx].get()->getType()) << "\n\n"
            << "Input:\t" << input << "\n"
            << "Output:\t" << parser->listOutput() << "\n"; 

        
        }
    }
    

};

TEST_F(PostFixTests, SimpleAddition) {
    input = "3+5";
    expected = {"3", "5", "+"};
    check();
}

TEST_F(PostFixTests, SimpleSubtraction) {
    input = "10-2";
    expected = {"10", "2", "-"};
    check();
}

TEST_F(PostFixTests, UnaryMinus) {
    input = "-3+5";
    expected = {"-3", "5", "+"};
    check();
}

TEST_F(PostFixTests, UnaryMinusBeforeParenthesis) {
    input = "-(2+3)";
    expected = {"2", "3", "+", "-"};
    check();
}

TEST_F(PostFixTests, UnaryMinusAfterOperator) {
    input = "5*-3";
    expected = {"5", "-3", "*"};
    check();
}

TEST_F(PostFixTests, FunctionAndOperator) {
    input = "cos(0)+sin(0)";
    expected = { "cos(0)", "sin(0)", "+"};
    check();
}



TEST_F(PostFixTests, ComplexExpression) {
    input = "3+4*2/(1-5)^2^3";
    expected = {"3", "4", "2", "*", "1", "5", "-", "2", "3", "^", "^", 
                                                                    "/", "+"};
    check();
}



TEST_F(PostFixTests, EmptyInput) {
    input = "";
    expected = {};
    check();
}





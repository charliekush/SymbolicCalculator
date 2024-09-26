/**
 * @file tokenizer_tests.cpp
 * @author Charles Kushelevsky (charliekushelevsky@gmail.com)
 * @brief Google Tests for tokenizer.cpp
 * @version 0.1
 * @date 2024-09-04
 */

#include "token.hpp"
#include "tokenizer.hpp"
#include "token_queue.hpp"
#include "token_vector.hpp"
#include "lookup.hpp"

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

    std::string matched;

    std::vector<std::pair<std::string, TokenType>> expectedPairs;

    TokenVector actualTokens;
    TokenVector expectedTokens;

    std::shared_ptr<Function> function;
    std::shared_ptr<Variable> variable;
    std::shared_ptr<Number> number;
    std::shared_ptr<Operator> op;
    std::shared_ptr<TokenQueue> queue;

    void SetUp() override {

        parser = nullptr;
        queue = std::make_shared<TokenQueue>();
        matched = "";
    }

    /**
    * @brief TearDown, runs after every test. Clears test log. 
    */
    void TearDown() override {
        
        parser.release();
        input.clear();
        expectedPairs.clear();
        expectedTokens.clear();
        actualTokens.clear();
        queue->clear();

    }

    std::string msg(int idx)
    {
        std::string out = "\nExpected: "
            + expectedPairs[idx].first + ", "
            + Lookup::getTokenType(expectedPairs[idx].second) + "\n"
            + "Actual: " 
            + actualTokens[idx].get()->getFullStr() + ", "
            + Lookup::getTokenType(actualTokens[idx].get()->getType()) + "\n"+
            "Matched: " + matched + "\n" ;
        return out;
    }

    std::string msg2(int idx)
    {
        std::string out = "\nExpected: "
            + expectedTokens[idx]->getFullStr() + ", "
            + Lookup::getTokenType(expectedTokens[idx]->getType()) + "\n"
            + "Actual: " 
            + actualTokens[idx].get()->getFullStr() + ", "
            + Lookup::getTokenType(actualTokens[idx].get()->getType()) + "\n" +
            "Matched: " + matched + "\n" ;
        return out;
    }
    std::string sizeMsg()
    {
        std::string expectedStr = "";
        std::string actualStr = "";
        for (int idx = 0; idx < expectedTokens.size(); idx++)
        {
            if (idx != 0)
            {
                expectedStr += ", ";
            }
            expectedStr += "\"" + expectedTokens[idx]->getFullStr() + "\"";
        }
        for (int idx = 0; idx < actualTokens.size(); idx++)
        {
            if (idx != 0)
            {
                actualStr += ", ";
            }
            actualStr += "\"" + actualTokens[idx]->getFullStr() + "\"";
        }

         std::string out = "\nExpected: "
            + expectedStr + "\n"
            + "Actual: " 
            + actualStr + "\n"
            + "Matched: " + matched + "\n";
        return out;
    }

    void check()
    {
        parser = std::make_unique<Tokenizer>(input);
        actualTokens = parser->tokenize();

        

        int size = expectedPairs.size() > actualTokens.size() ? 
            actualTokens.size():expectedPairs.size();
        
        for (int idx = 0; idx < size; idx++)
        {
            ASSERT_EQ(actualTokens[idx].get()->getFullStr(), 
                                expectedPairs[idx].first) << msg(idx); 

            ASSERT_EQ(Lookup::getTokenType(actualTokens[idx].get()->getType()), 
                Lookup::getTokenType(expectedPairs[idx].second)) << msg(idx);
            if (matched != "")
            {
                matched += ", ";
            }
            matched += actualTokens[idx].get()->getFullStr();
            
        }
        ASSERT_EQ(expectedPairs.size(), actualTokens.size());

    }
    void checkFunction(std::shared_ptr<Token> expectedToken, 
                        std::shared_ptr<Token> actualToken, 
                        int idx)
    {
        std::shared_ptr<Function> expected = 
                    std::dynamic_pointer_cast<Function>(expectedToken);
        std::shared_ptr<Function> actual = 
                    std::dynamic_pointer_cast<Function>(actualToken);
        ASSERT_EQ(actual->isNegative(), expected->isNegative()) 
            << msg2(idx);
        ASSERT_EQ(actual->getStr(), expected->getStr()) 
            << msg2(idx);
        ASSERT_EQ(actual->getSubExpr(), expected->getSubExpr()) 
            << msg2(idx);
        ASSERT_EQ(actual->getFullStr(), expected->getFullStr()) 
            << msg2(idx);
        ASSERT_EQ(expected->getSubscript()->getStr(), 
                    actual->getSubscript()->getStr()) << msg2(idx);
        checkExactTokens(expected->getSubExpr()->getVector(), 
                        actual->getSubExpr()->getVector());
        
    }
    void checkNumber(std::shared_ptr<Token> expectedToken, 
                        std::shared_ptr<Token> actualToken, 
                        int idx)
    {
        std::shared_ptr<Number> expected = 
                    std::dynamic_pointer_cast<Number>(expectedToken);
        std::shared_ptr<Number> actual = 
                    std::dynamic_pointer_cast<Number>(actualToken);
        
        ASSERT_EQ(actual->isNegative(), expected->isNegative()) 
            << msg2(idx);
        ASSERT_EQ(actual->getFullStr(), expected->getFullStr()) 
            << msg2(idx);
    }
    void checkVariable(std::shared_ptr<Token> expectedToken, 
                        std::shared_ptr<Token> actualToken, 
                        int idx)
    {
        std::shared_ptr<Variable> expected = 
                    std::dynamic_pointer_cast<Variable>(expectedToken);
        std::shared_ptr<Variable> actual = 
                    std::dynamic_pointer_cast<Variable>(actualToken);
        ASSERT_EQ(actual->isNegative(), expected->isNegative()) 
            << msg2(idx);
        ASSERT_EQ(actual->getStr(), expected->getStr()) 
            << msg2(idx);
        ASSERT_EQ(actual->getSubscript(), expected->getSubscript()) 
            << msg2(idx);
        ASSERT_EQ(actual->getFullStr(), expected->getFullStr()) 
            << msg2(idx);
        
    }
    
    void checkExactTokens()
    {
        parser = std::make_unique<Tokenizer>(input);
        actualTokens = parser->tokenize();
        expectedTokens = TokenVector(queue->getVector());
        checkExactTokens(expectedTokens, actualTokens);
    }
    void checkExactTokens(std::vector<std::shared_ptr<Token>> expectedVec, 
                    std::vector<std::shared_ptr<Token>> actualVec)
    {
        checkExactTokens(TokenVector(expectedVec),TokenVector(actualVec));
    }                
    void checkExactTokens(TokenVector expectedVec, TokenVector actualVec)
    {
        TokenVector expectedTempVec = 
                this->expectedTokens;
        TokenVector actualTempVec = 
                this->actualTokens;
        this->expectedTokens = expectedVec;
        this->actualTokens = actualVec;

        int size = expectedVec.size() > actualVec.size() ? 
            actualVec.size():expectedVec.size();
        
        for (int idx = 0; idx < size; idx++)
        {
            std::shared_ptr<Token>& expectedToken = expectedVec[idx];
            std::shared_ptr<Token>& actualToken = actualVec[idx];

            ASSERT_EQ(Lookup::getTokenType(actualToken->getType()), 
                Lookup::getTokenType(expectedToken->getType())) << msg2(idx);
            if (matched != "")
            {
                matched += ", ";
            }
            matched += actualToken->getFullStr();
            
            checkExactToken(expectedToken, expectedToken, idx);
            
        }
        this->expectedTokens = expectedTempVec;
        this->actualTokens = actualTempVec;
        ASSERT_EQ(expectedVec.size(), actualVec.size()) << sizeMsg();

    }
    void checkExactToken(std::shared_ptr<Token>& expectedToken,
                        std::shared_ptr<Token>& actualToken,
                        int idx)
    {
        ASSERT_EQ(Lookup::getTokenType(actualToken->getType()), 
                Lookup::getTokenType(expectedToken->getType())) << msg2(idx);
            
        if (actualToken->getType() == TokenType::FUNCTION)
        {
            checkFunction(expectedToken, actualToken, idx);
        }
        else if (actualToken->getType() == TokenType::VARIABLE)
        {
            checkVariable(expectedToken, actualToken, idx);
        }
        
    }

};

TEST_F(TokenizerTests, EmptyInput)
{
    input = "";

    expectedPairs.clear(); // No actualTokens expected
    check();
}


TEST_F(TokenizerTests, SingleCharacter)
{
    input = "x";

    expectedPairs.emplace_back("x", TokenType::VARIABLE);
    check();
}

TEST_F(TokenizerTests, SingleOperator)
{
    input = "+";

    expectedPairs.emplace_back("+", TokenType::OPERATOR);
    check();
}

TEST_F(TokenizerTests, WhitespaceHandling)
{
    input = "  x  +  2  *  y ";

    expectedPairs.emplace_back("x", TokenType::VARIABLE);
    expectedPairs.emplace_back("+", TokenType::OPERATOR);
    expectedPairs.emplace_back("2", TokenType::NUMBER);
    expectedPairs.emplace_back("*", TokenType::OPERATOR);
    expectedPairs.emplace_back("y", TokenType::VARIABLE);
    check();
}

//! TODO: Implement error handling
/*TEST_F(TokenizerTests, MultipleOperators)
{
    input = "*+/-";

    expectedPairs.emplace_back("*",  TokenType::OPERATOR);
    expectedPairs.emplace_back("+", TokenType::OPERATOR);
    expectedPairs.emplace_back("/", TokenType::OPERATOR);
    expectedPairs.emplace_back("-", TokenType::OPERATOR);
    check();
}*/
TEST_F(TokenizerTests, ParenthesesMatching)
{
    input = "(a + b) * c";

    expectedPairs.emplace_back("(", TokenType::LEFTPAREN);
    expectedPairs.emplace_back("a", TokenType::VARIABLE);
    expectedPairs.emplace_back("+", TokenType::OPERATOR);
    expectedPairs.emplace_back("b", TokenType::VARIABLE);
    expectedPairs.emplace_back(")", TokenType::RIGHTPAREN);
    expectedPairs.emplace_back("*", TokenType::OPERATOR);
    expectedPairs.emplace_back("c", TokenType::VARIABLE);
    check();
}


TEST_F(TokenizerTests, NestedParentheses)
{
    input = "((a + b) * (c - d))";

    expectedPairs.emplace_back("(", TokenType::LEFTPAREN);
    expectedPairs.emplace_back("(", TokenType::LEFTPAREN);
    expectedPairs.emplace_back("a", TokenType::VARIABLE);
    expectedPairs.emplace_back("+", TokenType::OPERATOR);
    expectedPairs.emplace_back("b", TokenType::VARIABLE);
    expectedPairs.emplace_back(")", TokenType::RIGHTPAREN);
    expectedPairs.emplace_back("*", TokenType::OPERATOR);
    expectedPairs.emplace_back("(", TokenType::LEFTPAREN);
    expectedPairs.emplace_back("c", TokenType::VARIABLE);
    expectedPairs.emplace_back("-", TokenType::OPERATOR);
    expectedPairs.emplace_back("d", TokenType::VARIABLE);
    expectedPairs.emplace_back(")", TokenType::RIGHTPAREN);
    expectedPairs.emplace_back(")", TokenType::RIGHTPAREN);
    check();
}

TEST_F(TokenizerTests, FunctionAndVariable)
{
    input = "sin(x) + log(y)";

    expectedPairs.emplace_back("sin(x)", TokenType::FUNCTION);
    expectedPairs.emplace_back("+", TokenType::OPERATOR);
    expectedPairs.emplace_back("log(y)", TokenType::FUNCTION);
    check();
}




TEST_F(TokenizerTests, Exponentiation)
{
    input = "a^b^c";

    expectedPairs.emplace_back("a", TokenType::VARIABLE);
    expectedPairs.emplace_back("^", TokenType::OPERATOR);
    expectedPairs.emplace_back("b", TokenType::VARIABLE);
    expectedPairs.emplace_back("^", TokenType::OPERATOR);
    expectedPairs.emplace_back("c", TokenType::VARIABLE);
    check();
}

TEST_F(TokenizerTests, UnaryMinusPlus)
{
    input = "-x + +y";

    expectedPairs.emplace_back("-x", TokenType::VARIABLE);
    expectedPairs.emplace_back("+", TokenType::OPERATOR);
    expectedPairs.emplace_back("y", TokenType::VARIABLE);
    check();
}

//! TODO: Implement error handling
/*TEST_F(TokenizerTests, InvalidExpression)
{
    input = "a + * b";

    expectedPairs.emplace_back("a", TokenType::VARIABLE);
    expectedPairs.emplace_back("+", TokenType::OPERATOR);
    // This could be treated as an error or invalid token
    expectedPairs.emplace_back("*", TokenType::OPERATOR); 
    expectedPairs.emplace_back("b", TokenType::VARIABLE);
    check();
}*/




TEST_F(TokenizerTests, ComplexExpression)
{
    input = "-3 (a + b^2) / sin(x)";

    expectedPairs.emplace_back("-3", TokenType::NUMBER);
    expectedPairs.emplace_back("*", TokenType::OPERATOR);
    expectedPairs.emplace_back("(", TokenType::LEFTPAREN);
    expectedPairs.emplace_back("a", TokenType::VARIABLE);
    expectedPairs.emplace_back("+", TokenType::OPERATOR);
    expectedPairs.emplace_back("b", TokenType::VARIABLE);
    expectedPairs.emplace_back("^", TokenType::OPERATOR);
    expectedPairs.emplace_back("2", TokenType::NUMBER);
    expectedPairs.emplace_back(")", TokenType::RIGHTPAREN);
    expectedPairs.emplace_back("/", TokenType::OPERATOR);
    expectedPairs.emplace_back("sin(x)", TokenType::FUNCTION);
    
    check();
}





TEST_F(TokenizerTests, CotMixedExpression)
{
    input = "-3x^yzcot(m)";

    expectedPairs.emplace_back("-3", TokenType::NUMBER);
    expectedPairs.emplace_back("*", TokenType::OPERATOR);
    expectedPairs.emplace_back("x", TokenType::VARIABLE);
    expectedPairs.emplace_back("^", TokenType::OPERATOR);
    expectedPairs.emplace_back("y", TokenType::VARIABLE);
    expectedPairs.emplace_back("*", TokenType::OPERATOR);
    expectedPairs.emplace_back("z", TokenType::VARIABLE);
    expectedPairs.emplace_back("*", TokenType::OPERATOR);
    expectedPairs.emplace_back("cot(m)", TokenType::FUNCTION);
    
    check();
}

TEST_F(TokenizerTests, LogWithPowerSubScriptAndMultiTokenInput)
{
    input = "log_4^2(x+7)";

    auto subExpr = std::make_shared<TokenQueue>();
    auto subscript = std::make_shared<TokenQueue>();

    function = std::make_shared<Function>("log");
    subExpr->push(std::make_shared<LeftParenthesis>());
    
    subExpr->push(std::make_shared<Variable>("x"));
    subExpr->push(std::make_shared<Operator>("+"));
    subExpr->push(std::make_shared<Number>("7",7));
    subExpr->push(std::make_shared<RightParenthesis>());
    

    function->setSubExpr(subExpr);
    function->setSubscript(std::make_shared<Number>("4",4));

    queue->push(function);

    queue->push(std::make_shared<Operator>("^"));
    queue->push(std::make_shared<Number>("2",2));
    
    checkExactTokens();
}

/*

TEST_F(TokenizerTests, SinWithSingleVariable)
{
    input = "sinx+7";

    auto function = std::make_shared<Function>("sin");
    auto subExpr = std::make_shared<TokenQueue>();

    subExpr->push(std::make_shared<Variable>("x"));
    function->setSubExpr(subExpr);

    queue->push(function);
    queue->push(std::make_shared<Operator>("+"));
    queue->push(std::make_shared<Number>("7", 7));

    checkExactTokens();
}

TEST_F(TokenizerTests, SinWithParenthesizedExpression)
{
    input = "sin(x+7)";

    auto function = std::make_shared<Function>("sin");
    auto subExpr = std::make_shared<TokenQueue>();

    subExpr->push(std::make_shared<Variable>("x"));
    subExpr->push(std::make_shared<Operator>("+"));
    subExpr->push(std::make_shared<Number>("7", 7));

    function->setSubExpr(subExpr);
    queue->push(function);

    checkExactTokens();
}

TEST_F(TokenizerTests, SinWithPowerAndVariable)
{
    input = "sin^2x";

    auto function = std::make_shared<Function>("sin");
    auto subExpr = std::make_shared<TokenQueue>();

    subExpr->push(std::make_shared<Variable>("x"));
    function->setSubExpr(subExpr);

    queue->push(function);
    queue->push(std::make_shared<Operator>("^"));
    queue->push(std::make_shared<Number>("2", 2));

    checkExactTokens();
}

TEST_F(TokenizerTests, SinWithPowerAndParenthesizedExpression)
{
    input = "sin^2(x+7)";

    auto function = std::make_shared<Function>("sin");
    auto subExpr = std::make_shared<TokenQueue>();

    subExpr->push(std::make_shared<Variable>("x"));
    subExpr->push(std::make_shared<Operator>("+"));
    subExpr->push(std::make_shared<Number>("7", 7));

    function->setSubExpr(subExpr);
    queue->push(function);
    queue->push(std::make_shared<Operator>("^"));
    queue->push(std::make_shared<Number>("2", 2));

    checkExactTokens();
}

TEST_F(TokenizerTests, LogWithSubscriptPowerAndVariable)
{
    input = "log_4^2x+7";

    auto function = std::make_shared<Function>("log");
    auto subExpr = std::make_shared<TokenQueue>();

    subExpr->push(std::make_shared<Variable>("x"));
    function->setSubExpr(subExpr);
    function->setSubscript(std::make_shared<Number>("4", 4));

    queue->push(function);
    queue->push(std::make_shared<Operator>("^"));
    queue->push(std::make_shared<Number>("2", 2));
    queue->push(std::make_shared<Operator>("+"));
    queue->push(std::make_shared<Number>("7", 7));

    checkExactTokens();
}

TEST_F(TokenizerTests, LogWithPowerSubscriptAndVariable)
{
    input = "log^2_4x+7";

    auto function = std::make_shared<Function>("log");
    auto subExpr = std::make_shared<TokenQueue>();

    subExpr->push(std::make_shared<Variable>("x"));
    function->setSubExpr(subExpr);
    function->setSubscript(std::make_shared<Number>("4", 4));

    queue->push(function);
    queue->push(std::make_shared<Operator>("^"));
    queue->push(std::make_shared<Number>("2", 2));
    queue->push(std::make_shared<Operator>("+"));
    queue->push(std::make_shared<Number>("7", 7));

    checkExactTokens();
}

TEST_F(TokenizerTests, LogWithSubscriptPowerAndParenthesizedExpression)
{
    input = "log_4^2(x+7)";

    auto function = std::make_shared<Function>("log");
    auto subExpr = std::make_shared<TokenQueue>();

    subExpr->push(std::make_shared<Variable>("x"));
    subExpr->push(std::make_shared<Operator>("+"));
    subExpr->push(std::make_shared<Number>("7", 7));

    function->setSubExpr(subExpr);
    function->setSubscript(std::make_shared<Number>("4", 4));

    queue->push(function);
    queue->push(std::make_shared<Operator>("^"));
    queue->push(std::make_shared<Number>("2", 2));

    checkExactTokens();
}

TEST_F(TokenizerTests, LogWithPowerSubscriptAndParenthesizedExpression)
{
    input = "log^2_4(x+7)";

    auto function = std::make_shared<Function>("log");
    auto subExpr = std::make_shared<TokenQueue>();

    subExpr->push(std::make_shared<Variable>("x"));
    subExpr->push(std::make_shared<Operator>("+"));
    subExpr->push(std::make_shared<Number>("7", 7));

    function->setSubExpr(subExpr);
    function->setSubscript(std::make_shared<Number>("4", 4));

    queue->push(function);
    queue->push(std::make_shared<Operator>("^"));
    queue->push(std::make_shared<Number>("2", 2));

    checkExactTokens();
}
*/
/**
 * @file derivative_tests.cpp
 * @author Charles Kushelevsky (charliekushelevsky@gmail.com)
 * @brief tests derivatives through the data set from
 * https://github.com/rbarket/GenInt/tree/main
 * @version 0.1
 * @date 2024-11-20
 */
#include "token.hpp"
#include "tokenizer.hpp"
#include "postfix.hpp"
#include "lookup.hpp"
#include "text_converter.hpp"
#include "log.hpp"
#include "derivative.hpp"
#include "approx.hpp"


#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <memory>
#include <random>
#include <chrono>
#include <vector>
#include <cmath>


class DerivativeTests : public ::testing::Test
{
protected:
    typedef std::shared_ptr<ExpressionNode> nodePtr;
    
    nodePtr actualOutput;
    nodePtr expectedOutput;
    std::string input;
    std::string expecteddDerivativeStr;
    std::vector<double> values;

    
    void generateValues()
    {
        std::mt19937_64 rng;
        uint64_t timeSeed = 
            std::chrono::high_resolution_clock::now().time_since_epoch().count();
        std::seed_seq ss{uint32_t(
                            timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
        rng.seed(ss);
        std::uniform_real_distribution<double> unif(0, 1);
       
        // ready to generate random numbers
        const int nSimulations = 10;
        int magnitude = 1;
        
        for (int i = 0; i < nSimulations; i++)
        {
            if (i % 2 == 0 && i > 0)
            {
                magnitude *= 10.0;
            }
            double randomNum = unif(rng);
            values.emplace_back(randomNum * magnitude);
        }
        values.emplace_back(M_PI);
        values.emplace_back(2 * M_PI);
        values.emplace_back(M_PI_2);
        values.emplace_back(M_PI_4);
        values.emplace_back(M_E);
    }
    void SetUp() override {
        if (values.size() == 0)
        {
            generateValues();
        }
        
        actualOutput = nullptr;
        expectedOutput = nullptr;
        input = "";
        expecteddDerivativeStr = "";
    }
    std::shared_ptr<ExpressionNode> getTree(std::string str)
    {
        Tokenizer parser(str);
        auto parsed = parser.tokenize();
        ShuntingYard converter(parsed);
        auto postfix = converter.getPostfix();
        return ExpressionNode::buildTree(postfix);
    }
    nodePtr getDerivative(Logger &log, std::string input, std::string wrt)
    {

        Derivative out(input, wrt);
        auto derivative = out.solve();
        log = out.log;
        return derivative;
    }
    void test()
    {
        std::string msg = "input: " + input  + "\n";
        auto var = std::make_shared<Variable>("x");
        Logger log(false);

        nodePtr inputTree = getTree(input);
        msg += "input tree: " + TextConverter::convertToText(inputTree)  + "\n";
        
        nodePtr expectedTree = getTree(expecteddDerivativeStr);
        msg += "expected derivative: " + expecteddDerivativeStr + "\n";
        msg += "expected: " + TextConverter::convertToText(expectedTree) + "\n";

        nodePtr derivative = getDerivative(log,input,"x");
        msg += "actual: " + TextConverter::convertToText(derivative) + "\n";
        values = {66.6690899873301};
        for (const auto &value : values)
        {
            double expected = Approx::approximate(expectedTree, var, value);
            double actual = Approx::approximate(derivative, var, value);

            ASSERT_TRUE(fabs(expected - actual ) < 0.001) 
                << msg
                << "value: " << value << "\n"
                << "actual: " << actual << "\n"
                << "expected: " << expected << "\n"
                << log.out();
            std::cout << "actual: " << actual << "\t"
                << "expected: " << expected << "\n";
        }
    }
    
};
TEST_F(DerivativeTests, func4)
{
	input = "ln(exp(x)-4)-x";
	expecteddDerivativeStr = "4/(exp(x)-4)";
	test();
}

/*TEST_F(DerivativeTests, func0)
{
    input = "2/3*ln(exp(x)+1/4*x+1/4)-2/3*x";
    expecteddDerivativeStr = "-2/3*x/(4*exp(x)+x+1)";
    test();
}



TEST_F(DerivativeTests, func1)
{
	input = "ln(exp(x)+6)-x";
	expecteddDerivativeStr = "-6/(exp(x)+6)";
	test();
}

TEST_F(DerivativeTests, func2)
{
	input = "ln(exp(x)-2)-x";
	expecteddDerivativeStr = "-4/(-2*exp(x)+4)";
	test();
}

TEST_F(DerivativeTests, func3)
{
	input = "ln(exp(x)+4)-x";
	expecteddDerivativeStr = "-4/(exp(x)+4)";
	test();
}



TEST_F(DerivativeTests, func5)
{
	input = "ln(exp(x)+4)-x";
	expecteddDerivativeStr = "-8/(2*exp(x)+8)";
	test();
}

TEST_F(DerivativeTests, func6)
{
	input = "ln(exp(x)-2)-x";
	expecteddDerivativeStr = "4/(2*exp(x)-4)";
	test();
}

TEST_F(DerivativeTests, func7)
{
	input = "4/(3*exp(x)+1)";
	expecteddDerivativeStr = "-4/(3*exp(x)+1)+4/(3*exp(x)+1)^2";
	test();
}

TEST_F(DerivativeTests, func8)
{
	input = "ln(exp(x)+1)-x";
	expecteddDerivativeStr = "2/(-2*exp(x)-2)";
	test();
}

TEST_F(DerivativeTests, func9)
{
	input = "ln(exp(x)+2)-x";
	expecteddDerivativeStr = "-4/(2*exp(x)+4)";
	test();
}

TEST_F(DerivativeTests, func10)
{
	input = "-ln(exp(x)+7)+x";
	expecteddDerivativeStr = "21/(3*exp(x)+21)";
	test();
}*/
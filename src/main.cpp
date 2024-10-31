#include "derivative.hpp"
#include "latex_converter.hpp"
#include "token_queue.hpp"
#include "arithmetic.hpp"
#include "log.hpp"
#include "approx.hpp"
#include "tokenizer.hpp"
#include "postfix.hpp"


#include <iostream>
#include <string>
#include <memory>
#include <utility>
#include <vector>
#include <cfloat>
#include <stdexcept>

#ifdef TEST_VERSION
#undef TEST_VERSION
#endif // !TEST_VERSION

#define TEST_VERSION 1


struct Options
{
    std::string function;
    std::string variable = "x"; // Default value
    std::string test = "";      // Default value
    double approximateValue = DBL_MAX; // Default value
};

Options parseArguments(const std::vector<std::string>& args) {
    Options options;
    bool functionSet = false;  // Track if function was explicitly set

    for (size_t i = 1; i < args.size(); ++i)
    {
        if (args[i] == "-f" || args[i] == "--function")
        {
            if (i + 1 < args.size())
            {
                options.function = args[i + 1];
                functionSet = true;
                ++i;
            }
            else
            {
                throw std::invalid_argument("Missing argument for --function");
            }
        }
        else if (args[i] == "-v" || args[i] == "--variable")
        {
            if (i + 1 < args.size())
            {
                options.variable = args[i + 1];
                ++i;
            }
            else
            {
                throw std::invalid_argument("Missing argument for --variable");
            }
        }
        else if (args[i] == "-t" || args[i] == "--test")
        {
            if (i + 1 < args.size())
            {
                options.test = args[i + 1];
                ++i;
            }
            else
            {
                throw std::invalid_argument("Missing argument for --test");
            }
        }
        else if (args[i] == "-n" || args[i] == "--approximate")
        {
            if (i + 1 < args.size())
            {
                options.approximateValue = std::stod(args[i + 1]);
                ++i;
            }
            else
            {
                throw std::invalid_argument(
                            "Missing argument for --approximate");
            }
        }
        else if (!functionSet && args[i][0] != '-')
        {
            options.function = args[i];
            functionSet = true;
        }
        else
        {
            throw std::invalid_argument("Unknown option: " + args[i]);
        }
    }

    // Ensure a function is set if not already
    if (!functionSet)
    {
        throw std::invalid_argument("Function argument is required.");
    }

    return options;
}



std::shared_ptr<ExpressionNode> getDerivative(Logger &log, std::string input,
                                                                std::string wrt)
{

    Derivative out(input, wrt);
    auto derivative = out.solve();
    log = out.log;
    return derivative;
}
std::shared_ptr<ExpressionNode> getTree(std::string input)
{

    Tokenizer parser(input);
    auto parsed = parser.tokenize();
    ShuntingYard converter(parsed);
    auto postfix = converter.getPostfix();
    return ExpressionNode::buildTree(postfix);
}
int main(int argc, char const* argv[])
{


    
    Arithmetic::floatSimplification = false;


    //std::string input =;

    std::string input;
    std::string wrt;
    std::string test_expr;
    double value;
    std::vector<double> values;

    #if TEST_VERSION
        //input = "ln(exp(x)-2*(2*x+3)/(5*x^2+x+4))";
        input = "x(ln(x)-1)";
        wrt = "x";
        test_expr = "";
        values = {"1.0","-1.0"};
    #else
    std::vector<std::string> args(argv, argv + argc);
    Options options;

    try
    {
        options = parseArguments(args);
    }
    catch (const std::invalid_argument& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    input = options.function;
    wrt = options.variable;
    test_expr = options.test;
    value = options.approximateValue;
    values.emplace_back(value);
    #endif // TEST_VERSION

    
    Logger log(false);
    auto derivative = getDerivative(log, input, wrt);

    Approx approximator(input, wrt, value);

    for (const double& v : values)
    {
        if (v != DBL_MAX)
        {
            auto var = std::make_shared<Variable>(wrt);
            double outValue = Approx::approximate(derivative, var, v);
            log.logApprox(v,outValue);
        }
    }
    if (test_expr != "")
    {
        
        auto var = std::make_shared<Variable>(wrt);
        auto testTree = getTree(test_expr);
        std::vector<double> values = {10,59, 1.1, 2958.0};
        bool same = true;
        for (const auto& value : values)
        {
            double expected = Approx::approximate(derivative, var, value);
            double actual = Approx::approximate(testTree, var, value);

            std::cout << value << ":\t" << expected << "\t" << actual << "\n";
            if (expected != actual)
            {
                same = false;
                
            }
        }
        
        
        log.logTest(test_expr,same);
    }
    
    std::cout << log.out() << "\n";
    //std::cout << values.first << "\t" << values.second << "\n";


    return 0;
}

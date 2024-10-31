#include "derivative.hpp"
#include "latex_converter.hpp"
#include "token_queue.hpp"
#include "arithmetic.hpp"
#include "log.hpp"
#include "approx.hpp"


#include <iostream>
#include <string>
#include <memory>
#include <utility>

#ifndef TEST_VERSION
    #define TEST_VERSION 0
#endif // !TEST_VERSION

Logger getDerivative(std::string input)
{
    
    Derivative out(input,"x");
    out.solve();
    return out.log;
}
int main(int argc, char const* argv[])
{
    

    // Set it to false
    Arithmetic::floatSimplification = false;
    
    #if TEST_VERSION
    std::string input = "ln(exp(x)-2*(2*x+3)/(5*x^2+x+4))";
    
    #else
    if (argc < 3)
    {
        std::cout << "Input needed\n";
        return 1;
    }
    std::string input = argv[1];
    #endif
    
    
    Logger log = getDerivative(input);
    Approx approximator(input,"x",9);
    auto values = approximator.approximate();
    std::cout << log.out() << "\n";
    //std::cout << values.first << "\t" << values.second << "\n";
    
    
    return 0;
}

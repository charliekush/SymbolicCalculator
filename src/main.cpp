#include "derivative.hpp"
#include "latex_converter.hpp"
#include "token_queue.hpp"
#include "calculator.hpp"



#include <iostream>
#include <string>
#include <memory>

std::string getDerivative(std::string input)
{
    
    Derivative out(input,"x");
    return LaTeXConverter::convertToLaTeX(out.solve());
}
int main(int argc, char const* argv[])
{
   std::cout << argc << "\n";
    if (argc == 1)
    {
        std::cout << "Input needed\n";
        return 1;
    }
    std::string input = argv[1];
    
    
    std::cout <<" \nInout: " << input << "\n";
    std::string out = getDerivative(input);
    std::cout <<" \nDerivative: " << out << "\n";

    return 0;
}

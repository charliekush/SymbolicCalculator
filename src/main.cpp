#include "token.hpp"
#include "tokenizer.hpp"
#include "postfix.hpp"


#include <iostream>
#include <string>
#include <memory>

int main(int argc, char const* argv[])
{
    std::string input = "(-3 (a + b^2) / sin(x)) * (-3x^yzcot(m))";
    Tokenizer parser(input);
    ShuntingYard converter(parser.tokenize());
    
    
    std::cout << "Hello World!\n";
    return 0;
}

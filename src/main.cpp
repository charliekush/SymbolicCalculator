#include "token.hpp"
#include "tokenizer.hpp"
#include "postfix.hpp"


#include <iostream>
#include <string>
#include <memory>

int main(int argc, char const* argv[])
{
    std::string input = "(-3 (a + b^2) / sin(x)) * (-3x^yzcot(m))";
    std::unique_ptr<Tokenizer> parser;
    std::unique_ptr<ShuntingYard> converter;

    parser = std::make_unique<Tokenizer>(input);
    converter = std::make_unique<ShuntingYard>(parser->tokenize());
    
    
    std::cout << "Hello World!\n";
    return 0;
}

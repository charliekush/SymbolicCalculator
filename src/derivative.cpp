
#include "derivative.hpp"
#include "token.hpp"
#include "tokenizer.hpp"
#include "postfix.hpp"

Derivative::Derivative(std::string input)
{
    Tokenizer parser(input);
    ShuntingYard converter(parser.tokenize());
    this->tree = ExpressionTree(converter.getPostfix());
}
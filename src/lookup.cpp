#include "lookup.hpp"

std::unordered_map<std::string, std::pair<TokenType, SymbolProperties>> 
    Lookup::symbolTable = {
    {"+", {TokenType::OPERATOR, {1, Associativity::LEFT}}},
    {"-", {TokenType::OPERATOR, {1, Associativity::LEFT}}},
    {"*", {TokenType::OPERATOR, {2, Associativity::LEFT}}},
    {"/", {TokenType::OPERATOR, {2, Associativity::LEFT}}},
    {"^", {TokenType::OPERATOR, {3, Associativity::RIGHT}}},
    {"sin", {TokenType::FUNCTION, {4, Associativity::NONE}}},
    {"cos", {TokenType::FUNCTION, {4, Associativity::NONE}}},
    {"tan", {TokenType::FUNCTION, {4, Associativity::NONE}}},
    {"cot", {TokenType::FUNCTION, {4, Associativity::NONE}}},
    {"csc", {TokenType::FUNCTION, {4, Associativity::NONE}}},
    {"sec", {TokenType::FUNCTION, {4, Associativity::NONE}}},
    {"exp", {TokenType::FUNCTION, {4, Associativity::NONE}}},
    {"log", {TokenType::FUNCTION, {5, Associativity::NONE}}},
    {"sqrt", {TokenType::FUNCTION, {5, Associativity::NONE}}},
    {"sqrt", {TokenType::FUNCTION, {5, Associativity::NONE}}},
    {"(", {TokenType::LEFTPAREN, {5, Associativity::NONE}}},
    {")", {TokenType::RIGHTPAREN, {5, Associativity::NONE}}},
    {"_", {TokenType::UNDERSCORE, {5, Associativity::NONE}}}
};
std::unordered_map<std::pair<TokenType,TokenType>, bool, PairHash> 
    Lookup::implicitMultiplication = {
    // LEFTPAREN cases
    {{TokenType::LEFTPAREN, TokenType::NUMBER}, false},
    {{TokenType::LEFTPAREN, TokenType::VARIABLE}, false},
    {{TokenType::LEFTPAREN, TokenType::FUNCTION}, false},
    {{TokenType::LEFTPAREN, TokenType::LEFTPAREN}, false},
    {{TokenType::LEFTPAREN, TokenType::RIGHTPAREN}, false},
    {{TokenType::LEFTPAREN, TokenType::OPERATOR}, false},
    {{TokenType::LEFTPAREN, TokenType::UNDERSCORE}, false},

    // RIGHTPAREN cases
    {{TokenType::RIGHTPAREN, TokenType::NUMBER}, true},
    {{TokenType::RIGHTPAREN, TokenType::VARIABLE}, true},
    {{TokenType::RIGHTPAREN, TokenType::FUNCTION}, true},
    {{TokenType::RIGHTPAREN, TokenType::LEFTPAREN}, false},
    {{TokenType::RIGHTPAREN, TokenType::RIGHTPAREN}, false},
    {{TokenType::RIGHTPAREN, TokenType::OPERATOR}, false},
    {{TokenType::RIGHTPAREN, TokenType::UNDERSCORE}, false},


    // NUMBER cases
    {{TokenType::NUMBER, TokenType::NUMBER}, true},
    {{TokenType::NUMBER, TokenType::VARIABLE}, true},
    {{TokenType::NUMBER, TokenType::FUNCTION}, true},
    {{TokenType::NUMBER, TokenType::LEFTPAREN}, true},
    {{TokenType::NUMBER, TokenType::RIGHTPAREN}, false},
    {{TokenType::NUMBER, TokenType::OPERATOR}, false},
    {{TokenType::NUMBER, TokenType::UNDERSCORE}, false},

    // VARIABLE cases
    {{TokenType::VARIABLE, TokenType::NUMBER}, true},
    {{TokenType::VARIABLE, TokenType::VARIABLE}, true},
    {{TokenType::VARIABLE, TokenType::FUNCTION}, true},
    {{TokenType::VARIABLE, TokenType::LEFTPAREN}, true},
    {{TokenType::VARIABLE, TokenType::RIGHTPAREN}, false},
    {{TokenType::VARIABLE, TokenType::OPERATOR}, false},
    {{TokenType::VARIABLE, TokenType::UNDERSCORE}, false},

    // FUNCTION cases
    {{TokenType::FUNCTION, TokenType::NUMBER}, true},
    {{TokenType::FUNCTION, TokenType::VARIABLE}, true},
    {{TokenType::FUNCTION, TokenType::FUNCTION}, true},
    {{TokenType::FUNCTION, TokenType::LEFTPAREN}, false},
    {{TokenType::FUNCTION, TokenType::RIGHTPAREN}, false},
    {{TokenType::FUNCTION, TokenType::OPERATOR}, false},
    {{TokenType::FUNCTION, TokenType::UNDERSCORE}, false},


    // UNDERSCORE cases
    {{TokenType::UNDERSCORE, TokenType::NUMBER}, false},
    {{TokenType::UNDERSCORE, TokenType::VARIABLE}, false},
    {{TokenType::UNDERSCORE, TokenType::FUNCTION}, false},
    {{TokenType::UNDERSCORE, TokenType::LEFTPAREN}, false},
    {{TokenType::UNDERSCORE, TokenType::RIGHTPAREN}, false},
    {{TokenType::UNDERSCORE, TokenType::OPERATOR}, false},
    

    // OPERATOR cases
    {{TokenType::OPERATOR, TokenType::NUMBER}, false},
    {{TokenType::OPERATOR, TokenType::VARIABLE}, false},
    {{TokenType::OPERATOR, TokenType::OPERATOR}, false},
    {{TokenType::OPERATOR, TokenType::FUNCTION}, false},
    {{TokenType::OPERATOR, TokenType::LEFTPAREN}, false},
    {{TokenType::OPERATOR, TokenType::RIGHTPAREN}, false},
    {{TokenType::OPERATOR, TokenType::UNDERSCORE}, false}
};

std::string Lookup::getTokenType(TokenType type) 
{
    switch (type)
    {
        case TokenType::NONE:       return "NONE";
        case TokenType::NUMBER:     return "NUMBER";
        case TokenType::VARIABLE:   return "VARIABLE";
        case TokenType::OPERATOR:   return "OPERATOR";
        case TokenType::FUNCTION:   return "FUNCTION";
        case TokenType::LEFTPAREN:  return "LEFTPAREN";
        case TokenType::RIGHTPAREN: return "RIGHTPAREN";
        case TokenType::UNDERSCORE: return "UNDERSCORE";
        case TokenType::STRING:     return "STRING";
        default:                    return "UNKNOWN";
    }
};
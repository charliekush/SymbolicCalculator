#include "text_converter.hpp"
#include "lookup.hpp"



#include "function_defs.hpp"  // Include your function definitions
#include <sstream>
#include <iostream>

std::string TextConverter::convertToText(nodePtr root)
{
    if (!root)
        return "";
    return nodeToText(root);
}

std::string TextConverter::nodeToText(nodePtr node)
{
    if (!node)
        return "";

    auto token = node->getToken();
    TokenType type = token->getType();
    //std::cout << "\nToken " << node->getFullStr() << "\n";
    //std::cout << "Type " << Lookup::getTokenType(type) << "\n";
    std::string out;
    if (TokenType::OPERATOR == type)
    {
        
        out = leftToText(node) + token->getStr() + rightToText(node);
    }
    else if (TokenType::FUNCTION == type )
    {
        out = functionToText(node);
    }
    else
    {
        out = node->getFullStr();
    }
    //std::cout << "Found " << out << "\n";
    return out;
}


std::string TextConverter::functionToText(nodePtr node)
{
    auto token = std::dynamic_pointer_cast<Function>(node->getToken());
    if (!token)
        return "";

    std::string func = token->getStr();
    if (token->getStr() == "exp")
    {
        func = "e^";
    }
    std::string argument = nodeToText(token->getSubExprTree());
    return func + parens(argument);
}

std::string TextConverter::leftToText(nodePtr node)
{
    
    if (node->getLeft()->getType() == TokenType::OPERATOR)
    {
        return parens(nodeToText(node->getLeft()));
    }
    return nodeToText(node->getLeft());
}

std::string TextConverter::rightToText(nodePtr node)
{   
    if (node->getRight()->getType() == TokenType::OPERATOR)
    {
        return parens(nodeToText(node->getRight()));
    }
    return nodeToText(node->getRight());
}

std::string TextConverter::parens(std::string str)
{
    return "(" + str + ")";
}
#include "latex_converter.hpp"
#include "lookup.hpp"


#include "latex_converter.hpp"
#include "function_defs.hpp"  // Include your function definitions
#include <sstream>
#include <iostream>

std::string LaTeXConverter::convertToLaTeX(std::shared_ptr<ExpressionNode> root)
{
    if (!root)
        return "";
    return nodeToLaTeX(root);
}

std::string LaTeXConverter::nodeToLaTeX(std::shared_ptr<ExpressionNode> node)
{
    if (!node)
        return "";

    auto token = node->getToken();
    TokenType type = token->getType();
    std::cout << "\nToken " << node->getFullStr() << "\n";
    std::cout << "Type " << Lookup::getTokenType(type) << "\n";
    std::string out;
    if (TokenType::OPERATOR == type)
    {
        std::string op = token->getStr();
        std::stringstream latex;
        if (op == "+")
        {
            latex << nodeToLaTeX(node->getLeft()) 
                    << " + " 
                    << nodeToLaTeX(node->getRight());
        }
        else if (op == "-")
        {
            latex << nodeToLaTeX(node->getLeft()) 
                    << " - " 
                    << nodeToLaTeX(node->getRight());
        }
        else if (op == "*") 
        {
            latex << nodeToLaTeX(node->getLeft()) 
                    << " \\cdot " 
                    << nodeToLaTeX(node->getRight());
        }
        else if (op == "/") 
        {
            latex << "\\dfrac{" << nodeToLaTeX(node->getLeft()) << "}"
                    << "{" << nodeToLaTeX(node->getRight()) << "}";
        }
        else if (op == "^")  
        {
            latex << nodeToLaTeX(node->getLeft()) 
                    << "^{" << nodeToLaTeX(node->getRight()) << "}";
        }
        out =  latex.str();
    }
    else if (TokenType::FUNCTION == type )
    {
        out = functionToLaTeX(node);
    }
    else
    {
        out =  "{" + node->getFullStr() + "}";
    }
    std::cout << "Found " << out << "\n";
    return out;
}


std::string LaTeXConverter::functionToLaTeX(std::shared_ptr<ExpressionNode> 
                                                                        node)
{
    auto token = std::dynamic_pointer_cast<Function>(node->getToken());
    if (!token)
        return "";

    std::string funcName = token->getStr();
    std::string latexFunc;

    
    if (funcName == "sin")
        latexFunc = "\\sin";
    else if (funcName == "cos")
        latexFunc = "\\cos";
    else if (funcName == "tan")
        latexFunc = "\\tan";
    else if (funcName == "cot")
        latexFunc = "\\cot";
    else if (funcName == "csc")
        latexFunc = "\\csc";
    else if (funcName == "sec")
        latexFunc = "\\sec";
    else if (funcName == "exp")
        latexFunc = "\\exp";
    else if (funcName == "ln")
        latexFunc = "\\ln";
    else if (funcName == "sqrt")
        latexFunc = "\\sqrt";
    else
        latexFunc = "\\" + funcName;  // Default case, add backslash

    // Check if the function has a parent operator that is an exponent
    auto parent = node->getParent().lock();
    if (parent && parent->getToken()->getType() == 
            TokenType::OPERATOR && parent->getToken()->getStr() == "^")
    {
        std::stringstream latex;
        latex << latexFunc << "^{" << nodeToLaTeX(parent->getRight()) << "}";
        latexFunc = latex.str();
    }

    // Function's argument inside \left( and \right)
    std::string argument = nodeToLaTeX(token->getSubExprTree());
    return latexFunc + "\\left(" + argument + "\\right)";
}

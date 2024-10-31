#ifndef __LATEX_CONVERTER_HPP__
#define __LATEX_CONVERTER_HPP__

#include "expression_node.hpp"
#include <string>
#include <memory>

class LaTeXConverter
{
public:
    // Converts the entire expression tree into a LaTeX string
    static std::string convertToLaTeX(std::shared_ptr<ExpressionNode> root);

private:
    // Helper function to convert a single node to LaTeX
    static std::string nodeToLaTeX(std::shared_ptr<ExpressionNode> node);

    // Function to handle LaTeX formatting for functions like sin, cos, etc.
    static std::string functionToLaTeX(std::shared_ptr<ExpressionNode> node);
    static std::string parens(std::string str);
};

#endif // __LATEX_CONVERTER_HPP__

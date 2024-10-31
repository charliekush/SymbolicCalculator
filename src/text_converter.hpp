#ifndef __TEXT_CONVERTER_HPP__
#define __TEXT_CONVERTER_HPP__


#include "expression_node.hpp"
#include <string>
#include <memory>


class TextConverter
{
private:
    typedef std::shared_ptr<ExpressionNode> nodePtr;
    // Helper function to convert a single node to Text
    static std::string nodeToText(nodePtr node);

    // Function to handle Text formatting for functions like sin, cos, etc.
    static std::string functionToText(nodePtr node);
    static std::string leftToText(nodePtr node);
    static std::string rightToText(nodePtr node);
    static std::string parens(std::string str);
public:
    // Converts the entire expression tree into a Text string
    static std::string convertToText(nodePtr root);


};
#endif // __TEXT_CONVERTER_HPP__
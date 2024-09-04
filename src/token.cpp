/**
 * @file token.cpp
 * @author Charlie Kushelevsky (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-08-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "token.hpp"

TokenType Token::getType() const
{
    return type;
} 

std::string Token::getStr() const
{
    return str;
} 
/**
 * @brief Default constructor for SymbolProperties.
 * Initializes precedence to -1 and associativity to LEFT.
 */
SymbolProperties::SymbolProperties()
{
    this->precedence = -1;
    this->associativity = Associativity::LEFT;
}

/**
 * @brief Constructs SymbolProperties with specified precedence and associativity.
 * @param precedence The precedence of the operator.
 * @param associativity The associativity of the operator.
 */
SymbolProperties::SymbolProperties(int precedence, Associativity associativity):
    precedence(precedence), associativity(associativity) {}

/**
 * @brief Constructs a Token with specified type and string.
 * @param t The type of the token.
 * @param s The string representation of the token.
 */
Token::Token(TokenType type, const std::string& str) : type(type), str(str) {}

/**
 * @brief Constructs an Operator with a specified string and properties.
 * @param str The string representation of the operator.
 * @param properties The properties of the operator.
 */
Operator::Operator(const std::string& str, SymbolProperties properties) : 
        Token(OPERATOR, str), properties(properties) {}

/**
 * @brief Constructs an Operator with a specified string and properties.
 * @param str The string representation of the operator.
 */
Operator::Operator(const std::string& str) : 
        Token(OPERATOR, str) 
{
    properties = symbolTable[str].second;
}


/**
 * @brief Gets the precedence of the operator.
 * @return The precedence of the operator.
 */
int Operator::getPrecedence()
{
    return properties.precedence;
}

/**
 * @brief Gets the associativity of the operator.
 * @return The associativity of the operator.
 */
Associativity Operator::getAssociativity()
{
    return properties.associativity;
}

/**
 * @brief Constructs a Function with a specified string and properties.
 * @param str The string representation of the function.
 * @param properties The properties of the function.
 */
Function::Function(const std::string& str, SymbolProperties properties) : 
        Token(FUNCTION, str), properties(properties) {}


/**
 * @brief Constructs a Function with a specified string and properties.
 * @param str The string representation of the function.
 */
Function::Function(const std::string& str) : 
        Token(FUNCTION, str)
{
    properties = symbolTable[str].second;
}


/**
 * @brief Gets the precedence of the function.
 * @return The precedence of the function.
 */
int Function::getPrecedence()
{
    return properties.precedence;
}

/**
 * @brief Gets the associativity of the function.
 * @return The associativity of the function.
 */
Associativity Function::getAssociativity()
{
    return properties.associativity;
}

/**
 * @brief Constructs a Number with a specified string and double value.
 * @param str The string representation of the number.
 * @param value The numeric value (double).
 */
Number::Number(const std::string& str, double value) : 
        Token(NUMBER, str), value(value), type(NumberType::DOUBLE) {}

/**
 * @brief Constructs a Number with a specified string and integer value.
 * @param str The string representation of the number.
 * @param value The numeric value (integer).
 */
Number::Number(const std::string& str, int value) : 
        Token(NUMBER, str), value(value), type(NumberType::INTEGER) {}

/**
 * @brief Checks if the number is an integer.
 * @return True if the number is an integer, otherwise false.
 */
bool Number::isInt() const 
{
    return type == NumberType::INTEGER;
}

/**
 * @brief Checks if the number is a double.
 * @return True if the number is a double, otherwise false.
 */
bool Number::isDouble() const 
{
    return type == NumberType::DOUBLE;
}

/**
 * @brief Gets the integer value of the number.
 * @return The integer value.
 * @throws std::bad_variant_access If the number is not an integer.
 */
int Number::getInt() const 
{
    return std::get<int>(value);
}

/**
 * @brief Gets the double value of the number.
 * @return The double value.
 * @throws std::bad_variant_access If the number is not a double.
 */
double Number::getFloat() const 
{
    return std::get<double>(value);
}

void Number::flipSign() {
    if(this->str[0] == '-')
    {
        this->str.erase(this->str.begin());
    }
    else
    {
        this->str.insert(0, 1, '-');
    }
    // Use std::visit to apply the sign flip to the variant value
    std::visit([this](auto& val) {
         // Get the type of the variant value
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_integral_v<T>) {
            // Handle integral type (int)
            val = -val;
        } else if constexpr (std::is_floating_point_v<T>) {
            // Handle floating-point type (double)
            val = -val;
        }
    }, value);
}
LeftParenthesis::LeftParenthesis() : Token(LEFTPAREN, "(") {};


RightParenthesis::RightParenthesis() : Token(RIGHTPAREN, ")") {};

Variable::Variable(const std::string& str) : Token(VARIABLE, str) {};
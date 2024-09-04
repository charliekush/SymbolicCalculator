/**
 * @file token.hpp
 * @author Charles Kushelevsky (charliekushelevsky@gmail.com)
 * @brief Defines classes and enums for token representation in
 * symbolic calculations.
 * @version 0.1
 * @date 2024-08-31
 */

#ifndef __TOKEN_HPP__
#define __TOKEN_HPP__

#include <string>
#include <unordered_map>
#include <utility>
#include <variant>

enum TokenType
{
    NUMBER,
    VARIABLE,
    OPERATOR,
    FUNCTION,
    LEFTPAREN,
    RIGHTPAREN
};

enum class NumberType
{
    INTEGER,
    DOUBLE
};

/**
 * @brief Defines associativity for operators.
 */
enum class Associativity
{
    NONE,
    LEFT,
    RIGHT
};

/**
 * @brief Defines properties for operators, variables, and functions.
 */
class SymbolProperties
{
public:
    /**
     * @brief Default constructor for SymbolProperties.
     */
    SymbolProperties();

    /**
     * @brief Constructs SymbolProperties with a precedence and associativity.
     * @param precedence The precedence of the operator.
     * @param associativity The associativity of the operator.
     */
    SymbolProperties(int precedence, Associativity associativity);

    int precedence;                //!< The precedence of the operator
    Associativity associativity;   //!< Associativity of the operator
};

/**
 * @brief Base class for tokens.
 */
class Token
{
protected:
    //!< Type of the token
    TokenType type; 
    //! String representation of the token
    std::string str; 
    //! Properties of the token
    SymbolProperties properties;
    bool negative; 

public:
    /**
     * @brief Constructs a Token with a type and a string representation.
     * @param type The type of the token.
     * @param str The string representation of the token.
     */
    Token(TokenType type, const std::string& str);

    virtual ~Token() = default;

    /**
     * @brief Returns the type of the token.
     * @return The type of the token.
     */
    TokenType getType() const;

    /**
     * @brief Returns the string representation of the token.
     * @return The string representation of the token.
     */
    std::string getStr() const;
    
    /**
     * @brief Get the Precedence of a token
     * 
     * @return Precedence of a token
     */
    int getPrecedence();

    /**
     * @brief Get the Associativity of a token
     * 
     * @return Associativity of a token
     */
    Associativity getAssociativity();


    /**
     * @brief sets the isNegative flag to indicate this token represents a 
     * negative value.
     * @param value The value to set isNegative to.
     */
    void setNegative(bool value);

    /**
     * @brief Returns whether the token represents a negative value.
     * @return True if the token is negative, otherwise false.
     */
    bool isNegative() const;
};



static std::unordered_map<std::string,
    std::pair<TokenType, SymbolProperties>>
    symbolTable = {
    {"+", {TokenType::OPERATOR, {1, Associativity::LEFT}}},
    {"-", {TokenType::OPERATOR, {1, Associativity::LEFT}}},
    {"*", {TokenType::OPERATOR, {2, Associativity::LEFT}}},
    {"/", {TokenType::OPERATOR, {2, Associativity::LEFT}}},
    {"^", {TokenType::OPERATOR, {3, Associativity::RIGHT}}},
    {"sin", {TokenType::FUNCTION, {4, Associativity::LEFT}}},
    {"cos", {TokenType::FUNCTION, {4, Associativity::LEFT}}},
    {"tan", {TokenType::FUNCTION, {4, Associativity::LEFT}}},
    {"cot", {TokenType::FUNCTION, {4, Associativity::LEFT}}},
    {"csc", {TokenType::FUNCTION, {4, Associativity::LEFT}}},
    {"sec", {TokenType::FUNCTION, {4, Associativity::LEFT}}},
    {"exp", {TokenType::FUNCTION, {4, Associativity::LEFT}}},
    {"log", {TokenType::FUNCTION, {5, Associativity::LEFT}}},
    {"(", {TokenType::LEFTPAREN, {5, Associativity::LEFT}}},
    {")", {TokenType::RIGHTPAREN, {5, Associativity::LEFT}}},
};

/**
 * @brief Represents an operator token.
 */
class Operator : public Token
{
public:
    
    /**
     * @brief Constructs an Operator with a string representation
     * and properties.
     * @param str The string representation of the operator.
     */
    Operator(const std::string& str);
    
};

/**
 * @brief Represents a function token.
 */
class Function : public Token
{
private:
    SymbolProperties properties; //!< Properties of the function

public:
    
    /**
     * @brief Constructs a Function with a string representation and properties.
     * @param str The string representation of the function.
     */
    Function(const std::string& str);
    
};

/**
 * @brief Represents a function token.
 */
class Variable : public Token
{
public:
    /**
     * @brief Constructs a Function with a string representation and properties.
     * @param str The string representation of the function.
     * @param properties The properties of the function.
     */
    Variable(const std::string& str);
};

/**
 * @brief Represents a number token.
 */
class Number : public Token
{
private:
    //! Value of the number token
    std::variant<int, double> value;

public:
    //! Type of the number token (integer or double)
    NumberType type;

    /**
     * @brief Constructs a Number with a string representation and a
     * double value.
     * @param str The string representation of the number.
     * @param value The double value of the number.
     */
    Number(const std::string& str, double value);

    /**
     * @brief Constructs a Number with a string representation and
     * an integer value.
     * @param str The string representation of the number.
     * @param value The integer value of the number.
     */
    Number(const std::string& str, int value);

    //! Checks if the number token is an integer.
    bool isInt() const;

    //! Checks if the number token is a double.
    bool isDouble() const;

    //! Returns the integer value of the number token.
    int getInt() const;

    //! Returns the double value of the number token.
    double getFloat() const;

    void flipSign();


};


/**
 * @brief Represents a left parenthesis
 */
class LeftParenthesis : public Token
{
public:
    /**
     * @brief Constructs a '(' token
     */
    LeftParenthesis();
};

/**
 * @brief Represents a left parenthesis
 */
class RightParenthesis : public Token
{
public:
    /**
     * @brief Constructs a '(' token
     */
    RightParenthesis();
};

#endif // __TOKEN_HPP__

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
#include "lookup.hpp"
#include "token_queue.hpp"


 /**
  * @brief Constructs a Token with specified type and string.
  * @param t The type of the token.
  * @param s The string representation of the token.
  */
Token::Token(TokenType type, const std::string& str) : type(type), str(str)
{
    if (Lookup::symbolTable.find(str) != Lookup::symbolTable.end())
    {
        properties = Lookup::symbolTable[str].second;
    }
    else
    {
        properties = SymbolProperties(0, Associativity::NONE, false);
    }
    this->setNegative(false);
}


TokenType Token::getType() const
{
    return this->type;
}

std::string Token::getStr() const
{
    return this->str;
}
std::string Token::getFullStr()
{
    std::string out = this->isNegative() ? "-" : "";
    out += this->str;
    return  out;
}

/**
 * @brief Gets the precedence of the operator.
 * @return The precedence of the operator.
 */
int Token::getPrecedence()
{
    return properties.precedence;
}

/**
 * @brief Gets the associativity of the operator.
 * @return The associativity of the operator.
 */
Associativity Token::getAssociativity()
{
    return properties.associativity;
}
bool Token::isCommutative()
{
    return properties.commutative;
}
/**
 * @brief sets the isNegative flag to indicate this token represents a
 * negative value.
 * @param value The value to set isNegative to.
 */
void Token::setNegative(bool value)
{
    this->negative = value;
}

/**
 * @brief Returns whether the token represents a negative value.
 * @return True if the token is negative, otherwise false.
 */
bool Token::isNegative() const
{
    return this->negative;
}

void Token::flipSign()
{
    this->setNegative(!this->isNegative());
}

/**
 * @brief Default constructor for SymbolProperties.
 * Initializes precedence to -1 and associativity to LEFT.
 */
SymbolProperties::SymbolProperties()
{
    this->precedence = -1;
    this->associativity = Associativity::LEFT;
    this->commutative = false;
}

/**
 * @brief Constructs SymbolProperties with specified precedence and associativity.
 * @param precedence The precedence of the operator.
 * @param associativity The associativity of the operator.
 */
SymbolProperties::SymbolProperties(int precedence, Associativity associativity,
                bool commutative) : 
    precedence(precedence), associativity(associativity), 
        commutative(commutative) {}






/**
 * @brief Constructs an Operator with a specified string and properties.
 * @param str The string representation of the operator.
 */
Operator::Operator(const std::string& str) :
    Token(TokenType::OPERATOR, str)
{
    properties = Lookup::symbolTable[str].second;
}




/**
 * @brief Constructs a Function with a specified string and properties.
 * @param str The string representation of the function.
 */
Function::Function(const std::string& str) :
    Token(TokenType::FUNCTION, str) 
{
    this->subExpr = nullptr;
    this->subExprTree = nullptr;
    this->exponent = nullptr;
    this->subscript = nullptr;
    if (Lookup::symbolTable.find(str) != Lookup::symbolTable.end())
    {
        this->properties =  Lookup::symbolTable[str].second;
    }
    else
    {
        throw std::runtime_error("Function not found!");
    }
    
}

void Function::setSubscript(std::shared_ptr<Number> base)
{
    if (this->str != "log")
    {
        throw std::runtime_error( "Only log function can use subscripts");
    } 
}
void Function::setExponent(std::shared_ptr<TokenQueue> exponent)
{
    this->exponent = exponent;
}
std::shared_ptr<TokenQueue> Function::getExponent()
{
    return this->exponent;
}
/**
 * @brief sets the input of the function
 * 
 * @param queue of subexpression tokens
 */
void Function::setSubExpr(std::shared_ptr<TokenQueue> queue)
{
    this->subExpr = queue;
}

void Function::setSubExprTree(std::shared_ptr<ExpressionNode> tree)
{
    this->subExprTree = tree;
}

std::shared_ptr<Number> Function::getSubscript()
{
    return this->subscript;
}

std::shared_ptr<TokenQueue> Function::getSubExpr()
{
    return this->subExpr;
}
std::shared_ptr<ExpressionNode> Function::getSubExprTree()
{
    return this->subExprTree;
}


std::string Function::getFullStr()
{
    std::string out = this->isNegative() ? "-" : "";
    out += this->str;

    if (this->getSubscript() != nullptr)
    {
        out += "_{" + this->getSubscript()->getFullStr() + "}";
    }
    if (this->getExponent() && !this->getExponent()->empty())
    {
        out += "^{" + this->getExponent()->toString() + "}";
    }
    if (this->subExpr && !this->subExpr->empty())
    {
        out += "(" + this->subExpr->toString() + ")";
    }
    return  out;
}


/**
 * @brief Constructs a Number with a specified string and double value.
 * @param str The string representation of the number.
 * @param value The numeric value (double).
 */
Number::Number(const std::string& str, double value) :
    Token(TokenType::NUMBER, str), value(value), type(NumberType::DOUBLE) 
{
    if (value < 0)
    {
        this->flipSign();
    }
}

/**
 * @brief Constructs a Number with a specified string and integer value.
 * @param str The string representation of the number.
 * @param value The numeric value (integer).
 */
Number::Number(const std::string& str, int value) :
    Token(TokenType::NUMBER, str), value(value), type(NumberType::INTEGER) {
}

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
    int out = std::get<int>(value);
    if (this->isNegative())
    {
        out *= -1;
    }
    return out;
}

/**
 * @brief Gets the double value of the number.
 * @return The double value.
 * @throws std::bad_variant_access If the number is not a double.
 */
double Number::getDouble() const
{
    double out = std::get<double>(value);
    if (this->isNegative())
    {
        out *= -1.0;
    }
    return out;
}

void Number::flipSign() {
    this->setNegative(!this->isNegative());
    // Use std::visit to apply the sign flip to the variant value
    std::visit([this](auto& val) {
        // Get the type of the variant value
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_integral_v<T>)
        {
            // Handle integral type (int)
            val = -val;
        }
        else if constexpr (std::is_floating_point_v<T>)
        {
            // Handle floating-point type (double)
            val = -val;
        }
    }, value);
}
LeftParenthesis::LeftParenthesis() : Token(TokenType::LEFTPAREN, "(") {};


RightParenthesis::RightParenthesis() : Token(TokenType::RIGHTPAREN, ")") {};

Variable::Variable(const std::string& str) : Token(TokenType::VARIABLE, str) {};

void Variable::setSubscript(std::string substr)
{
    this->subscript = substr;
}

std::string Variable::getSubscript()
{
    return this->subscript;
}
std::string Variable::getFullStr()
{
    std::string out = "";
    if (this->isNegative())
    {
        out += "-";
    }
    out += this->str;
    if (!this->subscript.empty())
    {
        out += "_{" + this->subscript + "}";
    }
    return out;
}


bool Variable::equals(std::shared_ptr<Token> other)
{
    if (other->getType() != TokenType::VARIABLE)
    {
        return false;
    }
    auto otherVar = std::dynamic_pointer_cast<Variable>(other);
    bool same = (this->getStr() == otherVar->getStr());
    if (same)
    {
        same = (this->getSubscript() == otherVar->getSubscript());
    }
    return same;
}

bool Number::equals(int other)
{
    
    if (this->isInt())
    {
        return other == this->getInt();
    }
    else
    {
        return (other * 1.0) == this->getDouble();
    }
}

bool Number::equals(double other)
{
    
    if (!this->isInt())
    {
        return other == this->getDouble();
    }
    else
    {
        return other == this->getInt() * 1.0;
    }
}
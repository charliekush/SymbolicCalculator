/**
 * @file tokenizer.hpp
 * @author Charles Kushelevsky (charliekushelevsky@gmail.com)
 * @brief Provides functionality for tokenizing an input string into tokens.
 * @version 0.1
 * @date 2024-08-31
 */

#ifndef __TOKENIZER_HPP__
#define __TOKENIZER_HPP__

#include "token.hpp"
#include <vector>

/**
 * @brief Tokenizer class for parsing an input string into tokens.
 */
class Tokenizer {
public:
    /**
     * @brief Constructs a Tokenizer with the given input string.
     * @param input The input string to be tokenized.
     */
    Tokenizer(const std::string& input);

    /**
     * @brief Tokenizes the input string into a vector of Tokens.
     * @return A vector of Token objects.
     */
    std::vector<Token> tokenize();

private:
    std::string input; //!< The input string to be tokenized
    size_t currentIndex; //!< The current index in the input string
    char currentChar; //!< The current character being processed
    int len; //!< Length of the input string

    /**
     * @brief Peeks at the next character in the input string without 
     * advancing the index.
     * @return The next character in the input string.
     */
    char peek() const;

    /**
     * @brief Advances to the next character in the input string.
     */
    void getNext();

    std::vector<Token> parseExpression(); //!< Vector to hold parsed tokens

    /**
     * @brief Parses a number token from the input string.
     * @return The parsed Number token.
     */
    Token parseNumber();

    /**
     * @brief Parses an identifier token (variable or function) 
     * from the input string.
     * @return The parsed Token for the identifier.
     */
    Token parseIdentifier();

    /**
     * @brief Parses an operator token from the input string.
     * @return The parsed Operator token.
     */
    Token parseOperator();

    /**
     * @brief Parses a parenthesis token from the input string.
     * @return The parsed Token for the parenthesis.
     */
    Token parseParenthesis();
};

#endif // __TOKENIZER_HPP__

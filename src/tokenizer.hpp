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
#include "MWT.hpp"

#include <memory>
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
    std::vector<std::shared_ptr<Token>> tokenize();
    
    //DEBUG START
    std::string listOutput();
    //DEBUG END
    

private:
    std::string input; //!< The input string to be tokenized
    std::string substr;
    size_t currentIndex; //!< The current index in the input string
    char currentChar; //!< The current character being processed
    int len; //!< Length of the input string
    std::shared_ptr<MWTNode> mwtRoot;
    std::vector<std::shared_ptr<Token>> output;
    
    /**
     * @brief constructs multiway trie from symbolTable @see token.hpp
     */
    void constructTree();
    

    /**
     * @brief Peeks at the next character in the input string without 
     * advancing the index.
     * @return The next character in the input string. '\0' if last character 
     * in input.
     */
    char peek() const;

    /**
     * @brief Peeks at the next character in the input string without 
     * advancing the index.
     * @return The previous character in the input string. '\0' if first 
     * character in input.
     */
    char peekBack() const;


    /**
     * @brief Advances to the next character in the input string. '\0' if 
     * last character in input.
     */
    void getNext();

    /**
     * @brief splits substr into variables and clears substr
     * 
     */
    void clearSubstr();

    /**
     * @brief checks for symbols in substr
     * 
     */
    void checkSubstr();
    
    /**
     * @brief Get the Sub Sript of a str
     * 
     * @param idx index in the input
     * @return subscript string
     */
    std::string getSubSript(int idx);

    /**
     * @brief parse the expression
     * 
     * @return std::vector<std::shared_ptr<Token>> 
     */
    void parseExpression(); 
    
    void checkImplicitMultiplication();
    /**
     * @brief Parses a number token from the input string.
     * @return The parsed Number token.
     */
    Number parseNumber();

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

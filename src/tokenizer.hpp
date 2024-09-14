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
#include "token_queue.hpp"

#include <memory>
#include <vector>
#include <set>


 /**
  * @brief Tokenizer class for parsing an input string into tokens.
  */
class Tokenizer
{
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
    //! The input string to be tokenized
    std::string input;
    std::string substr;

    //! The current index in the input string
    size_t currentIndex;

    //! The current character being processed

    char currentChar;
    //! Length of the input string
    int len;
    std::shared_ptr<MWTNode> mwtRoot;
    std::vector<std::shared_ptr<Token>> output;
    int tokensIdx;


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
     * @brief parse the expression
     *
     * @return std::vector<std::shared_ptr<Token>>
     */
    void parseExpression();

    void nextImplicit();
    /**
     * @brief Parses a number token from the input string.
     * @return The parsed Number token.
     */
    Number parseNumber();


    
    

    /**
     * @brief Parses a parenthesis token from the input string.
     * @return The parsed Token for the parenthesis.
     */
    Token parseParenthesis();

    /**
     * @brief handles potential unary minus sign in input
     *
     */
    void handleUnary();
    
    void handleFunction();
    void handleVariable();
    void handleSubscript();
    std::shared_ptr<TokenQueue> getFunctionSubscript();
    std::shared_ptr<TokenQueue> getSubTokens();
    void fixEulers();
    std::shared_ptr<Token> currentToken() const;
    

};

#endif // __TOKENIZER_HPP__

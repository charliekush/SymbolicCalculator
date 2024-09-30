/**
 * @file tokenizer.cpp
 * @author Charles Kushelevsky (charliekushelevsky@gmail.com)
 * @brief Implementation of the Tokenizer class which handles tokenizing
 * input strings.
 * @version 0.1
 * @date 2024-08-31
 */

#include "tokenizer.hpp"
#include "token_queue.hpp"
#include "lookup.hpp"

#include <stdexcept>
#include <iostream>



 /**
  * @class Tokenizer
  * @brief A class that tokenizes an input string into tokens.
  *
  * The Tokenizer class processes the input string character by character to
  * identify and extract tokens. It keeps track of the current character and
  * index within the input string.
  */

  /**
   * @brief Constructs a Tokenizer object with a specified input string.
   * @param input The string to be tokenized.
   *
   * This constructor initializes the input string, sets the current index to
   * 0, and initializes the current character to the first character of the
   * input string. The length of the input string is also recorded.
   */
Tokenizer::Tokenizer(const std::string& input) : input(input)
{

    this->mwtRoot = this->mwtRoot->constructMWT();
    this->substr = "";
    if (!this->input.empty())
    {
        this->currentIndex = 0;
        this->currentChar = this->input[0];
    }
    else
    {
        this->currentIndex = -1;
        this->currentChar = '\0'; // Handle empty this->input
    }
    this->len = this->input.length();
}

TokenVector Tokenizer::tokenize()
{

    this->parseExpression();


    for (this->tokensIdx = 0; this->tokensIdx < this->output.size();
                                                    this->tokensIdx++)
    {

        if (this->currentToken()->getType() == TokenType::VARIABLE &&
            this->currentToken()->getStr() == "e")
        {
            fixEulers();

        }

        // check for unary operators
        if (this->currentToken()->getType() == TokenType::OPERATOR &&
            (this->currentToken()->getStr() == "-" ||
                this->currentToken()->getStr() == "+"))
        {
            this->handleUnary();
        }
        if (this->currentToken()->getType() == TokenType::VARIABLE)
        {
            this->handleVariable();
        }
        if (this->currentToken()->getType() == TokenType::FUNCTION)
        {
            this->handleFunction();
        }
        
        if (this->currentToken()->getType() == TokenType::FUNCTION)
        {
            std::shared_ptr<Function> func =
                std::dynamic_pointer_cast<Function>(this->currentToken());
            std::shared_ptr<TokenQueue> exponent = func->getExponent();
            if (exponent)
            {
                int exponentIdx = this->tokensIdx + 1;
                while (!exponent->empty())
                {
                    this->output.emplace(exponentIdx, exponent->pop());
                    exponentIdx++;

                }
            }
        }
    }
    for (this->tokensIdx = 0; this->tokensIdx < this->output.size();
                                                    this->tokensIdx++)
    {
        this->nextImplicit();
    }
    return this->output;
}
std::shared_ptr<Token> Tokenizer::currentToken()
{
    return this->output[this->tokensIdx];
}

char Tokenizer::peek() const
{
    int idx = this->currentIndex + 1;
    while (idx <= this->len)
    {
        if (this->input[idx] != ' ')
        {
            return this->input[idx];
        }
        idx++;
    }
    return '\0';
}

char Tokenizer::peekBack() const
{
    int idx = this->currentIndex - 1;
    while (idx > 0)
    {
        if (this->input[idx] != ' ')
        {
            return this->input[idx];
        }
        idx--;
    }
    return '\0';
}

void Tokenizer::getNext()
{
    this->currentIndex++;
    if (this->currentIndex == this->len)
    {
        this->currentChar = '\0';
    }
    else
    {
        this->currentChar = this->input[this->currentIndex];
    }
}

void Tokenizer::parseExpression()
{
    while (this->currentChar != '\0')
    {
        //this->listOutput();
        if (this->currentChar >= '0' && this->currentChar <= '9')
        {
            this->clearSubstr();
            this->output.emplace_back(
                    std::make_shared<Number>(this->parseNumber()));

        }
        else if (this->currentChar == ' ')
        {
            this->clearSubstr();
            this->getNext();

        }
        else
        {
            this->substr += this->currentChar;
            this->getNext();
        }
        this->checkSubstr();

    }
    // Handle any remaining this->substr (if needed)
    if (!this->substr.empty())
    {
        this->clearSubstr();
    }

}
void Tokenizer::handleUnary()
{


    bool unary = (this->tokensIdx == 0);

    // Check if the previous character was an operator or left parenthesis
    if (!unary)
    {
        std::shared_ptr<Token>& lastToken = this->output[this->tokensIdx - 1];
        unary = (lastToken->getType() == TokenType::OPERATOR) ||
            (lastToken->getType() == TokenType::LEFTPAREN);

    }


    if (unary)
    {
        if (this->output.size() > this->tokensIdx + 1)
        {
            if (!(this->output[this->tokensIdx + 1]->getType() ==
                TokenType::OPERATOR &&
                (this->output[this->tokensIdx - 1]->getType() ==
                    TokenType::OPERATOR)))
            {
                if (this->currentToken()->getStr() == "-")
                {
                    this->output[this->tokensIdx + 1]->flipSign();
                }
                this->output.erase(this->tokensIdx);
            }
            {
                //!TODO: error for triple operator
            }


        }
        else
        {
            //! TODO: add error for extra plus/minus
        }

    }

}
Number Tokenizer::parseNumber()
{
    std::string numberStr;
    bool hasDecimalPoint = false;
    bool sciNotation = false;

    while ((this->currentChar >= '0' && this->currentChar <= '9'))
    {
        numberStr += this->currentChar;
        if (this->peek() == '.')
        {
            if (!hasDecimalPoint)
            {
                hasDecimalPoint = true;
                numberStr += this->peek();
                this->getNext();
            }
            else
            {
                std::runtime_error("Multiple decimal points in number");
            }
        }
        this->getNext();
    }

    if (hasDecimalPoint)
    {
        double value = std::stod(numberStr); // Convert to double
        return Number(numberStr, value);
    }
    else
    {
        int value = std::stoi(numberStr); // Convert to integer
        return Number(numberStr, value);
    }
}

void Tokenizer::clearSubstr()
{
    int startIndexInInput = this->currentIndex - this->substr.length();
    int matchStartInInput = startIndexInInput;
    this->checkSubstr();
    for (int idx = 0; idx < this->substr.length(); idx++)
    {

        this->output.emplace_back(std::make_shared<Variable>(
            std::string(1, this->substr[idx])));



    }
    this->substr.clear();
}


void Tokenizer::checkSubstr()
{
    if (this->substr.empty())
        return;
    std::shared_ptr<MWTNode> current = mwtRoot;
    int lastMatchedIndex = -1;
    std::string matchedString;
    // Traverse the substring to find the longest match in the trie
    for (int idx = 0; idx < this->substr.length(); idx++)
    {

        char ch = this->substr[idx];
        if (current->getChild(ch))
        {
            current = current->getChild(ch);
            matchedString += ch;
            if (current->isWord)
            {
                lastMatchedIndex = idx;
                break;
            }
        }
        else
        {
            matchedString = "";
            continue;
        }
    }

    if (lastMatchedIndex != -1) // Found a match
    {
        int startIndexInInput = this->currentIndex - this->substr.length();
        int matchStartInInput = startIndexInInput + lastMatchedIndex -
            (matchedString.length() - 1);
        // Process characters before the matched string as variables
        for (int idx = 0; idx < lastMatchedIndex + 1 - matchedString.length();
                                                                    idx++)
        {


            this->output.emplace_back(
                std::make_shared<Variable>(std::string(1, this->substr[idx])));
        }

        // Process the matched string as a function/operator
        auto match = Lookup::symbolTable[matchedString];
        if (match.first == TokenType::FUNCTION)
        {
            this->output.emplace_back(
                                std::make_shared<Function>(matchedString));

        }
        else if (match.first == TokenType::OPERATOR)
        {
            this->output.emplace_back(
                                std::make_shared<Operator>(matchedString));
        }
        else if (match.first == TokenType::LEFTPAREN)
        {
            this->output.emplace_back(std::make_shared<LeftParenthesis>());
        }
        else if (match.first == TokenType::UNDERSCORE)
        {
            this->output.emplace_back(
                        std::make_shared<Token>(TokenType::UNDERSCORE, "_"));
        }
        else if (match.first == TokenType::RIGHTPAREN)
            this->output.emplace_back(std::make_shared<RightParenthesis>());
        // Remove processed part from this->substr
        this->substr.erase(0, lastMatchedIndex + 1);
    }
}

void Tokenizer::handleFunction()
{
    std::shared_ptr<Token>& token = this->output[this->tokensIdx];
    std::shared_ptr<Function> func = std::dynamic_pointer_cast<Function>(token);

    // Initialize a queue to store subexpression tokens
    std::shared_ptr<TokenQueue> subExpr = std::make_shared<TokenQueue>();
    std::shared_ptr<TokenQueue> subScript = std::make_shared<TokenQueue>();
    std::shared_ptr<TokenQueue> exponent = std::make_shared<TokenQueue>();
    bool hasParentheses = false;
    int functionIdx = this->tokensIdx;

    bool foundSubstring = false;
    bool foundExponent = false;
    bool foundSubExpr = false;

    this->tokensIdx++;

    // Check if function is given argument
    if (this->tokensIdx == this->output.size())
    {
        subExpr->push(std::make_shared<Number>("1", 1));
        foundSubExpr = true;
    }
    else if (this->currentToken()->getType() == TokenType::OPERATOR &&
                    this->currentToken()->getStr() != "^")
    {
        subExpr->push(std::make_shared<Number>("1", 1));
        foundSubExpr = true;
    }
    if (!foundSubExpr)
    {
        for (int counter = 0; counter < 2; counter++)
        {
            if (this->tokensIdx == this->output.size())
            {

                return;
            }
            if (!(this->currentToken()->getStr() == "^"
                || this->currentToken()->getType() == TokenType::UNDERSCORE))
            {
                break;
            }
            if (!foundSubstring &&
                    this->currentToken()->getType() == TokenType::UNDERSCORE)
            {
                if (func->getStr() != "log")
                {
                    std::string errorMsg =
                        "Only log function can have subscript, not \"" +
                        func->getStr() + "\"!";
                    std::runtime_error(errorMsg.c_str());
                }
                int underscoreIdx = this->tokensIdx++;
                if (this->tokensIdx == this->output.size())
                {
                    std::runtime_error("No subscript found!");
                    return;
                }
                subScript = this->getSubTokens();
                if (subScript->size() == 1 &&
                        subScript->top()->getType() == TokenType::NUMBER)
                {
                    func->setSubscript(
                        std::dynamic_pointer_cast<Number>(subScript->top()));

                }
                else
                {
                    std::runtime_error(
                        "Bad subscript! logarithm must have numeric base");
                }

                this->output.erase(underscoreIdx, this->tokensIdx + 1);
                this->tokensIdx = underscoreIdx;
            }
            // proccess exponent
            else
            {

                int exponentIdx = this->tokensIdx++;

                exponent = this->getSubTokens();
                if (exponent->size() == 0)
                {
                    std::runtime_error("No exponent found!");
                }
                func->setExponent(exponent);
                this->output.erase(exponentIdx, this->tokensIdx + 1);
                this->tokensIdx = exponentIdx;
            }
        }
        subExpr = this->getSubTokens();




    }


    if (subExpr->size() == 0)
    {
        subExpr->push(std::make_shared<Number>("1", 1));
    }
    else
    {
        this->output.erase(functionIdx + 1, this->tokensIdx + 1);
    }
    func->setSubExpr(subExpr);
    this->tokensIdx = functionIdx;
    token = func;




}

std::shared_ptr<TokenQueue> Tokenizer::getSubTokens()
{
    std::shared_ptr<TokenQueue> subExpr = std::make_shared<TokenQueue>();
    int parenCount = 0;

    // Iterate through the tokens starting at the given index
    for (; this->tokensIdx < this->output.size(); this->tokensIdx++)
    {
        std::shared_ptr<Token>& token = this->output[this->tokensIdx];

        // Push current token to the subexpression
        subExpr->push(token);

        // Check for left parenthesis and increment the count
        if (token->getType() == TokenType::LEFTPAREN)
        {
            ++parenCount;
        }
        // Check for right parenthesis and decrement the count
        else if (token->getType() == TokenType::RIGHTPAREN)
        {
            --parenCount;
        }


        // If parentheses close and we reach the end of the subexpression
        if (parenCount == 0)
        {
            break;
            this->tokensIdx++;
        }
    }
    if (parenCount != 0)
    {
        //! TODO: implement error handling for mismatched parens
        subExpr->clear();
    }



    subExpr->removeParens();
    return subExpr;
}


void Tokenizer::handleVariable()
{
    std::shared_ptr<Token>& token = this->output[this->tokensIdx];
}
void Tokenizer::handleSubscript()
{
    std::shared_ptr<Token>& token = this->output[this->tokensIdx];
}



void Tokenizer::fixEulers()
{
    bool sign = this->currentToken()->isNegative();
    std::shared_ptr<Token> token = std::make_shared<Function>("exp");
    token->setNegative(sign);
    if (this->tokensIdx == this->output.size() - 1)
    {
        this->output.emplace_back(std::make_shared<LeftParenthesis>());
        this->output.emplace_back(std::make_shared<Number>("1", 1));
        this->output.emplace_back(std::make_shared<RightParenthesis>());
    }
    else if (!(this->output[this->tokensIdx + 1]->getType() == TokenType::OPERATOR
        && this->output[this->tokensIdx + 1]->getStr() != "^"))
    {
        auto itr = this->tokensIdx + 1;
        this->output.emplace(itr, std::make_shared<RightParenthesis>());
        this->output.emplace(itr, std::make_shared<Number>("1", 1));
        this->output.emplace(itr, std::make_shared<LeftParenthesis>());
    }


    token = this->output[this->tokensIdx];
}


void Tokenizer::nextImplicit()
{

    if (this->tokensIdx + 1 < this->output.size())
    {
        
        TokenType currentType = this->currentToken()->getType();
        TokenType nextType = this->output[this->tokensIdx + 1]->getType();
        if (Lookup::implicitMultiplication.find(
            {currentType, nextType })
                != Lookup::implicitMultiplication.end())
        {
            if (Lookup::implicitMultiplication[{currentType, nextType}])
            {
                this->output.emplace(this->tokensIdx + 1,
                                    std::make_shared<Operator>("*"));
            }
        }
        else
        {
            try
            {
                std::string msg = "Types " +
                    Lookup::getTokenType(this->currentToken()->getType()) +
                    " and " +
                    Lookup::getTokenType(nextType) +
                    " should not be next to each other. Found at " +
                    this->currentToken()->getFullStr() +
                    " and " +
                    this->output[this->tokensIdx + 1]->getFullStr() + "\n";

            }
            catch (const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }

        }
    }


}
//DEBUG START
std::string Tokenizer::listOutput()
{
    std::string out;
    for (int idx = 0; idx < this->output.size(); idx++)
    {
        out += this->output[idx].get()->getStr();
        if (idx != this->output.size() - 1)
        {
            out += ", ";
        }

    }
    return out;
}
//DEBUG END
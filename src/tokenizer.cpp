/**
 * @file tokenizer.cpp
 * @author Charles Kushelevsky (charliekushelevsky@gmail.com)
 * @brief Implementation of the Tokenizer class which handles tokenizing 
 * input strings.
 * @version 0.1
 * @date 2024-08-31
 */

#include "tokenizer.hpp"
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
    
    constructTree();
    this->substr = "";
    if (!this->input.empty()) {
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

std::vector<std::shared_ptr<Token>> Tokenizer::tokenize()
{
    this->parseExpression();
    return this->output;
}
void Tokenizer::constructTree()
{
    this->mwtRoot = std::make_shared<MWTNode>('\0',false);
    for (const auto &entry : symbolTable)
    {
        // Start from the root of the trie
        std::shared_ptr<MWTNode> current = this->mwtRoot;

        // Traverse through the characters in the entry key
        for (const char &letter : entry.first)
        {
            // Check if the child node exists
            std::shared_ptr<MWTNode> child = current->getChild(letter);

            if (!child) // If the child does not exist, create it
            {
                child = std::make_shared<MWTNode>(letter, false);
                current->children.push_back(child);
            }
            
            // Move to the child node
            current = child;
        }

        // Mark the end of a valid function/operator
        current->isWord = true;
    }
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
        this->listOutput();
        if (this->currentChar >= '0' && this->currentChar <= '9')
        {
            this->output.emplace_back(
                    std::make_shared<Number>(this->parseNumber()));
            this->clearSubstr();
        }
        else if (this->currentChar == ' ')
        {
            this->clearSubstr();
            this->getNext();
            
        }
        else if (this->currentChar == '-')
        {
            this->clearSubstr();
            // Check if '-' is followed by a number (handling unary minus)
            bool lastWasOperator = false;
            if (symbolTable.find(std::string(1,this->peekBack())) 
                        != symbolTable.end())
            {
                lastWasOperator = 
                (symbolTable[std::string(1,this->peekBack())].first 
                == OPERATOR) ||
                (symbolTable[std::string(1,this->peekBack())].first 
                == LEFTPAREN);
                
            }
            if (this->peekBack() == '\0' || lastWasOperator )
            {
                // Peek the next character to see if it's a number
                if (this->peek() >= '0' && this->peek() <= '9')
                {
                    this->checkImplicitMultiplication();
                    // It's a negative number
                    this->getNext(); // Move past the '-'
                    std::shared_ptr<Number> newNum = 
                        std::make_shared<Number>(this->parseNumber());
                    newNum->flipSign(); // Flip the sign of the number
                    this->output.emplace_back(newNum);
                }
                else
                {
                    // unary minus before a variable or expression
                    this->substr += '-';
                    this->getNext();
                }
            }
            else
            {
                // Binary minus (operator)
                this->output.emplace_back(std::make_shared<Operator>("-"));
                this->getNext();
            }
            
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
        clearSubstr();
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
        if(this->peek() == '.')
        {
            if (!hasDecimalPoint)
            {
                hasDecimalPoint = true;
                numberStr += this->peek();
                this->getNext();
            }
            else
            {
                //! TODO add error handling
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
        this->checkImplicitMultiplication();
        this->output.emplace_back(std::make_shared<Variable>(
                                                std::string(1, this->substr[idx])));
        
        this->getSubSript(idx);
        
        
    }
    this->substr.clear();
    
}

void Tokenizer::checkImplicitMultiplication()
{
     if (this->output.size() 
            && this->output.back().get()->getType() != OPERATOR
            && this->output.back().get()->getType() != FUNCTION
            && this->output.back().get()->getType() != LEFTPAREN)
        {
            this->output.emplace_back(std::make_shared<Operator>("*"));
        }
}

void Tokenizer::checkSubstr()
{
    if (this->substr.empty())
    {
        return;
    }
    
    std::shared_ptr<MWTNode> current = mwtRoot;
    int lastMatchedIndex = -1;
    std::string matchedString;

    // Traverse the substring to find the longest match in the trie
    for (int idx = 0; idx< this->substr.length(); idx++)
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
            std::string newVar;
            newVar += this->substr[idx] + 
                this->getSubSript(startIndexInInput + idx);
            this->checkImplicitMultiplication();
            this->output.emplace_back(std::make_shared<Variable>(newVar));
            idx += newVar.length() > 0 ? newVar.length() - 1 : 0;
            if (idx > lastMatchedIndex)
            {
                //! TODO: throw error for malformed subscript
            }
        }

        // Process the matched string as a function/operator
        auto match = symbolTable[matchedString]; 
        if (match.first == FUNCTION)
        {
            this->checkImplicitMultiplication();
            this->output.emplace_back(std::make_shared<Function>(matchedString));
        }
        else if (match.first == OPERATOR)
        {
            this->output.emplace_back(std::make_shared<Operator>(matchedString));
        }
        else if (match.first == LEFTPAREN)
        {
            this->checkImplicitMultiplication();
            this->output.emplace_back(std::make_shared<LeftParenthesis>());
        }
        else if (match.first == RIGHTPAREN)
        {
            this->output.emplace_back(std::make_shared<RightParenthesis>());
        }
        // Remove processed part from this->substr
        this->substr.erase(0, lastMatchedIndex + 1);
    }
    
    
}



std::string Tokenizer::getSubSript(int idx)
{
    std::string subscript = "";
    int tmpIdx = this->currentIndex;
    this->currentIndex = idx;
    
    if (this->peek() != '_')
    {
        this->currentIndex = tmpIdx;
        return subscript;
    }
    this->getNext();
    subscript += this->currentChar;
    this->getNext();

    if (this->peek() == '\0')
    {
        this->currentIndex = tmpIdx;
        return "";
    }

    if (this->currentChar != '{')
    {
        subscript += this->peek();
        this->currentIndex = tmpIdx;
        return subscript;
    }
    while (this->currentChar != '\0' && this->currentChar != '}')
    {
        subscript += this->currentChar;
        this->getNext();
    }
    this->currentIndex = tmpIdx;
    return subscript;
    
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
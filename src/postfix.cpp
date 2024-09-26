#include "postfix.hpp"


ShuntingYard::ShuntingYard(TokenVector input)
    : input(input) {}




TokenQueue ShuntingYard::getPostfix()
{

    this->convert();
    return output;
}

void ShuntingYard::convert()
{
    for (int idx = 0; idx < this->input.size(); idx++)
    {
        this->currentToken = this->input[idx];

        if (this->currentType() == TokenType::NUMBER)
        {
            this->output.push(this->currentToken);
        }
        else if (this->currentType() == TokenType::FUNCTION)
        {
            this->operators.push(this->currentToken);
        }
        else if (this->currentType() == TokenType::OPERATOR)
        {
            this->handleOperator();
        }
        else if (this->currentType() == TokenType::LEFTPAREN)
        {
            this->operators.push(this->currentToken);
        }
        else if (this->currentType() == TokenType::RIGHTPAREN)
        {
            while (this->operators.top()->getType() != TokenType::LEFTPAREN)
            {
                if (this->operators.size() == 0)
                {
                    std::runtime_error("Mismatched parentheses");
                    exit(1);
                }
                this->popToOutput();
            }
            if (this->operators.size() == 0)
            {
                std::runtime_error("Mismatched parentheses");
                exit(1);
            }
            this->operators.pop();
            if (this->operators.size() > 0 &&
                this->currentType() == TokenType::FUNCTION)
            {
                this->popToOutput();
            }
        }
    }
    while (this->operators.size() > 0)
    {
        if (this->operators.top()->getType() == TokenType::LEFTPAREN)
        {
            std::runtime_error("Mismatched parentheses");
            exit(1);
        }
        this->popToOutput();
    }
}

void ShuntingYard::handleOperator()
{
    bool topNotLeftParen, greaterPrecedence, equalLeftAssociativity;

    while (this->operators.size() > 0)
    {
        int topPrecedence = this->operators.top()->getPrecedence();
        int currentPrecedence = this->currentToken->getPrecedence();
        Associativity currentAssociativity = this->currentToken->getAssociativity();

        topNotLeftParen = this->operators.top()->getType() != TokenType::LEFTPAREN;

        greaterPrecedence = (topPrecedence > currentPrecedence);

        equalLeftAssociativity = (topPrecedence == currentPrecedence && currentAssociativity == Associativity::LEFT);

        if (!(topNotLeftParen && (greaterPrecedence || equalLeftAssociativity)))
        {
            break;
        }

        this->output.push(this->operators.top());
        this->operators.pop();
    }

    this->operators.push(this->currentToken);
}



void ShuntingYard::popToOutput()
{
    this->output.push(operators.top());
    this->operators.pop();
}

TokenType ShuntingYard::currentType()
{
    return this->currentToken->getType();
}


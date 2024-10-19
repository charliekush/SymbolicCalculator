#include "postfix.hpp"

#include <stdexcept>


ShuntingYard::ShuntingYard(TokenContainer input)
{
    this->input = TokenVector(input.getVector());
}




TokenQueue ShuntingYard::getPostfix()
{
    this->convert(this->input);
    return this->output;
}

void ShuntingYard::convert(TokenVector expression)
{
    for (int idx = 0; idx < this->input.size(); idx++)
    {
        this->currentToken = this->input[idx];

        if (this->currentType() == TokenType::NUMBER)
        {
            output.push(this->currentToken);
        }
        else if (this->currentType() == TokenType::VARIABLE)
        {
            output.push(this->currentToken);
        }
        else if (this->currentType() == TokenType::FUNCTION)
        {
            auto func = std::dynamic_pointer_cast<Function>(this->currentToken);
            ShuntingYard postfixInput(func->getSubExpr()->getVector());
            auto subExpr =
                std::make_shared<TokenQueue>(postfixInput.getPostfix());
            func->setSubExpr(subExpr);
            operators.push(this->currentToken);
        }
        else if (this->currentType() == TokenType::OPERATOR)
        {
            this->handleOperator();
        }
        else if (this->currentType() == TokenType::LEFTPAREN)
        {
            operators.push(this->currentToken);
        }
        else if (this->currentType() == TokenType::RIGHTPAREN)
        {
            while (operators.top()->getType() != TokenType::LEFTPAREN)
            {
                if (operators.size() == 0)
                {
                    throw std::runtime_error("Mismatched parentheses");
                    
                }
                this->popToOutput();
            }
            if (operators.size() == 0)
            {
                throw std::runtime_error("Mismatched parentheses");
                
            }
            operators.pop();
            if (operators.size() > 0 &&
                this->currentType() == TokenType::FUNCTION)
            {
                this->popToOutput();
            }
        }
    }
    while (operators.size() > 0)
    {
        if (operators.top()->getType() == TokenType::LEFTPAREN)
        {
            throw std::runtime_error("Mismatched parentheses");
            
        }
        this->popToOutput();
    }
}

void ShuntingYard::handleOperator()
{
    bool topNotLeftParen, greaterPrecedence, equalLeftAssociativity;

    while (operators.size() > 0)
    {
        int topPrecedence = operators.top()->getPrecedence();
        int currentPrecedence = this->currentToken->getPrecedence();
        Associativity currentAssociativity = this->currentToken->getAssociativity();

        topNotLeftParen = operators.top()->getType() != TokenType::LEFTPAREN;

        greaterPrecedence = (topPrecedence > currentPrecedence);

        equalLeftAssociativity = (topPrecedence == currentPrecedence && currentAssociativity == Associativity::LEFT);

        if (!(topNotLeftParen && (greaterPrecedence || equalLeftAssociativity)))
        {
            break;
        }

        output.push(operators.top());
        operators.pop();
    }

    operators.push(this->currentToken);
}



void ShuntingYard::popToOutput()
{
    output.push(operators.top());
    operators.pop();
}

TokenType ShuntingYard::currentType()
{
    return this->currentToken->getType();
}


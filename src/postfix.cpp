#include "postfix.hpp"


ShuntingYard::ShuntingYard(std::vector<std::shared_ptr<Token>> input)
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
        
        if (this->currentType() == NUMBER)
        {
                this->output.push(this->currentToken);
        }
        else if (this->currentType() == FUNCTION)
        {
            this->operators.push(this->currentToken);
        }
        else if (this->currentType() == OPERATOR)
        {
            this->handleOperator();
        }
        else if (this->currentType() == LEFTPAREN)
        {
            this->operators.push(this->currentToken);
        }
        else if (this->currentType() == RIGHTPAREN)
        {
            while (this->operators.top()->getType() != LEFTPAREN)
            {
                if (this->operators.size() == 0)
                {
                    //! TODO: add error for mismatched parentheses
                }
                this->popToOutput();
            }
            if (this->operators.size() == 0)
            {
                //! TODO: add error for mismatched parentheses
            }
            this->operators.pop();
            if (this->operators.size() > 0 && this->currentType() == FUNCTION)
            {
                this->popToOutput();
            }
        }
    }
    while (this->operators.size() > 0)
    {
        if (this->operators.top()->getType() == LEFTPAREN)
        {
            //! TODO: add error for mismatched parentheses
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

        topNotLeftParen = this->operators.top()->getType() != LEFTPAREN;

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


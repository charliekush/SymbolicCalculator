#include "token_stack.hpp"

void TokenStack::push(std::shared_ptr<Token> token)
{
    this->container.emplace(this->container.begin(), token);
}

std::shared_ptr<Token> TokenStack::pop()
{
    if (this->size() == 0)
    {
        //! TODO: implement error handling
        return nullptr;
    }
    std::shared_ptr<Token> out = this->container.front();
    this->container.erase(this->container.begin());
    return out;
}

std::shared_ptr<Token> TokenStack::top()
{
    if (this->size() == 0)
    {
        return nullptr;
    }
    return this->container.front();
}


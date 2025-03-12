#include "token_queue.hpp"
#include "token_vector.hpp"

TokenQueue::TokenQueue(TokenContainer container)
{
    this->container = container.getVector();
}

TokenQueue::TokenQueue(const TokenVector& tokenVector)
    : TokenContainer(tokenVector.getVector()) {}

TokenQueue::operator TokenVector() const
{
    return TokenVector(container);  // Convert internal container to TokenVector
}

void TokenQueue::push(std::shared_ptr<Token> token)
{
    this->container.emplace_back(token);
}

std::shared_ptr<Token> TokenQueue::pop()
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

std::shared_ptr<Token> TokenQueue::top()
{
    if (this->size() == 0)
    {
        return nullptr;
    }
    return this->container.front();
}




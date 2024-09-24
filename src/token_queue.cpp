#include "token_queue.hpp"

TokenQueue::TokenQueue() 
{
    this->container = std::vector<std::shared_ptr<Token>>();
}


TokenQueue::TokenQueue(std::vector<std::shared_ptr<Token>> input)
{
    this->container = 
        std::vector<std::shared_ptr<Token>>(input.begin(),input.end());
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


std::vector<std::shared_ptr<Token>> TokenQueue::getVector()
{
    return this->container;
}


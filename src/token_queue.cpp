#include "token_queue.hpp"

TokenQueue::TokenQueue() {}

TokenQueue::TokenQueue(int size)
{
    this->queue.reserve(size);
}
TokenQueue::TokenQueue(std::vector<std::shared_ptr<Token>> input)
{
    this->queue = 
        std::vector<std::shared_ptr<Token>>(input.begin(),input.end());
}

void TokenQueue::push(std::shared_ptr<Token> token)
{
    this->queue.emplace_back(token);
}

std::shared_ptr<Token> TokenQueue::front()
{
    return this->queue.front();
}

std::shared_ptr<Token> TokenQueue::back()
{
    return this->queue.back();
}

std::shared_ptr<Token> TokenQueue::pop()
{
    if (this->size() == 0)
    {
        //! TODO: implement error handling
        return nullptr;
    }
    std::shared_ptr<Token> out = this->queue.front();
    this->queue.erase(this->queue.begin());
    return out;
}

int TokenQueue::size()
{
    return this->queue.size();
}

std::string TokenQueue::toString()
{
    std::string out;
    for (int idx = 0; idx < this->queue.size(); idx++)
    {
        out += this->queue[idx].get()->getStr();
        if (idx != this->queue.size() - 1)
        {
            out += ", ";
        }

    }
    return out;
}
std::vector<std::shared_ptr<Token>> TokenQueue::getVector()
{
    return this->queue;
}
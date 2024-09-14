#include "token_queue.hpp"

TokenQueue::TokenQueue() 
{
    this->queue = std::vector<std::shared_ptr<Token>>();
}

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
bool TokenQueue::empty()
{
    return (this->queue.size() == 0);
}

std::string TokenQueue::toString()
{
    std::string out = "";
    for (int idx = 0; idx < this->queue.size(); idx++)
    {
        out += this->queue[idx].get()->getFullStr();
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

void TokenQueue::clear()
{
    queue.clear();
}
std::shared_ptr<Token> TokenQueue::popFront()
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
std::shared_ptr<Token> TokenQueue::popBack()
{
    if (this->size() == 0)
    {
        //! TODO: implement error handling
        return nullptr;
    }
    std::shared_ptr<Token> out = this->queue.back();
    this->queue.pop_back();
    return out;
}

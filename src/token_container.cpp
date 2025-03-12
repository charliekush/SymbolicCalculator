#include "token_container.hpp"

TokenContainer::TokenContainer(std::vector<std::shared_ptr<Token>> input)
{
    this->container = input;
}


TokenContainer::TokenContainer(std::shared_ptr<TokenContainer> container)
{
    this->container = container->getVector();
}
std::shared_ptr<Token> TokenContainer::front()
{
    return this->container.front();
}
std::shared_ptr<Token> TokenContainer::back()
{
    return this->container.back();
}
int TokenContainer::size()
{
    return this->container.size();
}
bool TokenContainer::empty()
{
    return (this->container.size() == 0);
}

std::string TokenContainer::toString()
{
    std::string out = "";
    for (int idx = 0; idx < this->container.size(); idx++)
    {
        out += this->container[idx].get()->getFullStr();
        if (idx != this->container.size() - 1)
        {
            out += ", ";
        }

    }
    return out;
}

std::shared_ptr<Token> TokenContainer::popFront()
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

std::shared_ptr<Token> TokenContainer::popBack()
{
    if (this->size() == 0)
    {
        //! TODO: implement error handling
        return nullptr;
    }
    std::shared_ptr<Token> out = this->container.back();
    this->container.pop_back();
    return out;
}

void TokenContainer::pushFront(std::shared_ptr<Token> token)
{
    this->container.emplace(this->container.begin(), token);
}

void TokenContainer::pushBack(std::shared_ptr<Token> token)
{
    this->container.emplace_back(token);
}
void TokenContainer::clear()
{
    this->container.clear();
}

void TokenContainer::removeParens()
{
    while (this->size() > 0 && 
            this->front()->getType() == TokenType::LEFTPAREN && 
            this->back()->getType() == TokenType::RIGHTPAREN)
    {
        this->popBack();
        this->popFront();
    }
}

std::vector<std::shared_ptr<Token>> TokenContainer::getVector() const
{
    return this->container;
}
#include "token_vector.hpp"

TokenVector::TokenVector(TokenContainer input)
{
    this->container = input.getVector();
}

std::shared_ptr<Token>& TokenVector::operator[](int index)
{
    if (index >= this->size())
    {
        std::string msg = "Vector index out of bounds!\n\tIndex: " +
            std::to_string(index) + "\n\tSize: " + std::to_string(this->size()) 
            + "\n\tVector: [" + this->toString() + "]";
        std::runtime_error(msg.c_str());
        exit(1);




    }
    return this->container[index];
}



void TokenVector::erase(int start, int end)
{
    if (start >= this->container.size() || 
                end > this->container.size() || start > end) 
    {
        std::string msg = "Indicies out of TokenVector bounds!\n\tRange: " + 
        std::to_string(start) + "-" + std::to_string(end) +
        "\n\tSize: " + std::to_string(this->size()) +
        "\n\tVector: [" + this->toString() + "]";
        std::runtime_error(msg.c_str());
        exit(1);
    }
    this->container.erase(this->container.begin() + start, 
                                this->container.begin() + end);
}
void TokenVector::erase(int idx)
{
    try
    {
        auto test = this->container.at(idx);
    }
    catch(const std::exception& e)
    {
    
        std::string msg = "Indicies out of TokenVector bounds!\n\tIndex: " +
        std::to_string(idx) +
        "\n\tSize: " + std::to_string(this->size()) +
        "\n\tVector: [" + this->toString() + "]";
        std::runtime_error(msg.c_str());
        exit(1);
    }
    this->container.erase(this->container.begin() + idx);
}

void TokenVector::emplace(int idx, std::shared_ptr<Token> token)
{
   
    if (idx < 0 || idx > this->size())
    {
        std::string msg = "Index out of TokenVector bounds!\n\tIndex: " +
        std::to_string(idx) +
        "\n\tSize: " + std::to_string(this->size()) +
        "\n\tVector: [" + this->toString() + "]";
        std::runtime_error(msg.c_str());
        exit(1);
    }
    this->container.emplace(this->container.begin() + idx, token);
    
}

void TokenVector::emplace_back(std::shared_ptr<Token> token)
{
    this->container.emplace_back(token);
}

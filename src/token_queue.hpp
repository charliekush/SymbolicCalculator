#ifndef __TOKEN_QUEUE_HPP__
#define __TOKEN_QUEUE_HPP__

#include "token.hpp"

#include <vector>
#include <string>
#include <memory>

class TokenQueue
{
public: 
    TokenQueue();
    TokenQueue(int size);
    TokenQueue(std::vector<std::shared_ptr<Token>> input);
    std::shared_ptr<Token> front();
    std::shared_ptr<Token> back();
    std::shared_ptr<Token> pop();
    void push(std::shared_ptr<Token> token);
    int size();
    std::string toString();
    std::vector<std::shared_ptr<Token>> getVector();

    
private:
    std::vector<std::shared_ptr<Token>> queue;
};
#endif // __TOKEN_QUEUE_HPP__
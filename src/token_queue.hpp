#ifndef __TOKEN_QUEUE_HPP__
#define __TOKEN_QUEUE_HPP__

#include "token_container.hpp"
#include "token.hpp"

#include <vector>
#include <string>
#include <memory>

class TokenQueue : public TokenContainer
{
public: 
    TokenQueue();
    
    TokenQueue(std::vector<std::shared_ptr<Token>> input);
    std::shared_ptr<Token> pop();
    std::shared_ptr<Token> top();
    void push(std::shared_ptr<Token> token);
    
    std::vector<std::shared_ptr<Token>> getVector();
    
};
#endif // __TOKEN_QUEUE_HPP__
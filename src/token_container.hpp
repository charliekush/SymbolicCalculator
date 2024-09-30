#ifndef __TOKEN_CONTAINER_HPP__
#define __TOKEN_CONTAINER_HPP__
#include "token.hpp"

#include <string>
#include <memory>
#include <vector>
class TokenContainer
{
protected: 
    std::vector<std::shared_ptr<Token>> container;
    std::shared_ptr<Token> front();
    std::shared_ptr<Token> back();
    std::shared_ptr<Token> popBack();
    std::shared_ptr<Token> popFront();
    
    void pushFront(std::shared_ptr<Token> token);
    void pushBack(std::shared_ptr<Token> token);
   
public:
    TokenContainer();
    TokenContainer(std::vector<std::shared_ptr<Token>> input);
    
    virtual ~TokenContainer() = default;
    
    
    void clear();
    int size();
    bool empty();
    void removeParens();
    std::vector<std::shared_ptr<Token>> getVector();
    
    std::string toString();
    
    
};
#endif // __TOKEN_CONTAINER_HPP__
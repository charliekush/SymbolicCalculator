#ifndef __TOKEN_VECTOR_HPP__
#define __TOKEN_VECTOR_HPP__

#include "token_container.hpp"
#include "token.hpp"

#include <vector>
#include <string>
#include <memory>

class TokenVector : public TokenContainer
{
public: 
    TokenVector() = default;
    TokenVector(TokenContainer input);
    
    std::shared_ptr<Token>& operator[](int);
    void emplace(int idx, std::shared_ptr<Token> token);
    void emplace_back(std::shared_ptr<Token> token);
    void erase(int idx);
    void erase(int start, int end);
    
};
#endif // __TOKEN_VECTOR_HPP__
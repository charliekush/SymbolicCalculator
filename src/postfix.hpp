#ifndef __POSTFIX_HPP__
#define __POSTFIX_HPP__

#include "token.hpp"
#include "token_queue.hpp"
#include "token_vector.hpp"
#include "token_stack.hpp"

#include <vector>
#include <stack>
#include <queue>
#include <memory>



class ShuntingYard
{
public:
    ShuntingYard(TokenVector input);
    TokenQueue getPostfix();
private:
    TokenVector input;
    TokenStack operators;
    TokenQueue output;
    std::shared_ptr<Token> currentToken;
    
    void convert();
    void handleOperator();
    
    TokenType currentType();
    void popToOutput();
    
};
#endif // __POSTFIX_HPP__
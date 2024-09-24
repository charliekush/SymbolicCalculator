#ifndef __POSTFIX_HPP__
#define __POSTFIX_HPP__

#include "token.hpp"
#include "token_queue.hpp"

#include <vector>
#include <stack>
#include <queue>
#include <memory>



class ShuntingYard
{
public:
    ShuntingYard(std::vector<std::shared_ptr<Token>> input);
    TokenQueue getPostfix();
private:
    std::vector<std::shared_ptr<Token>> input;
    std::stack<std::shared_ptr<Token>> operators;
    TokenQueue output;
    std::shared_ptr<Token> currentToken;
    
    void convert();
    void handleOperator();
    
    TokenType currentType();
    void popToOutput();
    
};
#endif // __POSTFIX_HPP__
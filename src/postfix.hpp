#ifndef __POSTFIX_HPP__
#define __POSTFIX_HPP__

#include "token.hpp"
#include "token_queue.hpp"
#include "token_vector.hpp"
#include "token_stack.hpp"
#include "expression_node.hpp"

#include <vector>
#include <stack>
#include <queue>
#include <memory>



class ShuntingYard
{
public:
    ShuntingYard(TokenContainer input);
    TokenQueue getPostfix();
private:
    TokenVector input;
    TokenQueue output;
    TokenStack operators;
    std::shared_ptr<Token> currentToken;
    void convert(TokenVector expression);
    void handleOperator();


    TokenType currentType();
    void popToOutput();


};
#endif // __POSTFIX_HPP__
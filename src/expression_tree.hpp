#ifndef __EXPRESSION_TREE_HPP__
#define __EXPRESSION_TREE_HPP__

#include "token.hpp"
#include "token_queue.hpp"
#include "expression_node.hpp"

class ExpressionTree
{
public:
    ExpressionTree(TokenQueue queue);
    std::shared_ptr<ExpressionNode> root;
private:
    std::shared_ptr<ExpressionNode> buildTree(TokenQueue queue);
    void simplify();
    
};

#endif // __EXPRESSION_TREE_HPP__
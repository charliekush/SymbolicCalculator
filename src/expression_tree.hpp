#ifndef __EXPRESSION_TREE_HPP__
#define __EXPRESSION_TREE_HPP__

#include "token.hpp"
#include "token_queue.hpp"
#include "expression_node.hpp"

#include <set>


class ExpressionTree
{
public:
    ExpressionTree() = default;
    ExpressionTree(TokenQueue queue);
    ExpressionTree(std::shared_ptr<ExpressionNode> root);
    std::shared_ptr<ExpressionNode> root;
private:
    std::shared_ptr<ExpressionNode> buildTree(TokenQueue queue);

    std::vector<std::shared_ptr<ExpressionNode>>
        getLeaves(std::shared_ptr<ExpressionNode> root);
    void getLeavesHelper(std::shared_ptr<ExpressionNode> node,
                std::vector<std::shared_ptr<ExpressionNode>>& leaves);
    
};

#endif //__EXPRESSION_TREE_HPP__
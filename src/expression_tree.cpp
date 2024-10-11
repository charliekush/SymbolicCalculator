#include "expression_tree.hpp"

#include <stack>
ExpressionTree::ExpressionTree(TokenQueue queue)
{
    this->root = this->buildTree(queue);
}
ExpressionTree::ExpressionTree(std::shared_ptr<ExpressionNode> root)
{
    this->root = root;
}

std::shared_ptr<ExpressionNode> ExpressionTree::buildTree(TokenQueue queue)
{
    // Stack to store nodes during tree construction
    std::stack<std::shared_ptr<ExpressionNode>> nodeStack;

    while (queue.size() > 0)
    {
        std::shared_ptr<Token> currentToken = queue.pop();

        // If the token is a number or variable, 
        // create a new node and push it onto the stack
        if (currentToken->getType() == TokenType::NUMBER
                        || currentToken->getType() == TokenType::VARIABLE)
        {
            std::shared_ptr<ExpressionNode> newNode =
                std::make_shared<ExpressionNode>(currentToken);
            nodeStack.push(newNode);
        }
        // If the token is an operator, pop two operands and make a new subtree
        else if (currentToken->getType() == TokenType::OPERATOR)
        {
            std::shared_ptr<ExpressionNode> newNode =
                std::make_shared<ExpressionNode>(currentToken);

            // Pop the right operand first, then the left operand
            if (!nodeStack.empty())
            {
                newNode->setRight(nodeStack.top());
                nodeStack.pop();
            }
            if (!nodeStack.empty())
            {
                newNode->setLeft(nodeStack.top());
                nodeStack.pop();
            }

            // Push the new subtree back onto the stack
            nodeStack.push(newNode);
        }
        // If the token is a function, pop its arguments and create a subtree
        else if (currentToken->getType() == TokenType::FUNCTION)
        {
            auto func = std::dynamic_pointer_cast<Function>(currentToken);
            ExpressionTree subTree(*func->getSubExpr().get());
            func->setSubExprTree(std::make_shared<ExpressionTree>(subTree));
            std::shared_ptr<ExpressionNode> newNode =
                std::make_shared<ExpressionNode>(currentToken);

            // Push the function subtree back onto the stack
            nodeStack.push(newNode);
        }

    }

    // The final node on the stack will be the root of the expression tree
    if (!nodeStack.empty())
    {
        return nodeStack.top();
    }

    return nullptr; // In case the queue was empty
}


/**
 * @brief gets all the leaves in the expression tree
 *
 * @param root the node to start from
 * @return shared pointers to the leaves of the tree
 */
std::vector<std::shared_ptr<ExpressionNode>>
ExpressionTree::getLeaves(std::shared_ptr<ExpressionNode> root)
{
    std::vector<std::shared_ptr<ExpressionNode>> leaves;
    this->getLeavesHelper(root, leaves);
    return leaves;
}
void ExpressionTree::getLeavesHelper(std::shared_ptr<ExpressionNode> node,
                std::vector<std::shared_ptr<ExpressionNode>>& leaves) {
    if (!node)
    {
        return;
    }

    // If the node is a leaf (no children), add it to the list
    if (!node->getLeft() && !node->getRight())
    {
        leaves.emplace_back(node);
        return;
    }

    // Recursively gather leaves from the left and right subtrees
    this->getLeavesHelper(node->getLeft(), leaves);
    this->getLeavesHelper(node->getRight(), leaves);
}
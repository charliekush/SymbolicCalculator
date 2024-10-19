#include "expression_node.hpp"
#include "token_queue.hpp"
#include <stack>

std::shared_ptr<ExpressionNode> ExpressionNode::buildTree(TokenQueue queue)
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
            auto subTree = buildTree(*func->getSubExpr().get());
            func->setSubExprTree(subTree);
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


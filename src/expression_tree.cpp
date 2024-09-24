#include "expression_tree.hpp"

#include <stack>
ExpressionTree::ExpressionTree(TokenQueue queue)
{
    this->root = this->buildTree(queue);
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
        // If the token is an operator, pop two operands and create a new subtree
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
            std::shared_ptr<ExpressionNode> newNode = 
            std::make_shared<ExpressionNode>(currentToken);
            std::vector<std::shared_ptr<ExpressionNode>> arguments;
            // Pop arguments from the stack
            if (!nodeStack.empty())
            {
                arguments.push_back(nodeStack.top());
                nodeStack.pop();
            }
            
            // Add arguments as children to the function node
            for (auto it = arguments.rbegin(); it != arguments.rend(); ++it)
            {
                newNode->addChild((*it));
            }

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

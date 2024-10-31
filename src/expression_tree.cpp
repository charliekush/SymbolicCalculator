#include "expression_node.hpp"
#include "token_queue.hpp"
#include "lookup.hpp"

#include <stack>
#include <iostream>

std::shared_ptr<ExpressionNode> ExpressionNode::buildTree(TokenQueue queue)
{
    // Stack to store nodes during tree construction
    std::stack<std::shared_ptr<ExpressionNode>> nodeStack;

    
    while (queue.size() > 0)
    {
        std::shared_ptr<Token> currentToken = queue.pop();
        
        // Handle numbers and variables
        if (currentToken->getType() == TokenType::NUMBER
            || currentToken->getType() == TokenType::VARIABLE)
        {
            std::shared_ptr<ExpressionNode> newNode =
                std::make_shared<ExpressionNode>(currentToken);
            nodeStack.push(newNode);

                    }
        // Handle operators
        else if (currentToken->getType() == TokenType::OPERATOR)
        {
            std::shared_ptr<ExpressionNode> newNode =
                std::make_shared<ExpressionNode>(currentToken);

            // Pop the right and left operands
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

            // Push the new operator subtree onto the stack
            nodeStack.push(newNode);
                    }
        // Handle functions
        else if (currentToken->getType() == TokenType::FUNCTION)
        {
            auto func = std::dynamic_pointer_cast<Function>(currentToken);

            // Build a tree from the function's subexpression
            if (func && func->getSubExpr())
            {
                                
                // Recursively build the tree for the subexpression
                auto subTree = buildTree(*(func->getSubExpr()));
                
                // Set the subexpression tree for the function
                func->setSubExprTree(subTree);

                // Create a new node for the function and link it 
                std::shared_ptr<ExpressionNode> newNode =
                    std::make_shared<ExpressionNode>(currentToken);

                // Attach the subexpression tree as the left child
                newNode->setLeft(subTree);  

                // Push the function node back onto the stack
                nodeStack.push(newNode);

                            }
        }

        // Print the current stack for debugging
                std::stack<std::shared_ptr<ExpressionNode>> tempStack = nodeStack;
        while (!tempStack.empty())
        {
                        tempStack.pop();
        }
            }

    // The final node on the stack will be the root of the expression tree
    if (!nodeStack.empty())
    {
                return nodeStack.top();
    }
    
        return nullptr; // In case the queue was empty
}
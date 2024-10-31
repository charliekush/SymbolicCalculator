#include "tree_modifier.hpp"



void TreeModifier::swapNodes(nodePtr node1, nodePtr node2)
{
    auto temp = std::make_shared<ExpressionNode>();
    temp->setToken(node1->getToken());
    temp->setDerivative(node1->getDerivative());
    temp->setLeft(node1->getLeft());
    temp->setRight(node1->getRight());

    node1->setToken(node2->getToken());
    node1->setDerivative(node2->getDerivative());
    node1->setLeft(node2->getLeft());
    node1->setRight(node2->getRight());

    node2->setToken(temp->getToken());
    node2->setDerivative(temp->getDerivative());
    node2->setLeft(temp->getLeft());
    node2->setRight(temp->getRight());
}

std::shared_ptr<ExpressionNode> TreeModifier::expandNegative(nodePtr node)
{
    
    // Flip the sign of the original node's token to make it positive
    node->getToken()->flipSign();

    // Create -1 node
    auto negativeOne = std::make_shared<Number>("1", 1);
    negativeOne->setNegative(true);
    auto negativeOneNode = std::make_shared<ExpressionNode>(negativeOne);

    // New parent '*' pointer
    auto timesToken = std::make_shared<Operator>("*");
    auto timesNode = std::make_shared<ExpressionNode>(timesToken);

    // Copy of current node
    auto copyNode = node->copyTree();

    timesNode->setLeft(negativeOneNode);
    timesNode->setRight(copyNode);

    
    
    return timesNode;
    
}
void TreeModifier::replaceWithLeftChild(nodePtr& node) {
    if (!node || !node->getLeft()) return;

    // Get the left child of the node
    auto left = node->getLeft();

    // Reset the current node's children
    node->removeLeftChild();
    node->removeRightChild();
    


    node->setToken(left->getToken());
    node->setDerivative(left->getDerivative());
    node->setLeft(left->getLeft());
    node->setRight(left->getRight());
}


void TreeModifier::replaceWithRightChild(nodePtr& node) {
    if (!node || !node->getRight()) return;

    // Get the right child of the node
    auto right = node->getRight()->copyTree();


   
    // Reset the current node's children
    node->removeLeftChild();
    node->removeRightChild();
    


    node->setToken(right->getToken());
    node->setDerivative(right->getDerivative());
    node->setLeft(right->getLeft());
    node->setRight(right->getRight());
    
    

    
}

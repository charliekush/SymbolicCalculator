#include "MWT.hpp"
#include "token.hpp"
#include "lookup.hpp"

MWTNode::MWTNode(char inChar, bool word) :
    character(inChar), isWord(word) {
}

std::shared_ptr<MWTNode> MWTNode::getChild(const char& in)
{

    for (const std::shared_ptr<MWTNode>& child : children)
    {
        if (child.get()->character == in)
        {

            return child;
        }
    }
    return nullptr;
}

std::shared_ptr<MWTNode> MWTNode::constructMWT()
{
    std::shared_ptr<MWTNode> root = std::make_shared<MWTNode>('\0', false);
    for (const auto& entry : Lookup::symbolTable)
    {
        // Start from the root of the trie
        std::shared_ptr<MWTNode> current = root;

        // Traverse through the characters in the entry key
        for (const char& letter : entry.first)
        {
            // Check if the child node exists
            std::shared_ptr<MWTNode> child = current->getChild(letter);
            
            // If the child does not exist, create it
            if (!child) 
            {
                child = std::make_shared<MWTNode>(letter, false);
                current->children.push_back(child);
            }

            // Move to the child node
            current = child;
        }

        // Mark the end of a valid function/operator
        current->isWord = true;
    }
    return root;
}
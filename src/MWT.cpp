#include "MWT.hpp"

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


/**
 * @file MWT.hpp
 * @author Charles Kushelevsky (charliekushelevsky@gmail.com)
 * @brief Provides declaration for multiway trie
 * @version 0.1
 * @date 2024-08-31
 */
#ifndef __MWT_HPP__
#define __MWT_HPP__

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

class MWTNode
{
public:
    char character;
    bool isWord;
    std::vector<std::shared_ptr<MWTNode>> children;

    MWTNode(char character, bool is_word);

    std::shared_ptr<MWTNode> getChild(const char& in);
    
};
#endif // __MWT_HPP__
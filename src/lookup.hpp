/**
 * @file lookup.hpp
 * @author Charles Kushelevsky (charliekushelevsky@gmail.com)
 * @brief Header that contains global static lookup tables.
 * @version 0.1
 * @date 2024-09-11
 */

#ifndef __LOOKUP_HPP__
#define __LOOKUP_HPP__
#include "token.hpp"

#include <unordered_map>

struct PairHash {
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2>& pair) const
    {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

/**
 * @brief class that contains lookup tables and methods to access them
 * 
 */
class Lookup
{
public:
    static std::unordered_map<std::pair<TokenType,TokenType>, bool, PairHash> 
                            implicitMultiplication;

    static std::unordered_map<std::string,
                std::pair<TokenType, SymbolProperties>> symbolTable;
    static std::string getTokenType(TokenType type);
    
};



#endif // __LOOKUP_HPP__
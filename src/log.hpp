#ifndef __LOG_HPP__
#define __LOG_HPP__

#include "expression_node.hpp"

#include <memory>
#include <vector>
#include <string>

class Logger
{
private:
    typedef std::shared_ptr<ExpressionNode> nodePtr;
    std::string input;
    std::string output;
    std::string mode;
    std::vector<std::vector<std::string>> steps;
    std::string (*converter) (nodePtr);
    std::string str(std::string in);
    int depth;
    std::string indent();
    void addLine(std::string line, bool endComma=true);
    void addPair(std::string key, std::string value, 
                                            bool endComma=true);
    std::string outStr;
    void addBrace(std::string c, bool endComma=false);
    std::string cleanBraces(const std::string& input);
public:
    Logger(bool useLaTeX);
    void setInput(std::string input);
    void setMode(std::string input);
    void setOutput(nodePtr node);
    void logChainRule(nodePtr function, nodePtr subDerivative);
    void logProductRule(nodePtr node);
    void logQuotientRule(nodePtr node);
    void logPowerRule(nodePtr node);
    void logAddition(nodePtr node);
    void logSubtraction(nodePtr node);
    std::string out();
    
};



#endif // __LOG_HPP__
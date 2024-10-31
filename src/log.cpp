#include "log.hpp"
#include "latex_converter.hpp"
#include "text_converter.hpp"

#include <stack>
Logger::Logger(bool useLaTeX)
{
    if (useLaTeX)
    {
        this->converter = LaTeXConverter::convertToLaTeX;
    }
    else
    {
        this->converter = TextConverter::convertToText;
    }
    depth = 0;
}

std::string Logger::str(std::string in)
{
    return "\"" + in + "\"";
}
void Logger::setInput(std::string input)
{
    this->input = input;
}
void Logger::setMode(std::string mode)
{
    this->mode = mode;
}

void Logger::setOutput(nodePtr node)
{
    this->output = converter(node);
}

void Logger::logChainRule(nodePtr function, nodePtr subDerivative)
{
    
    std::vector<std::string> step;
    step.emplace_back(str("Rule") + ": " + str("chain") );

    step.emplace_back(str("Function") + ": " + str(converter(function)));
    step.emplace_back(str("u'") + ": " + str(converter(subDerivative)));
    step.emplace_back(str("derivative") + ": " + 
                str(converter(function->getDerivative())));
    steps.emplace_back(step);
    
}
void Logger::logProductRule(nodePtr node)
{
    nodePtr left = node->getLeft();
    nodePtr right = node->getRight();
    std::vector<std::string> step;
    step.emplace_back(str("Rule") + ": " + str("product"));
    step.emplace_back(str("Expression") + ": " + str(converter(node)));
    step.emplace_back(str("u") + ": " + str(converter(left)));
    step.emplace_back(str("v") + ": " + str(converter(right)));
    step.emplace_back(str("u'") + ": " + str(converter(left->getDerivative())));
    step.emplace_back(str("v'") + ": " + str(converter(right->getDerivative())));
    step.emplace_back(str("derivative") + ": " 
                + str(converter(node->getDerivative())));
    steps.emplace_back(step);
}
void Logger::logQuotientRule(nodePtr node)
{
    nodePtr left = node->getLeft();
    nodePtr right = node->getRight();
    std::vector<std::string> step;
    step.emplace_back(str("Rule") + ": " + str("quotient"));
    step.emplace_back(str("Expression") + ": " + str(converter(node)));
    step.emplace_back(str("u") + ": " + str(converter(left)));
    step.emplace_back(str("v") + ": " + str(converter(right)));
    step.emplace_back(str("u'") + ": " + str(converter(left->getDerivative())));
    step.emplace_back(str("v'") + ": " + str(converter(right->getDerivative())));
    step.emplace_back(str("derivative") + ": " 
                + str(converter(node->getDerivative())));
    steps.emplace_back(step);
}
void Logger::logPowerRule(nodePtr node)
{
    nodePtr left = node->getLeft();
    nodePtr right = node->getRight();
    std::vector<std::string> step;
    step.emplace_back(str("Rule") + ": " + str("power"));
    step.emplace_back(str("Expression") + ": " + str(converter(node)));
    step.emplace_back(str("base") + ": " + str(converter(left)));
    step.emplace_back(str("exponent") + ": " + str(converter(right)));
    step.emplace_back(str("base derivative'") + ": " 
                + str(converter(left->getDerivative())));
    step.emplace_back(str("exponent derivative'") + ": " 
                + str(converter(right->getDerivative())));
    step.emplace_back(str("derivative") + ": " 
                + str(converter(node->getDerivative())));
    steps.emplace_back(step);
}
void Logger::logAddition(nodePtr node)
{
    nodePtr left = node->getLeft();
    nodePtr right = node->getRight();
    std::vector<std::string> step;
    step.emplace_back(str("Rule") + ": " + str("addition"));
    step.emplace_back(str("Expression") + ": " + str(converter(node)));
    step.emplace_back(str("left derivative'") + ": " 
                + str(converter(left->getDerivative())));
    step.emplace_back(str("right derivative'") + ": " 
                + str(converter(right->getDerivative())));
    step.emplace_back(str("derivative") + ": " 
                + str(converter(node->getDerivative())));
    steps.emplace_back(step);
}
void Logger::logSubtraction(nodePtr node)
{
    nodePtr left = node->getLeft();
    nodePtr right = node->getRight();
    std::vector<std::string> step;
    step.emplace_back(str("Rule") + ": " + str("subtraction"));
    step.emplace_back(str("Expression") + ": " + str(converter(node)));
    step.emplace_back(str("left derivative'") + ": " 
                + str(converter(left->getDerivative())));
    step.emplace_back(str("right derivative'") + ": " 
                + str(converter(right->getDerivative())));
    step.emplace_back(str("derivative") + ": " 
                + str(converter(node->getDerivative())));
    steps.emplace_back(step);
}

std::string Logger::indent()
{
    std::string tabs;
    for (int counter = 0; counter < this->depth; counter++)
    {
        tabs += "    ";
    }
    return tabs;
}
void Logger::addLine(std::string line, bool endComma)
{
    std::string comma = (endComma) ? ",":"";
    this->outStr += this->indent() + str(line) + ":";
    if (endComma)
    {
        this->outStr += ",";
    }
    this->outStr += "\n";
}
void Logger::addPair(std::string key, std::string value, bool endComma)
{
    std::string comma = (endComma) ? ",":"";
    this->outStr += this->indent() + str(key) + ": " + str(value);
    if (endComma)
    {
        this->outStr += ",";
    }
    this->outStr += "\n";
}
void Logger::addBrace(std::string c, bool endComma)
{

    if (c == "}" || c == "]")
    {
        this->depth--;
    }

    this->outStr += this->indent() + c;
    if (endComma)
    {
        this->outStr += ",";
    }
    this->outStr += "\n";

    
    if (c == "{" || c == "[")
    {
        this->depth++;
    }
    
}



std::string Logger::out()
{
    this->outStr = "";
    
    this->addBrace("{");
    
    
    this->addLine("steps",false);
    
    this->addBrace("[");
    
    for (int i = 0; i < this->steps.size(); i++)
    {
        this->addBrace("{");
        
        std::vector<std::string>& step = this->steps[i];
        for (int j = 0; j < step.size(); j++)
        {
            
            this->outStr += this->indent() + step[j];
            if ((j + 1) != step.size())
            {
                this->outStr += ",";
            }
            this->outStr += "\n";

        }
        
        this->addBrace("}",(i + 1) != this->steps.size());
    }
    
    this->addBrace("]",true);
    
    this->addPair("input", this->input);
    this->addPair("output", this->output);
    this->addPair("mode", this->mode,false);
    this->addBrace("}");
    
    return this->outStr;

}

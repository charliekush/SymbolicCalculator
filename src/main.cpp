#include "derivative.hpp"
#include "latex_converter.hpp"
#include "token_queue.hpp"



#include <iostream>
#include <string>
#include <memory>

std::string getDerivative(std::string input)
{
    
    Derivative out(input,"x");
    return LaTeXConverter::convertToLaTeX(out.solve());
}
int main(int argc, char const* argv[])
{
   /*std::cout << argc << "\n";
    if (argc == 1)
    {
        std::cout << "Input needed\n";
        return 1;
    }
    std::string input = argv[1];*/
    
    //"-2/5*ln(exp(x)-2*(2*x+3)/(5*x^2+x+4))+2/5*x"
    std::string out = getDerivative("sin(cos(x))");
    std::cout <<" \nDerivative: " << out << "\n";


    auto testRoot = std::make_shared<ExpressionNode>(std::make_shared<Operator>("*"));
    
    auto func = std::make_shared<Function>("sin");
    auto var = std::make_shared<Variable>("x");
    auto queue = std::make_shared<TokenQueue>();
    queue->push(var);
    func->setSubExpr(queue);
    func->setSubExprTree(std::make_shared<ExpressionNode>(var));
    auto left = std::make_shared<ExpressionNode>(
                                        std::make_shared<Number>("-1",-1));
    auto right = std::make_shared<ExpressionNode>(func);
    
    testRoot->setLeft(left);
    testRoot->setRight(right);
    //std::cout << "\nout: " << LaTeXConverter::convertToLaTeX(testRoot) << "\n";
    return 0;
}

#include "function_defs.hpp"
#include "calculator.hpp"

#include <cmath>

FunctionDefinition::FunctionDefinition(std::shared_ptr<ExpressionNode> node)
    : node(node){}

void FunctionDefinition::update()
{
    this->func = std::dynamic_pointer_cast<Function>(this->node->getToken());
    this->subDerivative = this->node->getDerivative();
}
void FunctionDefinition::update(std::shared_ptr<ExpressionNode> node)
{
    this->node = node;
    this->func = std::dynamic_pointer_cast<Function>(this->node->getToken());
    this->subDerivative = this->node->getDerivative();
    this->subExpr = func->getSubExprTree()->copyTree();
}

std::shared_ptr<ExpressionNode> FunctionDefinition::chain(
                            std::shared_ptr<ExpressionNode> derivative)
{
    auto full = Operation::times(derivative, this->subDerivative);
    return full;
}
std::shared_ptr<ExpressionNode> FunctionDefinition::chain(
                            std::shared_ptr<Function> derivative)
{
    auto derivativePtr = std::make_shared<ExpressionNode>(derivative);
    auto full = Operation::times(derivativePtr, this->subDerivative);
    return full;
}



// d/dx sin(x) = cos(x)
std::shared_ptr<ExpressionNode> Sin::getDerivative()
{
    auto derivative = std::make_shared<Function>("cos");
    derivative->setSubExprTree(this->subExpr);
    return chain(derivative);
}

double Sin::evaluate(double arg)
{
    return std::sin(arg);
}


// d/dx cos(x) = -sin(x)
std::shared_ptr<ExpressionNode> Cos::getDerivative()
{
    auto derivative = std::make_shared<Function>("sin");
    derivative->flipSign();
    derivative->setSubExprTree(this->subExpr);
    
    
    return chain(derivative);
}

double Cos::evaluate(double arg)
{
    return std::cos(arg);
}


// d/dx tan(x) = sec^2(x)
std::shared_ptr<ExpressionNode> Tan::getDerivative()
{
    auto derivative = std::make_shared<Function>("sec");
    derivative->setSubExprTree(this->subExpr);
    auto squared = Operation::power(
            std::make_shared<ExpressionNode>(derivative),
            std::make_shared<ExpressionNode>(std::make_shared<Number>("2",2)));
    return chain(squared);
}

double Tan::evaluate(double arg)
{
    return std::tan(arg);
}

// d/dx sec(x) = sec(x)tan(x)
std::shared_ptr<ExpressionNode> Sec::getDerivative()
{
    auto secFunc = std::make_shared<Function>("sec");
    secFunc->setSubExprTree(func->getSubExprTree());

    auto tanFunc = std::make_shared<Function>("tan");
    tanFunc->setSubExprTree(func->getSubExprTree());

    auto product = Operation::times(
        std::make_shared<ExpressionNode>(secFunc),
        std::make_shared<ExpressionNode>(tanFunc)
    );

    return chain(product);
}

double Sec::evaluate(double arg)
{
    return 1.0 / std::cos(arg);
}

// d/dx exp(x) = exp(x)
std::shared_ptr<ExpressionNode> Exp::getDerivative()
{
    auto derivative = std::make_shared<Function>("exp");
    derivative->setSubExprTree(this->subExpr);
    return chain(derivative);
}

double Exp::evaluate(double arg)
{
    return std::exp(arg);
}

// d/dx ln(x) = 1/x
std::shared_ptr<ExpressionNode> Ln::getDerivative()
{
    auto denominator = func->getSubExprTree();
    auto numerator = this->subDerivative;

    auto derivative = Operation::divide(numerator, denominator);
    return derivative;
}

double Ln::evaluate(double arg)
{
    return std::log(arg);
}

// d/dx log_a(x) = 1/x
std::shared_ptr<ExpressionNode> Log::getDerivative()
{
    
    
    auto numerator = this->subDerivative;
    
    auto derivative = std::make_shared<ExpressionNode>(
                                    std::make_shared<Operator>("/"));
    auto denomenator = std::make_shared<ExpressionNode>(
                                    std::make_shared<Operator>("*"));
    auto baseFunc = std::make_shared<Function>("ln");
    baseFunc->setSubExpr(this->func->getSubExpr());
    denomenator->setLeft(std::make_shared<ExpressionNode>(baseFunc));
    denomenator->setRight(this->func->getSubExprTree());
    return derivative;
}
// d/dx cot(x) = -csc^2(x)
std::shared_ptr<ExpressionNode> Cot::getDerivative()
{
    auto derivative = std::make_shared<Function>("csc");
    derivative->setSubExprTree(this->subExpr);
    
    // Create csc^2(x)
    auto squared = Operation::power(
        std::make_shared<ExpressionNode>(derivative),
        std::make_shared<ExpressionNode>(std::make_shared<Number>("2", 2))
    );
    
    
    squared->getToken()->flipSign();
    
    return chain(squared);
}

double Cot::evaluate(double arg)
{
    return 1.0 / std::tan(arg);
}

double Log::evaluate(double arg)
{    
    auto base = this->func->getSubscript();
    if ( base->isDouble())
    {
    return std::log(arg) / std::log(base->getDouble());
    }
    return (std::log(arg) / std::log(1.0 * base->getInt()));
}

// d/dx csc(x) = -csc(x)cot(x)
std::shared_ptr<ExpressionNode> Csc::getDerivative()
{
    auto cscFunc = std::make_shared<Function>("csc");
    cscFunc->setSubExprTree(func->getSubExprTree());

    auto cotFunc = std::make_shared<Function>("cot");
    cotFunc->setSubExprTree(func->getSubExprTree());

    auto product = Operation::times(
        std::make_shared<ExpressionNode>(cscFunc),
        std::make_shared<ExpressionNode>(cotFunc)
    );

    // Make the result negative
    product->getToken()->flipSign();

    return chain(product);
}

double Csc::evaluate(double arg)
{
    return 1.0 / std::sin(arg);
}


// d/dx sqrt(x) = 1 / (2 * sqrt(x))
std::shared_ptr<ExpressionNode> Sqrt::getDerivative()
{
    auto two = std::make_shared<ExpressionNode>(
        std::make_shared<Number>("2", 2)
    );

    auto sqrtFunc = std::make_shared<Function>("sqrt");
    sqrtFunc->setSubExprTree(func->getSubExprTree());

    auto denominator = Operation::times(
        two,
        std::make_shared<ExpressionNode>(sqrtFunc)
    );

    auto numerator = std::make_shared<ExpressionNode>(
        std::make_shared<Number>("1", 1)
    );

    auto derivative = Operation::divide(numerator, denominator);
    return chain(derivative);
}

double Sqrt::evaluate(double arg)
{
    return std::sqrt(arg);
}

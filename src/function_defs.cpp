#include "function_defs.hpp"
#include "arithmetic.hpp"
#include "operation.hpp"

#include <cmath>
#include <stdexcept>

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



// d/dx cos(x) = -sin(x)
std::shared_ptr<ExpressionNode> Cos::getDerivative()
{
    auto derivative = std::make_shared<Function>("sin");
    derivative->flipSign();
    derivative->setSubExprTree(this->subExpr);
    
    
    return chain(derivative);
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




// d/dx ln(x) = 1/x
std::shared_ptr<ExpressionNode> Ln::getDerivative()
{
    auto denominator = func->getSubExprTree();
    auto numerator = this->subDerivative;

    auto derivative = Operation::divide(numerator, denominator);
    return derivative;
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


double Sin::evaluate(double arg)
{
    return std::sin(arg); // No bounds checking required
}

double Cos::evaluate(double arg)
{
    return std::cos(arg); // No bounds checking required
}

double Tan::evaluate(double arg)
{
    if (std::fmod(arg, M_PI) == M_PI_2) // Check for odd multiples of π/2
    {
        throw std::runtime_error("undefined operation: tan(arg) is undefined at odd multiples of π/2");
    }
    return std::tan(arg);
}

double Sec::evaluate(double arg)
{
    double cosValue = std::cos(arg);
    if (cosValue == 0.0) // Check for cos(arg) = 0
    {
        throw std::runtime_error("undefined operation: sec(arg) is undefined when cos(arg) is zero");
    }
    return 1.0 / cosValue;
}

double Exp::evaluate(double arg)
{
    if (std::isnan(arg))
    {
        throw std::invalid_argument("Input to exp cannot be NaN");
    }

    // Check for overflow threshold
    if (arg > std::log(std::numeric_limits<double>::max()))
    {
        throw std::overflow_error("exp input too large, results in overflow");
    }

    // Check for underflow threshold
    if (arg < std::log(std::numeric_limits<double>::min()))
    {
        return 0.0; // exp(arg) is effectively zero
    }

    return std::exp(arg);
}

double Ln::evaluate(double arg)
{
    if (arg <= 0.0) // Check for non-positive values
    {
        std::string msg = "undefined operation: ln(" + std::to_string(arg) + ")";
        throw std::runtime_error(msg.c_str());
    }
    return std::log(arg);
}

double Cot::evaluate(double arg)
{
    double tanValue = std::tan(arg);
    if (tanValue == 0.0) // Check for tan(arg) = 0
    {
        throw std::runtime_error("undefined operation: cot(arg) is undefined when tan(arg) is zero");
    }
    return 1.0 / tanValue;
}

double Log::evaluate(double arg)
{
    auto base = this->func->getSubscript();
    double baseValue = base->isDouble() ? base->getDouble() : static_cast<double>(base->getInt());
    if (baseValue <= 0.0 || baseValue == 1.0) // Check for invalid base
    {
        throw std::runtime_error("undefined operation: log base must be positive and not equal to 1");
    }
    if (arg <= 0.0) // Check for non-positive arg
    {
        throw std::runtime_error("undefined operation: log(arg) is undefined for non-positive values of arg");
    }
    return std::log(arg) / std::log(baseValue);
}

double Csc::evaluate(double arg)
{
    double sinValue = std::sin(arg);
    if (sinValue == 0.0) // Check for sin(arg) = 0
    {
        throw std::runtime_error("undefined operation: csc(arg) is undefined when sin(arg) is zero");
    }
    return 1.0 / sinValue;
}

double Sqrt::evaluate(double arg)
{
    if (arg < 0.0) // Check for negative values
    {
        throw std::runtime_error("undefined operation: sqrt(arg) is undefined for negative values");
    }
    return std::sqrt(arg);
}
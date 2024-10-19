#ifndef __FUNCTION_DEFS_HPP__
#define __FUNCTION_DEFS_HPP__

#include "token.hpp"
#include "expression_node.hpp"
#include "expression_node.hpp"

#include <memory>


class FunctionDefinition
{
protected:
    std::shared_ptr<ExpressionNode> node;
    std::shared_ptr<ExpressionNode> subDerivative;
    std::shared_ptr<Function> func;
    std::shared_ptr<ExpressionNode> subExpr;
    bool sign;

    void update();
    
public:

    FunctionDefinition() = default;
    FunctionDefinition(std::shared_ptr<ExpressionNode> node);
    virtual ~FunctionDefinition() = default;
    
    // Method to compute the d/dx the function
    virtual std::shared_ptr<ExpressionNode> getDerivative() = 0;

    // Method to numerically evaluate the function
    virtual double evaluate(double arg) = 0;
    std::shared_ptr<ExpressionNode> chain(std::shared_ptr<ExpressionNode> node);
    std::shared_ptr<ExpressionNode> chain(std::shared_ptr<Function> node);
    void update(std::shared_ptr<ExpressionNode> node);
};

class Sin : public FunctionDefinition
{
public:
    
    std::shared_ptr<ExpressionNode> getDerivative() override;

    double evaluate(double arg) override;
};

class Cos : public FunctionDefinition
{
public:
    // d/dx cos(x) = -sin(x)
    std::shared_ptr<ExpressionNode> getDerivative() override;

    // Numerical evaluation of cos(x)
    double evaluate(double arg) override;
};

class Tan : public FunctionDefinition
{
public:
    // d/dx tan(x) = sec^2(x)
    std::shared_ptr<ExpressionNode> getDerivative() override;

    // Numerical evaluation of tan(x)
    double evaluate(double arg) override;
};
class Cot : public FunctionDefinition
{
public:
    std::shared_ptr<ExpressionNode> getDerivative() override;
    double evaluate(double arg) override;
};

class Csc : public FunctionDefinition
{
public:
    std::shared_ptr<ExpressionNode> getDerivative() override;
    double evaluate(double arg) override;
};

class Sec : public FunctionDefinition
{
public:
    std::shared_ptr<ExpressionNode> getDerivative() override;
    double evaluate(double arg) override;
};

class Exp : public FunctionDefinition
{
public:
    std::shared_ptr<ExpressionNode> getDerivative() override;
    double evaluate(double arg) override;
};

class Ln : public FunctionDefinition
{
public:
    std::shared_ptr<ExpressionNode> getDerivative() override;
    double evaluate(double arg) override;
};

class Sqrt : public FunctionDefinition
{
public:
    std::shared_ptr<ExpressionNode> getDerivative() override;
    double evaluate(double arg) override;
};

class Log : public FunctionDefinition
{
public:
    // d/dx log(x) = 1/x
    std::shared_ptr<ExpressionNode> getDerivative() override;
    // Numerical evaluation of log(x)
    double evaluate(double arg) override;
};

#endif // __FUNCTION_DEFS_HPP__
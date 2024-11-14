#include "computer.h"
#include <stdexcept>
#include <stack>
#include <vector>

// converts a string representation of a normal, infix expression into a vector representation of a postfix expression
std::vector<std::string> infixToPostfix(const std::string &expression);
// evaluates a postfix expression
double evaluatePostfix(std::vector<std::string> &postfix);

const std::string Computer::ops = "+-*/";

std::string Computer::evaluate(const std::string &expression)
{
    std::vector<std::string> postfix = infixToPostfix(expression);
    double res = evaluatePostfix(postfix);
    return std::to_string(res);
}

double applyOperator(double a, double b, char op)
{
    switch (op)
    {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        if (b == 0)
        {
            throw std::runtime_error("Division by zero");
        }
        return a / b;
    default:
        throw std::runtime_error("Unknown operator");
    }
}

int precedence(char op)
{
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return 0;
}

bool isOperator(char c)
{
    return Computer::ops.find(c) != std::string::npos;
}

bool isNumber(char c)
{
    return isdigit(c) || c == '.';
}

std::vector<std::string> infixToPostfix(const std::string &expression)
{
    std::stack<char> operators;
    std::vector<std::string> output;
    int i = 0;
    // to detect invalid sequences like "...++..." or "(+..."
    bool lastWasOperator = true;

    while (i < expression.length())
    {
        char curr = expression[i];

        // if the character is (part of) a number(int/double), push the entire number to the output
        if (isNumber(curr))
        {
            // read the entire number into a string
            std::string num;
            while (i < expression.length() && (isNumber(expression[i])))
            {
                num += expression[i++];
            }
            // push the number to the output
            output.push_back(num);
            lastWasOperator = false;
        }
        // if the character is a '(', push it to the stack
        else if (curr == '(')
        {
            operators.push(curr);
            i++;
            // after '(' we expect an operand or another '('
            lastWasOperator = true;
        }
        // if the character is a ')', push operators to output until we see a '('
        else if (curr == ')')
        {
            while (!operators.empty() && operators.top() != '(')
            {
                output.push_back(std::string(1, operators.top()));
                operators.pop();
            }
            if (operators.empty() || operators.top() != '(')
            {
                throw std::runtime_error("Missing parenthesis");
            }
            // discard '('
            operators.pop();
            i++;
            // we expect an operator after ')'
            lastWasOperator = false;
        }
        // if the character is an operator
        else if (isOperator(curr))
        {
            if (lastWasOperator)
            {
                throw std::runtime_error("Invalid operator placement");
            }

            // pop operators with higher or equal precedence to output
            while (!operators.empty() && precedence(operators.top()) >= precedence(curr))
            {
                output.push_back(std::string(1, operators.top()));
                operators.pop();
            }
            // push the current operator to the stack
            operators.push(curr);
            i++;
            lastWasOperator = true;
        }
        else
        {
            // unknown character
            throw std::runtime_error("Invalid character in the expression");
        }
    }

    // pop all the remaining operators from the stack to the output
    while (!operators.empty())
    {
        if (operators.top() == '(')
        {
            throw std::runtime_error("Invalid parenthesis");
        }
        output.push_back(std::string(1, operators.top()));
        operators.pop();
    }

    return output;
}

double evaluatePostfix(std::vector<std::string> &postfix)
{
    std::stack<double> operands;

    for (std::string token : postfix)
    {
        // if the token is a number, push it to the operand stack
        if (isdigit(token[0]) || token[0] == '.')
        {
            operands.push(std::stod(token));
        }
        // if the token is an operator, pop two operands and apply it to them
        else if (isOperator(token[0]))
        {
            if (operands.size() < 2)
            {
                throw std::runtime_error("Invalid postfix expression");
            }
            double b = operands.top();
            operands.pop();
            double a = operands.top();
            operands.pop();
            operands.push(applyOperator(a, b, token[0]));
        }
    }

    // there should be only one value left in the stack - the result
    if (operands.size() != 1)
    {
        throw std::runtime_error("Invalid postfix expression");
    }

    return operands.top();
}

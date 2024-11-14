#ifndef COMPUTER_H
#define COMPUTER_H

#include <string>

class Computer
{
public:
    // a string containing all valid operators
    static const std::string ops;
    static std::string evaluate(const std::string &expression);
};

#endif

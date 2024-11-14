#include "cli.h"
#include <iostream>
#include <sstream>
#include <cctype>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

Cli::Cli(History &history) : history(history) {}

void Cli::run()
{
    std::string input;
    while (true)
    {
        std::cout << "Enter an expression or command: ";
        std::getline(std::cin, input);
        if (processInput(input))
        {
            std::cout << "Quit the calculator." << std::endl;
            break;
        }
    }
}

int Cli::processInput(std::string &input)
{
    // replace consecutive whitespace with a single space
    boost::regex_replace(input, boost::regex("\\s+"), " ");
    // trim whitespace from the ends
    boost::trim(input);

    if (input.empty())
        return 0;

    if (input == "exit")
    {
        std::cout << "Saving history..." << std::endl;
        history.persistSession();
        return 1;
    }

    // history commands
    if (input.rfind("h", 0) == 0)
    {
        handleHistoryCommand(input);
        return 0;
    }

    // expression evaluation
    try
    {
        std::string result = handleExpression(input);
        if (result.empty())
        {
            std::cout << "Invalid characters found in expression input" << std::endl;
        }
        else
        {
            history.addEntryToSession(input + " = " + result);
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}

void Cli::handleHistoryCommand(const std::string &input)
{
    if (input == "h save")
    {
        history.persistSession();
        std::cout << "Session persisted." << std::endl;
    }
    else if (input == "h clear session")
    {
        history.clearSession();
        std::cout << "Session history cleared." << std::endl;
    }
    else if (input == "h clear all")
    {
        history.clearAll();
        std::cout << "All history cleared." << std::endl;
    }
    else if (input == "h up") {
        history.shiftPointer(1);
        std::cout << history.getEntry() << std::endl;
    }
    else if (input == "h down") {
        history.shiftPointer(-1);
        std::cout << history.getEntry() << std::endl;
    }
    else if (input.find("h ") == 0)
    {
        try
        {
            int n = std::stoi(input.substr(2));
            // show last n entries
            auto entries = history.getRecentEntries(n);
            for (auto entry : entries)
            {
                std::cout << entry << std::endl;
            }
        }
        catch (...)
        {
            std::cout << "Invalid command." << std::endl;
        }
    }
    else
    {
        std::cout << "Invalid command." << std::endl;
    }
}

std::string Cli::handleExpression(const std::string &input)
{
    std::string expr = input;
    // remove spaces from input
    boost::erase_all(expr, " ");
    // check that the input contains valid characters only
    for (auto c : expr)
    {
        if (c != '(' && c != ')' && c != '.' && Computer::ops.find(c) == std::string::npos && !std::isdigit(c))
        {
            return "";
        }
    }

    // evaluate and return result
    std::string result = Computer::evaluate(expr);
    std::cout << result << std::endl;
    return result;
}

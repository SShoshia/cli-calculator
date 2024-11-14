#ifndef CLI_H
#define CLI_H

#include <string>
#include "history.h"
#include "computer.h"

class Cli {
private:
    History& history;

public:
    Cli(History& history);
    // run the input loop
    void run();
    // process single line of input by the user. return 1 if user wants to exit, 0 otherwise
    int processInput(std::string& input);
    // handle a command relating to history
    void handleHistoryCommand(const std::string& input);
    // evaluate entered expression
    std::string handleExpression(const std::string& input);
};

#endif

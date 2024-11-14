#include "history.h"
#include "cli.h"
#include <iostream>

int main() {
    History history;
    history.readHistory();

    Cli cli(history);
    cli.run();

    history.persistSession();
    return 0;
}

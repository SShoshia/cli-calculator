#ifndef HISTORY_H
#define HISTORY_H

#include <vector>
#include <string>

class History
{
private:
    // stores history of the current session
    std::vector<std::string> sessionHistory;
    // stores history that has been persisted
    std::vector<std::string> persistedHistory;
    // current index in the operations history
    int pointer;
    // name of the file where the history is persisted
    static const std::string HISTORY_FILENAME;

public:
    History();
    // load persisted history from file
    void readHistory();
    // add to session history
    void addEntryToSession(const std::string &entry);
    // save session history to file
    void persistSession();
    // move up/down in history
    void shiftPointer(int shift);
    // get current entry at pointer
    std::string getEntry() const;
    // get recent n entries
    std::vector<std::string> getRecentEntries(int n) const;
    // clear session history
    void clearSession();
    // clear all history(session + persistent)
    void clearAll();
};

#endif

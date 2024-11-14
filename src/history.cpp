#include "history.h"
#include <fstream>
#include <iostream>

const std::string History::HISTORY_FILENAME = "history.txt";

History::History() : pointer(0) {}

void History::readHistory()
{
    // read all entries from the file into persisted history vector
    std::ifstream file(HISTORY_FILENAME);
    std::string line;
    while (std::getline(file, line))
    {
        persistedHistory.push_back(line);
    }
    file.close();
}

void History::addEntryToSession(const std::string &entry)
{
    sessionHistory.push_back(entry);
}

void History::persistSession()
{
    // append every line of session history to the history file
    std::ofstream file(HISTORY_FILENAME, std::ios::app);
    for (const auto &entry : sessionHistory)
    {
        file << entry << "\n";
    }
    file.close();

    // move all of session history to the persisted history
    persistedHistory.insert(persistedHistory.end(), sessionHistory.begin(), sessionHistory.end());
    sessionHistory.clear();
}

void History::shiftPointer(int shift)
{
    pointer += shift;
    // pointer can be -1 to display just an empty string
    if (pointer < -1)
        pointer = -1;
    // set to max index if going above the size of the entire history
    if (pointer > 0 && pointer >= sessionHistory.size() + persistedHistory.size())
        pointer = sessionHistory.size() + persistedHistory.size() - 1;
}

std::string History::getEntry() const
{
    // return empty string if pointer is negative
    if (pointer < 0)
        return "";
    // indexing starts from the most recent entries, so the pointer and the index into the vectors are going to be inverse to each other
    if (pointer < sessionHistory.size())
        return sessionHistory[sessionHistory.size() - 1 - pointer];
    // if the pointer is bigger than the size of the session history, return from persisted history
    if (pointer < sessionHistory.size() + persistedHistory.size())
        return persistedHistory[persistedHistory.size() - 1 - (pointer - sessionHistory.size())];
    // if the pointer is bigger than the size of the entire history, return the very first entry
    return persistedHistory[0];
}

std::vector<std::string> History::getRecentEntries(int n) const
{
    // if n is zero or negative, return empty vector
    if (n <= 0)
    {
        std::vector<std::string> res;
        return res;
    }
    // if n is bigger than the entire history size, return entire history
    if (n > (sessionHistory.size() + persistedHistory.size()))
    {
        std::vector<std::string> res(persistedHistory.begin(), persistedHistory.end());
        res.insert(res.end(), sessionHistory.begin(), sessionHistory.end());
        return res;
    }
    // return part of session history
    if (n < sessionHistory.size())
    {
        std::vector<std::string> res(sessionHistory.end() - n, sessionHistory.end());
        return res;
    }
    // return part of persisted history + session history
    std::vector<std::string> res(persistedHistory.end() - (n - sessionHistory.size()), persistedHistory.end());
    res.insert(res.end(), sessionHistory.begin(), sessionHistory.end());
    return res;
}

void History::clearSession()
{
    sessionHistory.clear();
}

void History::clearAll()
{
    // clear all the in-memory history
    persistedHistory.clear();
    sessionHistory.clear();
    // clear all history in the file
    std::ofstream file(HISTORY_FILENAME, std::ios::trunc);
    file.close();
}

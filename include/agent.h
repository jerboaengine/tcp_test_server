#ifndef AGENT_H
#define AGENT_H

#include <string>
#include <list>
#include "core.h"

class Agent {
public:
    void handleReceiveData(const int sessionId, const char * const data, const unsigned int size, std::string &resp);
    const char *getFormatedResult() const;
    void removeSession(int sessionId);
private:
    void parseData(const char * const data, const unsigned int size, std::list<std::string> &valList);
    void selectSeq(const int sessionId, const std::string &command, const uint64_t start, const uint64_t step);
    void sequenceDataToString(const sequences &seqData, std::string &str);

    Core core;
};

#endif // AGENT_H
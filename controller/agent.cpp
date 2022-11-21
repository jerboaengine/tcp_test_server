#include "agent.h"
#include <sstream>
#include "log.h"

void Agent::handleReceiveData(const int sessionId, const char * const data, const unsigned int size, std::string &resp) {
    unsigned int printableSize = size;
    //string at end has some invisible characters. cut them by specifying length of data array
    for (unsigned int i = 0; i < size; i++) {
        if (!std::isprint(data[i])) {
            printableSize = i;
            break;
        }
    }

    if (printableSize == 0) {
        // ignore
        return;
    }

    std::list<std::string> valList;
    parseData(data, printableSize, valList);

    auto it = valList.begin();
    const std::string &command = *it;
    if (command == "export") {
        if (valList.size() != 2) {
            throw std::runtime_error("wrong command");
        }
        
        const std::string &param = *++it;
        if (param != "seq") {
            throw std::runtime_error("wrong command");
        }
        sequences respSeq;
        core.exportSeq(sessionId, respSeq);
        sequenceDataToString(respSeq, resp);
    } else {
        if (valList.size() != 3) {
            throw std::runtime_error("wrong command");
        }

        const uint64_t start = std::stoull(*++it);
        const uint64_t step = std::stoull(*++it);
        selectSeq(sessionId, command, start, step);
    }
}

void Agent::selectSeq(const int sessionId, const std::string &command, const uint64_t start, const uint64_t step) {
    if (command == "seq1") {
        core.setSeq1(sessionId, start, step);
    } else if (command == "seq2") {
        core.setSeq2(sessionId, start, step);
    } else if (command == "seq3") {
        core.setSeq3(sessionId, start, step);
    } else {
        throw std::runtime_error("wrong command");
    }
}

void Agent::parseData(const char * const data, const unsigned int size, std::list<std::string> &valList) {
    std::stringstream dataStream;
    dataStream.write(data, size);
    std::string word;
    while (std::getline(dataStream, word, ' ')) {
        valList.push_back(word);
    }

    if (valList.size() < 1) {
        throw std::runtime_error("wrong command");
    }
}

void Agent::sequenceDataToString(const sequences &seqData, std::string &str) {
    for (int i = 0; i < SEQ_ROWS; i++) {
        if (!seqData[i]) {
            return; // not all sequences are filled
        }
    }

    for (int x = 0; x < SEQ_ROWS; x++) {
        for (int y = 0; y < SEQ_LENGTH; y++) {
            if (y != 0) {
                str += "\t";
            }
            str += std::to_string(seqData[y].value()[x]);
        }
        str += "\n";
    }
}

void Agent::removeSession(int sessionId) {
    core.removeSession(sessionId);
}
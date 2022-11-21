#include "core.h"
#include "log.h"

void Core::setSeq1(int sessionId, uint64_t start, uint64_t step) {
    sequence seqArray;
    calcSeq(start, step, seqArray);
    source.addSessionData(sessionId, 0, seqArray);
}

void Core::setSeq2(int sessionId, uint64_t start, uint64_t step) {
    sequence seqArray;
    calcSeq(start, step, seqArray);
    source.addSessionData(sessionId, 1, seqArray);
}

void Core::setSeq3(int sessionId, uint64_t start, uint64_t step) {
    sequence seqArray;
    calcSeq(start, step, seqArray);
    source.addSessionData(sessionId, 2, seqArray);
}

void Core::exportSeq(const int idSession, sequences &seqOut) {
    seqOut = source.getSequences(idSession);
}

void Core::calcSeq(uint64_t start, uint64_t step, sequence &seqOut) {
    if (start == 0 || step == 0) {
        throw std::runtime_error("start == 0 or step == 0");
    }

    uint64_t cumulative = start;
    for (int i = 0; i < SEQ_LENGTH; i++) {
        seqOut[i] = cumulative;
        cumulative += step;
        if (cumulative < seqOut[i]) {
            cumulative = start;
        }
    }
}

void Core::removeSession(int sessionId) {
    source.removeSession(sessionId);
}
#ifndef CORE_H
#define CORE_H

#include <cstdint>
#include "source.h"

class Core {
public:
    void setSeq1(int sessionId, uint64_t start, uint64_t step);
    void setSeq2(int sessionId, uint64_t start, uint64_t step);
    void setSeq3(int sessionId, uint64_t start, uint64_t step);
    void exportSeq(const int idSession, sequences &seqOut);

    void removeSession(int sessionId);
private:
    void calcSeq(uint64_t start, uint64_t step, sequence &seqOut);

    Source source;
};

#endif // CORE_H
#include "source.h"

void Source::addSessionData(const int idSession, const int seqI, sequence array) {
    const std::lock_guard<std::mutex> lock(mutex);
    db[idSession][seqI] = array;
}

void Source::removeSession(const int idSession) {
    const std::lock_guard<std::mutex> lock(mutex);
    db.erase(idSession);
}

sequences &Source::getSequences(const int idSession) {
    const std::lock_guard<std::mutex> lock(mutex);
    return db.at(idSession);
}
#ifndef SOURCE_H
#define SOURCE_H

#include <unordered_map>
#include <optional>
#include <array>
#include <thread>
		
#define SEQ_LENGTH 3
#define SEQ_ROWS 3
typedef std::array<uint64_t, SEQ_LENGTH> sequence;
typedef std::array<std::optional<sequence>, SEQ_ROWS> sequences;

class Source {
public:
    void addSessionData(const int idSession, const int seqI, sequence array);
    void removeSession(const int idSession);
    sequences &getSequences(const int idSession);

private:
    std::unordered_map<int, sequences> db;
    std::mutex mutex;
};

#endif // SOURCE_H
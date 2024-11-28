// Statements.h
#ifndef STATEMENTS_H
#define STATEMENTS_H

#include "Statement.h"
#include <unordered_map>
#include <vector>

class Statements {
public:

    void addStatement(int id, const std::string& comment, const std::string& verdictStr, const std::string& date);
    void addTopics(int id, const std::vector<double>& topics);

    std::string getComment(int id) const;
    std::string getVerdict(int id) const;
    std::vector<double> getTopics(int id) const;
    std::string getDate(int id) const;
    std::int32_t getSize() const;

private:
    Verdict parseVerdict(const std::string& verdictStr) const;
    std::unordered_map<int, Statement> entries;
    std::unordered_map<int, std::vector<double>> topicEntries;
};

#endif // STATEMENTS_H

// Statements.h
#ifndef STATEMENTS_H
#define STATEMENTS_H

#include "Statement.h"
#include <unordered_map>
#include <vector>

class Statements {
public:

    void addStatement(int id, const Statement& statement, const std::vector<double>& topics);

    std::string getComment(int id) const;
    std::string getVerdict(int id) const;
    std::vector<double> getTopics(int id) const;
    std::string getDate(int id) const;
    std::int32_t getSize() const;

private:
    std::unordered_map<int, Statement> entries;
};

#endif // STATEMENTS_H

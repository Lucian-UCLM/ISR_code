// Statements.cpp
#include "Statements.h"

void Statements::addStatement(int id, const Statement& statement, const std::vector<double>& topics) {
    entries.emplace(id, statement);
    (void)topics;
    // Store topics if needed, not shown in skeleton
}

std::string Statements::getComment(int id) const {
    // Placeholder return
    return entries.at(id).getComment();
}

std::string Statements::getVerdict(int id) const {
    // Placeholder return, assumes Verdict can be converted to string
    return std::to_string(static_cast<int>(entries.at(id).getVerdict()));
}

std::vector<double> Statements::getTopics(int id) const {
    // Placeholder empty vector, implement as needed
    (void)id;
    return {};
}

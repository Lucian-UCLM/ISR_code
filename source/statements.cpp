// Statements.cpp
#include "Statements.h"

void Statements::addStatement(int id, const Statement& statement, const std::vector<double>& topics) {
    entries.emplace(id, statement);
    (void)topics;
    // Store topics if needed, not shown in skeleton
}

std::string Statements::getComment(int id) const {
    auto it = entries.find(id);
    if (it != entries.end()) {
        return it->second.getComment();
    } else {
        std::cerr << "Error: Statement with id " << id << " not found." << std::endl;
        return "";
    }
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

std::int32_t Statements::getSize() const {
    return static_cast<std::int32_t>(entries.size());
}

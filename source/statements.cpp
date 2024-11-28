// Statements.cpp
#include "Statements.h"

void Statements::addStatement(int id, const std::string& comment, const std::string& verdictStr, const std::string& date) {
    Verdict verdict = parseVerdict(verdictStr);
    Statement statement(comment, verdict, date);
    entries.emplace(id, statement);
}

void Statements::addTopics(int id, const std::vector<double>& topics) {
    if (entries.find(id) != entries.end()) {
        topicEntries[id] = topics;
    } else {
        std::cerr << "Error: Cannot assign topics to non-existent statement ID " << id << "." << std::endl;
    }
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
    auto it = entries.find(id);
    if (it != entries.end()) {
        switch (it->second.getVerdict()) {
            case Verdict::TRUE: return "true";
            case Verdict::MOSTLY_TRUE: return "mostly-true";
            case Verdict::HALF_TRUE: return "half-true";
            case Verdict::MOSTLY_FALSE: return "mostly-false";
            case Verdict::FALSE: return "false";
            case Verdict::PANTS_FIRE: return "pants-fire";
            default: return "unknown";
        }
    } else {
        std::cerr << "Error: Statement with id " << id << " not found." << std::endl;
        return "unknown";
    }
}

std::int32_t Statements::getSize() const {
    return static_cast<std::int32_t>(entries.size());
}

std::vector<double> Statements::getTopics(int id) const {
    auto it = topicEntries.find(id);
    if (it != topicEntries.end()) {
        return it->second;
    } else {
        std::cerr << "Error: No topics found for statement ID " << id << "." << std::endl;
        return {};
    }
}

Verdict Statements::parseVerdict(const std::string& verdictStr) const {
    if (verdictStr == "true") return Verdict::TRUE;
    if (verdictStr == "mostly-true") return Verdict::MOSTLY_TRUE;
    if (verdictStr == "half-true") return Verdict::HALF_TRUE;
    if (verdictStr == "mostly-false") return Verdict::MOSTLY_FALSE;
    if (verdictStr == "false") return Verdict::FALSE;
    if (verdictStr == "pants-fire") return Verdict::PANTS_FIRE;

    std::cerr << "Warning: Unknown verdict \"" << verdictStr << "\". Defaulting to FALSE." << std::endl;
    return Verdict::FALSE;
}
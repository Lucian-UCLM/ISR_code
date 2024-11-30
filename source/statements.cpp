#include "statements.h"

/**
 * @brief Adds a new statement to the collection.
 * 
 * Creates a `Statement` object from the provided data and adds it to the `entries` map.
 * 
 * @param id Unique identifier for the statement.
 * @param comment The text of the statement.
 * @param verdictStr The verdict string (e.g., "true", "false").
 * @param date The date of the statement in "mm/dd/yyyy" format.
 */
void Statements::addStatement(int id, const std::string& comment, const std::string& verdictStr, const std::string& date) {
    Verdict verdict = parseVerdict(verdictStr);
    Statement statement(comment, verdict, date);
    entries.emplace(id, statement);
}

/**
 * @brief Assigns topic proportions to an existing statement.
 * 
 * Topics are stored in the `topicEntries` map. An error message is logged if the statement ID is invalid.
 * 
 * @param id Identifier of the statement.
 * @param topics A vector of topic proportions.
 */
void Statements::addTopics(int id, const std::vector<double>& topics) {
    if (entries.find(id) != entries.end()) {
        topicEntries[id] = topics;
    } else {
        std::cerr << "Error: Cannot assign topics to non-existent statement ID " << id << "." << std::endl;
    }
}

/**
 * @brief Retrieves the comment of a statement by ID.
 * 
 * @param id Identifier of the statement.
 * @return The comment as a string.
 */
std::string Statements::getComment(int id) const {
    auto it = entries.find(id);
    if (it != entries.end()) {
        return it->second.getComment();
    } else {
        std::cerr << "Error: Statement with id " << id << " not found." << std::endl;
        return "";
    }
}

/**
 * @brief Retrieves the verdict of a statement as a string by ID.
 * 
 * Uses the `getVerdict` method of the `Statement` class and converts the `Verdict` enum to a string.
 * 
 * @param id Identifier of the statement.
 * @return The verdict string (e.g., "true", "false").
 */
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

/**
 * @brief Retrieves the topic proportions of a statement by ID.
 * 
 * @param id Identifier of the statement.
 * @return A vector of topic proportions.
 */
std::vector<double> Statements::getTopics(int id) const {
    auto it = topicEntries.find(id);
    if (it != topicEntries.end()) {
        return it->second;
    } else {
        std::cerr << "Error: No topics found for statement ID " << id << "." << std::endl;
        return {};
    }
}

/**
 * @brief Retrieves the number of statements in the collection.
 * 
 * @return The total number of statements.
 */
std::int32_t Statements::getSize() const {
    return static_cast<std::int32_t>(entries.size());
}

/**
 * @brief Parses a verdict string and converts it to a `Verdict` enum.
 * 
 * @param verdictStr The verdict string to parse.
 * @return The corresponding `Verdict` enum value.
 */
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

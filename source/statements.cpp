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
 * @param originator The originator of the statement.
 * @param source The source of the statement (e.g., news, speech).
 * @param factchecker The name of the factchecker.
 * @param factcheckDate The fact-checking date in "mm/dd/yyyy" format.
 */
void Statements::addStatement(int id, const std::string& comment, const std::string& verdictStr, const std::string& date,
                              const std::string& originator, const std::string& source, const std::string& factchecker, const std::string& factcheckDate) {
    Verdict verdict = parseVerdict(verdictStr);
    Statement statement(comment, verdict, date, originator, source, factchecker, factcheckDate);
    entries.emplace(id, statement);
}

/**
 * @brief Assigns topic proportions to an existing statement.
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
 */
std::string Statements::getComment(int id) const {
    auto it = entries.find(id);
    return it != entries.end() ? it->second.getComment() : "";
}

/**
 * @brief Retrieves the verdict of a statement as a string by ID.
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
    }
    return "unknown";
}

/**
 * @brief Retrieves the topic proportions of a statement by ID.
 */
std::vector<double> Statements::getTopics(int id) const {
    auto it = topicEntries.find(id);
    return it != topicEntries.end() ? it->second : std::vector<double>{};
}

/**
 * @brief Retrieves the originator of a statement by ID.
 */
std::string Statements::getOriginator(int id) const {
    auto it = entries.find(id);
    return it != entries.end() ? it->second.getOriginator() : "";
}

/**
 * @brief Retrieves the source of a statement by ID.
 */
std::string Statements::getSource(int id) const {
    auto it = entries.find(id);
    return it != entries.end() ? it->second.getSource() : "";
}

/**
 * @brief Retrieves the factchecker of a statement by ID.
 */
std::string Statements::getFactchecker(int id) const {
    auto it = entries.find(id);
    return it != entries.end() ? it->second.getFactchecker() : "";
}

/**
 * @brief Retrieves the factcheck date of a statement by ID.
 */
std::string Statements::getFactcheckDate(int id) const {
    auto it = entries.find(id);
    return it != entries.end() ? it->second.getFactcheckDate() : "";
}

/**
 * @brief Retrieves the number of statements in the collection.
 */
std::int32_t Statements::getSize() const {
    return static_cast<std::int32_t>(entries.size());
}

/**
 * @brief Parses a verdict string and converts it to a `Verdict` enum.
 */
Verdict Statements::parseVerdict(const std::string& verdictStr) const {
    if (verdictStr == "true") return Verdict::TRUE;
    if (verdictStr == "mostly-true") return Verdict::MOSTLY_TRUE;
    if (verdictStr == "half-true") return Verdict::HALF_TRUE;
    if (verdictStr == "mostly-false") return Verdict::MOSTLY_FALSE;
    if (verdictStr == "false") return Verdict::FALSE;
    if (verdictStr == "pants-fire") return Verdict::PANTS_FIRE;
    return Verdict::FALSE;
}

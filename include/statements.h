#ifndef STATEMENTS_H
#define STATEMENTS_H

#include "statement.h"
#include <unordered_map>
#include <vector>
#include <string>

/**
 * @class Statements
 * @brief Manages a collection of statements and their associated data, such as topics and metadata.
 */
class Statements {
public:
    /**
     * @brief Adds a new statement to the collection.
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
    void addStatement(int id, const std::string& comment, const std::string& verdictStr, const std::string& date,
                      const std::string& originator, const std::string& source, const std::string& factchecker, const std::string& factcheckDate);

    /**
     * @brief Assigns topic proportions to an existing statement.
     * 
     * @param id Identifier of the statement.
     * @param topics A vector of topic proportions.
     */
    void addTopics(int id, const std::vector<double>& topics);

    /**
     * @brief Retrieves the comment of a statement by ID.
     * 
     * @param id Identifier of the statement.
     * @return The comment as a string.
     */
    std::string getComment(int id) const;

    /**
     * @brief Retrieves the verdict of a statement as a string by ID.
     * 
     * @param id Identifier of the statement.
     * @return The verdict string (e.g., "true", "false").
     */
    std::string getVerdict(int id) const;

    /**
     * @brief Retrieves the topic proportions of a statement by ID.
     * 
     * @param id Identifier of the statement.
     * @return A vector of topic proportions.
     */
    std::vector<double> getTopics(int id) const;

    /**
     * @brief Retrieves the originator of a statement by ID.
     * 
     * @param id Identifier of the statement.
     * @return The originator as a string.
     */
    std::string getOriginator(int id) const;

    /**
     * @brief Retrieves the source of a statement by ID.
     * 
     * @param id Identifier of the statement.
     * @return The source as a string.
     */
    std::string getSource(int id) const;

    /**
     * @brief Retrieves the factchecker of a statement by ID.
     * 
     * @param id Identifier of the statement.
     * @return The factchecker as a string.
     */
    std::string getFactchecker(int id) const;

    /**
     * @brief Retrieves the factcheck date of a statement by ID.
     * 
     * @param id Identifier of the statement.
     * @return The factcheck date as a string.
     */
    std::string getFactcheckDate(int id) const;

    /**
     * @brief Retrieves the number of statements in the collection.
     * 
     * @return The total number of statements.
     */
    std::int32_t getSize() const;

private:
    /**
     * @brief Parses a verdict string and converts it to a `Verdict` enum.
     * 
     * @param verdictStr The verdict string to parse.
     * @return The corresponding `Verdict` enum value.
     */
    Verdict parseVerdict(const std::string& verdictStr) const;

    std::unordered_map<int, Statement> entries; ///< Map of statement IDs to their `Statement` objects.
    std::unordered_map<int, std::vector<double>> topicEntries; ///< Map of statement IDs to topic proportions.
};

#endif // STATEMENTS_H

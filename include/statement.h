#ifndef STATEMENT_H
#define STATEMENT_H

#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <iostream>

/**
 * @enum Verdict
 * @brief Represents the truthfulness of a statement.
 */
enum class Verdict {
    TRUE,           ///< Completely true statement.
    MOSTLY_TRUE,    ///< Mostly true statement.
    HALF_TRUE,      ///< Half true, half false statement.
    MOSTLY_FALSE,   ///< Mostly false statement.
    FALSE,          ///< Completely false statement.
    PANTS_FIRE      ///< Statement is blatantly false.
};

/**
 * @class Statement
 * @brief Represents a statement with a comment, a verdict, and a date.
 */
class Statement {
public:
    /**
     * @brief Constructs a Statement object.
     * @param comment The text of the statement.
     * @param verdict The truthfulness verdict of the statement.
     * @param date The date of the statement in "mm/dd/yyyy" format.
     */
    Statement(std::string comment, Verdict verdict, std::string date);

    /**
     * @brief Retrieves the comment of the statement.
     * @return The comment as a string.
     */
    std::string getComment() const;

    /**
     * @brief Retrieves the verdict of the statement.
     * @return The verdict as an enum value of Verdict.
     */
    Verdict getVerdict() const;

    /**
     * @brief Retrieves the date of the statement.
     * @return The date as a `std::tm` structure.
     */
    std::tm getDate() const;

private:
    std::string comment; ///< The text of the statement.
    Verdict verdict;     ///< The truthfulness verdict of the statement.
    std::tm date;        ///< The date the statement was made.
    int id;              ///< Unique identifier for the statement.
};

#endif // STATEMENT_H

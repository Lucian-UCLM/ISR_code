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
 * @brief Represents a statement with additional metadata including originator, source, and factcheck information.
 */
class Statement {
public:
    /**
     * @brief Constructs a Statement object.
     * 
     * @param comment The text of the statement.
     * @param verdict The truthfulness verdict of the statement.
     * @param date The date of the statement in "mm/dd/yyyy" format.
     * @param originator The originator of the statement.
     * @param source The source of the statement (e.g., news, speech).
     * @param factchecker The name of the factchecker.
     * @param factcheckDate The date the statement was fact-checked in "mm/dd/yyyy" format.
     */
    Statement(std::string comment, Verdict verdict, std::string date, std::string originator, std::string source, std::string factchecker, std::string factcheckDate);

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

    /**
     * @brief Retrieves the originator of the statement.
     * @return The originator as a string.
     */
    std::string getOriginator() const;

    /**
     * @brief Retrieves the source of the statement.
     * @return The source as a string.
     */
    std::string getSource() const;

    /**
     * @brief Retrieves the name of the factchecker.
     * @return The factchecker name as a string.
     */
    std::string getFactchecker() const;

    /**
     * @brief Retrieves the fact-checking date of the statement.
     * @return The fact-checking date as a string.
     */
    std::string getFactcheckDate() const;

private:
    std::string comment;       ///< The text of the statement.
    Verdict verdict;           ///< The truthfulness verdict of the statement.
    std::tm date;              ///< The date the statement was made.
    int id;                    ///< Unique identifier for the statement.
    std::string originator;    ///< The originator of the statement.
    std::string source;        ///< The source of the statement (e.g., news, speech).
    std::string factchecker;   ///< The name of the factchecker.
    std::string factcheckDate; ///< The date the statement was fact-checked.
};

#endif // STATEMENT_H

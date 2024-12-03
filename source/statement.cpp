#include "statement.h"

/**
 * @brief Constructs a Statement with the specified comment, verdict, and additional metadata.
 * 
 * Initializes the comment, verdict, originator, source, factchecker, and factcheck date directly.
 * Converts the date and factcheck date from strings in "mm/dd/yyyy" format to `std::tm` structures.
 * If either date format is invalid, an error message is printed to `std::cerr`.
 * 
 * @param comment The text of the statement.
 * @param verdict The truthfulness verdict of the statement.
 * @param date The date of the statement in "mm/dd/yyyy" format.
 * @param originator The originator of the statement.
 * @param source The source of the statement (e.g., news, speech).
 * @param factchecker The name of the factchecker.
 * @param factcheckDate The date the statement was fact-checked in "mm/dd/yyyy" format.
 */
Statement::Statement(std::string comment, Verdict verdict, std::string date, std::string originator, std::string source, std::string factchecker, std::string factcheckDate)
    : comment(std::move(comment)), verdict(verdict), originator(std::move(originator)), source(std::move(source)), factchecker(std::move(factchecker)), factcheckDate(std::move(factcheckDate)), id(0) {
    // Initialize `std::tm` structures to zero
    this->date = {};
    std::istringstream dateStream(date);
    dateStream >> std::get_time(&this->date, "%m/%d/%Y");

    if (dateStream.fail()) {
        std::cerr << "Error: Date format invalid for statement date. Expected format: mm/dd/yyyy." << std::endl;
    }
}

/**
 * @brief Retrieves the comment of the statement.
 * @return The comment as a string.
 */
std::string Statement::getComment() const {
    return comment;
}

/**
 * @brief Retrieves the verdict of the statement.
 * @return The verdict as an enum value of Verdict.
 */
Verdict Statement::getVerdict() const {
    return verdict;
}

/**
 * @brief Retrieves the date of the statement.
 * @return The date as a `std::tm` structure.
 */
std::tm Statement::getDate() const {
    return date;
}

/**
 * @brief Retrieves the originator of the statement.
 * @return The originator as a string.
 */
std::string Statement::getOriginator() const {
    return originator;
}

/**
 * @brief Retrieves the source of the statement.
 * @return The source as a string.
 */
std::string Statement::getSource() const {
    return source;
}

/**
 * @brief Retrieves the name of the factchecker.
 * @return The factchecker name as a string.
 */
std::string Statement::getFactchecker() const {
    return factchecker;
}

/**
 * @brief Retrieves the fact-checking date of the statement.
 * @return The fact-checking date as a string.
 */
std::string Statement::getFactcheckDate() const {
    return factcheckDate;
}

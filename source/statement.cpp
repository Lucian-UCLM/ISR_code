#include "statement.h"

/**
 * @brief Constructs a Statement with the specified comment, verdict, and date.
 * 
 * Initializes the comment and verdict directly and converts the date from a string 
 * in "mm/dd/yyyy" format to a `std::tm` structure. If the date format is invalid,
 * an error message is printed to `std::cerr`.
 * 
 * @param comment The text of the statement.
 * @param verdict The truthfulness verdict of the statement.
 * @param date The date of the statement in "mm/dd/yyyy" format.
 */
Statement::Statement(std::string comment, Verdict verdict, std::string date)
    : comment(std::move(comment)), verdict(verdict), id(0) {
    // Initialize `std::tm` to zero
    this->date = {};

    // Convert date string (e.g., "6/11/2008") to `std::tm` structure
    std::istringstream dateStream(date);
    dateStream >> std::get_time(&this->date, "%m/%d/%Y");

    // Handle parsing errors
    if (dateStream.fail()) {
        std::cerr << "Error: Date format invalid. Expected format: mm/dd/yyyy." << std::endl;
    }
}

/**
 * @brief Retrieves the comment of the statement.
 * 
 * @return The comment as a string.
 */
std::string Statement::getComment() const {
    return comment;
}

/**
 * @brief Retrieves the verdict of the statement.
 * 
 * @return The verdict as an enum value of Verdict.
 */
Verdict Statement::getVerdict() const {
    return verdict;
}

/**
 * @brief Retrieves the date of the statement.
 * 
 * @return The date as a `std::tm` structure.
 */
std::tm Statement::getDate() const {
    return date;
}

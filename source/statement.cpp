// Statement.cpp
#include "Statement.h"

Statement::Statement(std::string comment, Verdict verdict, std::string date)
    : comment(std::move(comment)), verdict(verdict), id(0) {
    // Initialize `std::tm` to zero
    this->date = {};

    // Convert date string (e.g., "6/11/2008") to `std::tm` structure
    std::istringstream dateStream(date);
    dateStream >> std::get_time(&this->date, "%m/%d/%Y");

    if (dateStream.fail()) {
        std::cerr << "Error: Date format invalid. Expected format: mm/dd/yyyy." << std::endl;
    }
}

std::string Statement::getComment() const {
    return comment;
}

Verdict Statement::getVerdict() const {
    return verdict;
}

std::tm Statement::getDate() const {
    return date;
}

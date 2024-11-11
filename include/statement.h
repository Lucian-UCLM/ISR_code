#ifndef STATEMENT_H
#define STATEMENT_H

#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <iostream>

enum class Verdict {
    // Define the verdicts (e.g., ACCEPTED, REJECTED) as needed
};

class Statement {
public:

    Statement(std::string comment, Verdict verdict, std::string date);
    
    std::string getComment() const;
    Verdict getVerdict() const;
    std::tm getDate() const;

private:
    std::string comment;
    Verdict verdict;
    std::tm date;
    int id;
};

#endif // STATEMENT_H
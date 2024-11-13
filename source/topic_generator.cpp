// Topic_generator.cpp
#include "Topic_generator.h"
#include <nlohmann/json.hpp>
#include <boost/process.hpp>
#include <fstream>
#include <iostream>

namespace bp = boost::process;
using json = nlohmann::json;

Topic_generator::Topic_generator() : malletFile(""), rawData("") {
    // Constructor initialization
}

void Topic_generator::importStoreData(Statements& statements) {
    std::ifstream file("input/politifact_factcheck_data_cleaned.json");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open JSON file." << std::endl;
        return;
    }

    json data;
    file >> data;

    int id = 0;
    for (const auto& item : data["statements"]) {
        std::string verdict = item["verdict"];
        std::string comment = item["statement"];
        std::string date = item["statement_date"];

        // Create a Statement object and add it to Statements
        Statement statement(comment, static_cast<Verdict>(verdict == "true" ? 1 : 0), date);
        statements.addStatement(id++, statement, {});  // Empty topics for now
    }

    std::cout << "Data successfully imported and stored in Statements table." << std::endl;
}

void Topic_generator::dataDateFilter(const std::string& input) {
    (void)input;
    return;
}

void Topic_generator::buildMalletProfile(const std::string& input, const std::string& output) {
    try {
        std::cout << "Building Mallet profile..." << std::endl;

        // Command to execute "mallet import-file"
        std::string command = "mallet import-file --input " + input + 
                              " --output " + output + 
                              " --keep-sequence --remove-stopwords";

        // Run the command using Boost.Process
        bp::system(command, bp::std_out > stdout, bp::std_err > stderr);

        std::cout << "Mallet profile built successfully: " << output << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "Error in buildMalletProfile: " << ex.what() << std::endl;
    }
}

void Topic_generator::generateTopics(const std::string& input, int numTopics, const std::string& outputState, const std::string& output) {
    try {
        std::cout << "Generating topics..." << std::endl;

        // Command to execute "mallet train-topics"
        std::string command = "mallet train-topics --input " + input + 
                              " --num-topics " + std::to_string(numTopics) + 
                              " --output-state " + outputState + 
                              " --output-topic-keys " + output + "_keys.txt" +
                              " --output-doc-topics " + output + "_composition.txt";

        // Run the command using Boost.Process
        bp::system(command, bp::std_out > stdout, bp::std_err > stderr);

        std::cout << "Topics generated successfully." << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "Error in generateTopics: " << ex.what() << std::endl;
    }
}

void Topic_generator::numTopicsSelector(int numOfTopics) {
    (void)numOfTopics;
    return;
}

void Topic_generator::exportSimilarity(const std::string& input, const std::string& fieldsToClean) {
    (void)input;
    (void)fieldsToClean;
    return;
}

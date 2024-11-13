// Topic_generator.cpp
#include "Topic_generator.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

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

        Statement statement(comment, static_cast<Verdict>(verdict == "true" ? 1 : 0), date);
        statements.addStatement(id++, statement, {});
    }

    std::cout << "Data successfully imported and stored in Statements table." << std::endl;
}

void Topic_generator::dataDateFilter(const std::string& input) {
    (void)input;
    return;
}

void Topic_generator::buildMalletProfile(Statements& statements, const std::string& output) {
    try {
        std::cout << "Building Mallet profile from Statements table..." << std::endl;

        const std::string tempInputFile = "./temp/statements_only.txt";
        std::ofstream outFile(tempInputFile);
        if (!outFile.is_open()) {
            std::cerr << "Error: Could not create temporary input file." << std::endl;
            return;
        }

        for (int id = 0; id < statements.getSize(); ++id) {
            outFile << statements.getComment(id) << std::endl;
        }
        outFile.close();
        std::cout << "Statements written to " << tempInputFile << std::endl;

        std::string command = "mallet import-file --input " + tempInputFile + 
                              " --output ./temp/" + output + ".mallet" +
                              " --keep-sequence --remove-stopwords";

        int ret_code = std::system(command.c_str());
        if (ret_code != 0) {
            std::cerr << "Error: Mallet process failed with exit code " << ret_code << std::endl;
        } else {
            std::cout << "Mallet profile built successfully." << std::endl;
        }
    } catch (const std::exception& ex) {
        std::cerr << "Error in buildMalletProfile: " << ex.what() << std::endl;
    }
}

void Topic_generator::generateTopics(const std::string& input, int numTopics, const std::string& output) {
    try {
        std::cout << "Generating topics..." << std::endl;

        std::string command = "mallet train-topics --input ./temp/" + input + ".mallet" + 
                              " --num-topics " + std::to_string(numTopics) +
                              " --output-topic-keys ./temp/" + output + "_keys.txt" +
                              " --output-doc-topics ./temp/" + output + "_composition.txt";

        int ret_code = std::system(command.c_str());
        if (ret_code != 0) {
            std::cerr << "Error: Mallet process failed with exit code " << ret_code << std::endl;
        } else {
            std::cout << "Topics generated successfully." << std::endl;
        }
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

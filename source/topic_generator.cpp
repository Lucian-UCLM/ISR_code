// Topic_generator.cpp
#include "topic_generator.h"
#include "perplexity_utils.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cmath>
#include <vector>
#include <tinyxml2.h>
#include <map>

using json = nlohmann::json;
using namespace tinyxml2;

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

        statements.addStatement(id++, comment, verdict, date);
    }

    std::cout << "Data successfully imported and stored in Statements table." << std::endl;
}

void Topic_generator::dataDateFilter(const std::string& input) {
    (void)input;
    return;
}

void Topic_generator::buildMalletProfile(Statements& statements, const std::string& output) {
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
}

void Topic_generator::generateTopics(const std::string& input, int numTopics, const std::string& output) {
    std::cout << "Generating topics..." << std::endl;

    std::string command = "mallet train-topics --input ./temp/" + input + ".mallet" + 
                " --num-topics " + std::to_string(numTopics) +
                " --word-topic-counts-file ./temp/" + output + "_word_topic.txt" +
                " --output-doc-topics ./temp/" + output + "_composition.txt" +
                " --diagnostics-file ./temp/" + output + "_diagnostics.xml" +
                " --optimize-interval 20 --optimize-burn-in 50 --num-iterations 1000" +
                " --beta 0.05 --alpha 50"+
                " > NUL 2>&1";

    int ret_code = std::system(command.c_str());
    if (ret_code != 0) {
        std::cerr << "Error: Mallet process failed with exit code " << ret_code << std::endl;
    } else {
        std::cout << "Topics generated successfully." << std::endl;
    }
}

void Topic_generator::assignTopics(Statements& statements, int numOfTopics, const std::string& profile) {
    int numDocs = 0;
    double* docTopicProbs = parseDocTopicProb(profile, numOfTopics, numDocs);

    if (docTopicProbs == nullptr) {
        std::cerr << "Error: Failed to parse document-topic probabilities." << std::endl;
        return;
    }

    for (int docID = 0; docID < numDocs; ++docID) {
        std::vector<double> topics(numOfTopics);
        for (int topicID = 0; topicID < numOfTopics; ++topicID) {
            topics[topicID] = docTopicProbs[docID * numOfTopics + topicID];
        }
        statements.addTopics(docID, topics);
    }

    delete[] docTopicProbs;

    std::cout << "Topics successfully assigned to the statements hash table." << std::endl;
}

/**
 * @brief Example function to calculate perplexity for a given profile.
 * @param numTopics Number of topics in the model.
 */
#include <fstream> // Include for file handling

void Topic_generator::perplexityPypelyne(int numTopics) {
    size_t numWordsToConsider = 200;
    int numDocs;

    double* docTopicProbs = parseDocTopicProb("profile1", numTopics, numDocs);
    double* wordTopicProbs = parseDiagnosticsForWordTopicProbs("profile1", numTopics, numWordsToConsider);
    double* docProbs = calculateDocumentProbabilities(docTopicProbs, numDocs, numTopics, wordTopicProbs, numWordsToConsider);
    int totalWordsInCorpus = totalNumberOfWords("profile1");

    double perplexity = calculatePerplexity(docProbs, numDocs, totalWordsInCorpus);
    std::cout << "Perplexity: " << perplexity << std::endl;

    double* means = calculateMeans("profile1", numTopics);

    const char* metricNames[] = {
        "tokens", "document_entropy", "word_length", "coherence",
        "uniform_dist", "corpus_dist", "eff_num_words", "token_doc_diff",
        "rank_1_docs", "allocation_ratio", "allocation_count", "exclusivity"
    };

    // Write data to a CSV file
    std::ofstream csvFile("./temp/means_data.csv");
    if (!csvFile.is_open()) {
        std::cerr << "Error: Unable to open file for writing!" << std::endl;
        return;
    }

    // Write headers
    csvFile << numTopics << "\n";
    csvFile << perplexity <<"\n";

    // Write data
    for (int i = 0; i < 12; ++i) {
        csvFile << means[i] << "\n";
    }

    csvFile.close();
    std::cout << "Data successfully exported to means_data.csv" << std::endl;

    delete[] docTopicProbs;
    delete[] wordTopicProbs;
    delete[] docProbs;
}


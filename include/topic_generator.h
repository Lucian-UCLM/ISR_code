#ifndef TOPIC_GENERATOR_H
#define TOPIC_GENERATOR_H

#include <string>
#include "statements.h"
#include <cstdlib>
#include <map>

/**
 * @class Topic_generator
 * @brief Generates and assigns topics to statements using Mallet and perplexity calculations.
 */
class Topic_generator {
public:
    /**
     * @brief Constructor for Topic_generator.
     * Initializes the generator with empty data paths.
     */
    Topic_generator();

    /**
     * @brief Imports and stores data from a JSON file into a `Statements` object.
     * 
     * @param statements Reference to the `Statements` object where data will be stored.
     */
    void importStoreData(Statements& statements);

    /**
     * @brief Cleans the input data (placeholder function).
     * 
     * @param input The input string to clean.
     */
    void dataCleaner(const std::string& input);

    /**
     * @brief Filters data based on a date range (placeholder function).
     * 
     * @param input The input string to filter.
     */
    void dataDateFilter(const std::string& input);

    /**
     * @brief Builds a Mallet profile file from the statements.
     * 
     * @param statements Reference to the `Statements` object containing statement data.
     * @param output Name of the output file for the Mallet profile.
     */
    void buildMalletProfile(Statements& statements, const std::string& output);

    /**
     * @brief Generates topics using the Mallet tool.
     * 
     * @param input Name of the Mallet input file.
     * @param numTopics Number of topics to generate.
     * @param output Name of the output file prefix for topic data.
     */
    void generateTopics(const std::string& input, int numTopics, const std::string& output);

    /**
     * @brief Assigns topic distributions to statements based on Mallet output.
     * 
     * @param statements Reference to the `Statements` object to update.
     * @param numOfTopics Number of topics in the model.
     * @param profile Name of the profile file containing topic distributions.
     */
    void assignTopics(Statements& statements, int numOfTopics, const std::string& profile);

    /**
     * @brief Calculates perplexity and exports metrics for the topic model.
     * 
     * @param numTopics Number of topics in the model.
     */
    void perplexityPypelyne(int numTopics);

private:
    std::string malletFile; ///< Path to the Mallet file.
    std::string rawData;    ///< Raw data path.
};

#endif // TOPIC_GENERATOR_H

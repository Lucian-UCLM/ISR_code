// Topic_generator.h
#ifndef TOPIC_GENERATOR_H
#define TOPIC_GENERATOR_H

#include <string>
#include "Statements.h"
#include <cstdlib>
#include <map>

class Topic_generator {
public:
    Topic_generator();

    void importStoreData(Statements& statements);
    void dataCleaner(const std::string& input);
    void dataDateFilter(const std::string& input);
    void buildMalletProfile(Statements& statements, const std::string& output);
    void generateTopics(const std::string& input, int numTopics, const std::string& output);
    void assignTopics(Statements& statements, int numOfTopics, const std::string& profile);
    void exportSimilarity(const std::string& input, const std::string& fieldsToClean);

    void perplexityPypelyne(int numTopics);

private:
    std::string malletFile;
    std::string rawData;
};

#endif // TOPIC_GENERATOR_H
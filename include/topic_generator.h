// Topic_generator.h
#ifndef TOPIC_GENERATOR_H
#define TOPIC_GENERATOR_H

#include <string>
#include "Statements.h"

class Topic_generator {
public:
    
    Topic_generator();

    void importStoreData(Statements& statements);
    void callJavaHelloWorld();
    void dataCleaner(const std::string& input);
    void dataDateFilter(const std::string& input);
    void buildMalletProfile(const std::string& input, const std::string& output);
    void generateTopics(const std::string& input, int numTopics, const std::string& outputState, const std::string& output);
    void numTopicsSelector(int numOfTopics);
    void exportSimilarity(const std::string& input, const std::string& fieldsToClean);

private:
    std::string malletFile;
    std::string rawData;
};

#endif // TOPIC_GENERATOR_H

// Topic_generator.h
#ifndef TOPIC_GENERATOR_H
#define TOPIC_GENERATOR_H

#include <string>
#include "Statements.h"
#include <cstdlib>

struct DocumentTopic {
    int documentId;
    std::vector<double> topicProportions;
};

struct TopicWord {
    int topicId;
    std::unordered_map<std::string, double> wordProportions;
};

class Topic_generator {
public:
    Topic_generator();

    void importStoreData(Statements& statements);
    void dataCleaner(const std::string& input);
    void dataDateFilter(const std::string& input);
    void buildMalletProfile(Statements& statements, const std::string& output);
    void generateTopics(const std::string& input, int numTopics, const std::string& output);

    std::vector<DocumentTopic> parseDocumentTopicComposition(const std::string& filePath);
    std::vector<TopicWord> parseTopicWordDistributions(const std::string& filePath);
    double computePerplexity(
        const std::vector<DocumentTopic>& docTopics,
        const std::vector<TopicWord>& topicWords,
        const std::unordered_map<std::string, int>& wordCounts);
    void calculateAndPrintPerplexity();
    void numTopicsSelector(int numOfTopics);
    void exportSimilarity(const std::string& input, const std::string& fieldsToClean);

private:
    std::string malletFile;
    std::string rawData;
};

#endif // TOPIC_GENERATOR_H
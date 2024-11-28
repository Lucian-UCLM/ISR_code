#ifndef PERPLEXITY_UTILS_H
#define PERPLEXITY_UTILS_H

#include <string>
#include <tinyxml2.h>

/**
 * @brief Calculates the total number of words (tokens) in the corpus from the diagnostics file.
 * @param diagnosticsFile Name of the diagnostics file (without the "_diagnostics.xml" suffix).
 * @return Total number of words in the corpus.
 */
int totalNumberOfWords(const std::string& diagnosticsFile);

/**
 * @brief Loads document-topic probabilities from a composition file.
 * @param profile Name of the profile file (without the "_composition.txt" suffix).
 * @param numTopics Number of topics in the model.
 * @param numDocs [Output] Number of documents in the file.
 * @return Array of document-topic probabilities (size: numDocs * numTopics).
 */
double* parseDocTopicProb(const std::string& profile, int numTopics, int& numDocs);

/**
 * @brief Parses word-topic probabilities from the diagnostics file.
 * @param diagnosticsFile Name of the diagnostics file (without the "_diagnostics.xml" suffix).
 * @param numTopics Number of topics in the model.
 * @param numWords Number of words to extract per topic.
 * @return Array of word-topic probabilities (size: numTopics * numWords).
 */
double* parseDiagnosticsForWordTopicProbs(const std::string& diagnosticsFile, int numTopics, size_t numWords);

/**
 * @brief Calculates the probabilities of each document in the corpus.
 * @param docTopicProbs Document-topic probabilities array.
 * @param numDocs Number of documents in the corpus.
 * @param numTopics Number of topics in the model.
 * @param wordTopicProbs Word-topic probabilities array.
 * @param numWords Number of words per topic.
 * @return Array of document probabilities (size: numDocs).
 */
double* calculateDocumentProbabilities(double* docTopicProbs, int numDocs, int numTopics, double* wordTopicProbs, size_t numWords);

/**
 * @brief Calculates the perplexity of the corpus.
 * @param docProbs Document probabilities array.
 * @param numDocs Number of documents in the corpus.
 * @param totalWordsInCorpus Total number of words in the corpus.
 * @return Perplexity of the corpus.
 */
double calculatePerplexity(double* docProbs, int numDocs, int totalWordsInCorpus);

double* calculateMeans(const std::string& diagnosticsFile, int numTopics);

#endif // PERPLEXITY_UTILS_H
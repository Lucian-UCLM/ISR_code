#include "perplexity_utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstring> 
#include <omp.h>
#include <cmath>
#include <vector>

using namespace tinyxml2;

int totalNumberOfWords(const std::string& diagnosticsFile) {
    XMLDocument doc;
    if (doc.LoadFile(("./temp/" + diagnosticsFile + "_diagnostics.xml").c_str()) != XML_SUCCESS) {
        std::cerr << "Error: Failed to load diagnostics file: " << diagnosticsFile << std::endl;
        return 0;
    }

    int totalTokens = 0;
    XMLElement* model = doc.FirstChildElement("model");
    if (!model) {
        std::cerr << "Error: No <model> element found in diagnostics file." << std::endl;
        return 0;
    }

    XMLElement* topic = model->FirstChildElement("topic");
    while (topic) {
        totalTokens += topic->IntAttribute("tokens");
        topic = topic->NextSiblingElement("topic");
    }

    return totalTokens;
}

double* calculateMeans(const std::string& diagnosticsFile, int numTopics) {
    XMLDocument doc;
    if (doc.LoadFile(("./temp/" + diagnosticsFile + "_diagnostics.xml").c_str()) != XML_SUCCESS) {
        std::cerr << "Error: Failed to load diagnostics file: " << diagnosticsFile << std::endl;
        return nullptr;
    }

    // Initialize sums for each metric
    double tokens = 0.0, document_entropy = 0.0, word_length = 0.0, coherence = 0.0;
    double uniform_dist = 0.0, corpus_dist = 0.0, eff_num_words = 0.0, token_doc_diff = 0.0;
    double rank_1_docs = 0.0, allocation_ratio = 0.0, allocation_count = 0.0, exclusivity = 0.0;

    XMLElement* model = doc.FirstChildElement("model");
    if (!model) {
        std::cerr << "Error: No <model> element found in diagnostics file." << std::endl;
        return nullptr;
    }

    XMLElement* topic = model->FirstChildElement("topic");
    int topicCount = 0;

    while (topic) {
        tokens += topic->DoubleAttribute("tokens");
        document_entropy += topic->DoubleAttribute("document_entropy");
        word_length += topic->DoubleAttribute("word-length");
        coherence += topic->DoubleAttribute("coherence");
        uniform_dist += topic->DoubleAttribute("uniform_dist");
        corpus_dist += topic->DoubleAttribute("corpus_dist");
        eff_num_words += topic->DoubleAttribute("eff_num_words");
        token_doc_diff += topic->DoubleAttribute("token-doc-diff");
        rank_1_docs += topic->DoubleAttribute("rank_1_docs");
        allocation_ratio += topic->DoubleAttribute("allocation_ratio");
        allocation_count += topic->DoubleAttribute("allocation_count");
        exclusivity += topic->DoubleAttribute("exclusivity");

        topic = topic->NextSiblingElement("topic");
        topicCount++;
    }

    static double means[12];
    means[0] = tokens / numTopics;
    means[1] = document_entropy / numTopics;
    means[2] = word_length / numTopics;
    means[3] = coherence / numTopics;
    means[4] = uniform_dist / numTopics;
    means[5] = corpus_dist / numTopics;
    means[6] = eff_num_words / numTopics;
    means[7] = token_doc_diff / numTopics;
    means[8] = rank_1_docs / numTopics;
    means[9] = allocation_ratio / numTopics;
    means[10] = allocation_count / numTopics;
    means[11] = exclusivity / numTopics;

    return means;
}


double* parseDocTopicProb(const std::string& profile, int numTopics, int& numDocs) {
    std::ifstream file("./temp/" + profile + "_composition.txt", std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open composition file." << std::endl;
        return nullptr;
    }

    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    if (fileSize == 0) {
        std::cerr << "Error: Composition file is empty." << std::endl;
        return nullptr;
    }

    std::string lastLine;
    for (std::streamoff i = 1; i <= fileSize; ++i) {
        file.seekg(-i, std::ios::end);
        char ch;
        file.get(ch);
        if (ch == '\n' && i != 1) {
            std::getline(file, lastLine);
            break;
        }
    }

    numDocs = 0;
    if (!lastLine.empty()) {
        std::istringstream iss(lastLine);
        int docID;
        iss >> docID;
        numDocs = docID + 1;
    }

    double* docTopicProbs = new double[numDocs * numTopics];
    std::memset(docTopicProbs, 0, sizeof(double) * numDocs * numTopics);

    file.clear();
    file.seekg(0, std::ios::beg);
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int docID;
        std::string label;
        iss >> docID >> label;
        for (int topicID = 0; topicID < numTopics; ++topicID) {
            double prob;
            if (!(iss >> prob)) {
                std::cerr << "Error: Unexpected format in composition file on line: " << line << std::endl;
                delete[] docTopicProbs;
                return nullptr;
            }
            docTopicProbs[docID * numTopics + topicID] = prob;
        }
    }

    return docTopicProbs;
}

double* parseDiagnosticsForWordTopicProbs(const std::string& diagnosticsFile, int numTopics, size_t numWords) {
    XMLDocument doc;
    if (doc.LoadFile(("./temp/" + diagnosticsFile + "_diagnostics.xml").c_str()) != XML_SUCCESS) {
        std::cerr << "Error: Failed to load diagnostics file: " << diagnosticsFile << std::endl;
        return nullptr;
    }

    double* wordTopicProbs = new double[numTopics * numWords]();
    XMLElement* topic = doc.FirstChildElement("model")->FirstChildElement("topic");
    size_t topicIndex = 0;

    while (topic && topicIndex < numTopics) {
        XMLElement* word = topic->FirstChildElement("word");
        size_t wordIndex = 0;
        while (word && wordIndex < numWords) {
            double prob = word->DoubleAttribute("prob");
            wordTopicProbs[topicIndex * numWords + wordIndex] = prob;
            word = word->NextSiblingElement("word");
            ++wordIndex;
        }
        topic = topic->NextSiblingElement("topic");
        ++topicIndex;
    }

    return wordTopicProbs;
}

double* calculateDocumentProbabilities(double* docTopicProbs, int numDocs, int numTopics, double* wordTopicProbs, size_t numWords) {
    double* docProbs = new double[numDocs]();

    #pragma omp parallel for
    for (int docID = 0; docID < numDocs; ++docID) {
        double logDocProb = 0.0;

        for (size_t wordID = 0; wordID < numWords; ++wordID) {
            double wordProb = 0.0;

            double* wordTopicPtr = wordTopicProbs + wordID; // Pointer to P(w|t)
            double* docTopicPtr = docTopicProbs + docID * numTopics; // Pointer to P(t|d)

            for (int topicID = 0; topicID < numTopics; ++topicID) {
                wordProb += (*wordTopicPtr) * (*docTopicPtr);
                wordTopicPtr += numWords; // Move to the next topic
                ++docTopicPtr; // Move to the next topic
            }

            if (wordProb > 0) {
                logDocProb += std::log(wordProb);
            }
        }

        docProbs[docID] = std::exp(logDocProb);
    }

    return docProbs;
}


double calculatePerplexity(double* docProbs, int numDocs, int totalWordsInCorpus) {
    double logSum = 0.0;
    for (int docID = 0; docID < numDocs; ++docID) {
        if (docProbs[docID] > 0) {
            logSum += std::log(docProbs[docID]);
        }
    }
    return std::exp(-logSum / totalWordsInCorpus);
}
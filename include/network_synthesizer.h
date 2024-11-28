#ifndef NETWORK_SYNTHESIZER_H
#define NETWORK_SYNTHESIZER_H

#include <vector>
#include <string>
#include "Edge.h"
#include "Network.h"
#include "UF_DS.h"
#include "Statements.h"

class Network_Synthesizer {
public:
    // Constructor
    explicit Network_Synthesizer(const Statements& statements, int numTopics);

    // Methods for building and exporting the network
    void calculateSimilarity();
    void buildNetwork();
    void findMST();
    void exportMSTToCSV(const std::string& filename) const;
    void exportMSTToGraphML(const std::string& filename) const;

    // Debugging/Utility
    void printSimilarityMatrix() const;
    void printMST() const;

private:
    int numDocuments;                        // Number of documents
    int numTopics;                           // Number of topics
    std::vector<double> upperTriangle;       // Upper triangular similarity matrix
    std::vector<Edge> edges;                 // List of edges
    std::vector<Edge> mst;                   // Minimum Spanning Tree edges
    double mstWeight;                        // Total weight of the MST
    UF_DS uf;                                // Union-Find data structure

    // Private helper methods
    void readDocumentTopics(const Statements& statements);
    void calculateDocumentModulus();
    double calculateCosineSimilarity(int doc1, int doc2) const;

    // Internal storage
    std::vector<std::vector<double>> documents; // Topics per document
    std::vector<double> modulus;                // Modulus for each document
};

#endif // NETWORK_SYNTHESIZER_H

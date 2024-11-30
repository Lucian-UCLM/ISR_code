#ifndef NETWORK_SYNTHESIZER_H
#define NETWORK_SYNTHESIZER_H

#include "edge.h"
#include "network.h"
#include "uf_ds.h"
#include "statements.h"
#include <vector>
#include <string>

/**
 * @brief Synthesizes a network from document-topic data and computes the Minimum Spanning Tree (MST).
 */
class Network_Synthesizer {
public:
    /**
     * @brief Constructor for Network_Synthesizer.
     * @param statements Reference to the Statements object containing document-topic data.
     * @param numTopics Number of topics in the dataset.
     */
    explicit Network_Synthesizer(const Statements& statements, int numTopics);

    /**
     * @brief Calculates the similarity matrix for all document pairs.
     */
    void calculateSimilarity();

    /**
     * @brief Builds the network using the calculated similarity matrix.
     */
    void buildNetwork();

    /**
     * @brief Finds the Minimum Spanning Tree (MST) of the network.
     */
    void findMST();

    /**
     * @brief Exports the MST and node data to CSV files.
     * @param edgeFilename Filename for the MST edge CSV file.
     * @param nodeFilename Filename for the node data CSV file.
     * @param statements Reference to the Statements object for node information.
     */
    void exportMSTWithNodeData(const std::string& edgeFilename, const std::string& nodeFilename, const Statements& statements) const;

    /**
     * @brief Exports the MST with node data to a GraphML file.
     * @param filename Filename for the GraphML file.
     * @param statements Reference to the Statements object for node information.
     */
    void exportMSTToGraphMLWithNodeData(const std::string& filename, const Statements& statements) const;

    /**
     * @brief Prints the similarity matrix (for debugging purposes).
     */
    void printSimilarityMatrix() const;

    /**
     * @brief Prints the edges in the MST and its total weight.
     */
    void printMST() const;

private:
    int numDocuments;                        ///< Number of documents.
    int numTopics;                           ///< Number of topics.
    std::vector<double> upperTriangle;       ///< Upper triangular similarity matrix.
    std::vector<Edge> edges;                 ///< List of edges in the network.
    std::vector<Edge> mst;                   ///< Edges in the Minimum Spanning Tree (MST).
    double mstWeight;                        ///< Total weight of the MST.
    UF_DS uf;                                ///< Union-Find data structure for MST computation.

    /**
     * @brief Reads document-topic data from the Statements object.
     * @param statements Reference to the Statements object.
     */
    void readDocumentTopics(const Statements& statements);

    /**
     * @brief Calculates the modulus (norm) of each document's topic vector.
     */
    void calculateDocumentModulus();

    /**
     * @brief Calculates the cosine similarity between two documents.
     * @param doc1 Index of the first document.
     * @param doc2 Index of the second document.
     * @return Cosine similarity value between the two documents.
     */
    double calculateCosineSimilarity(int doc1, int doc2) const;

    std::vector<std::vector<double>> documents; ///< Matrix of topic proportions for each document.
    std::vector<double> modulus;                ///< Modulus (norm) for each document's topic vector.
};

#endif // NETWORK_SYNTHESIZER_H

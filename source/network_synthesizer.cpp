#include "Network_Synthesizer.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <chrono>

// --- Constructor ---
Network_Synthesizer::Network_Synthesizer(const Statements& statements, int numTopics)
    : numTopics(numTopics), numDocuments(statements.getSize()), uf(statements.getSize()) {
    // Resize data structures
    documents.resize(numDocuments, std::vector<double>(numTopics, 0.0));
    modulus.resize(numDocuments, 0.0);

    // Initialize upperTriangle with the size of the upper triangular matrix
    size_t matrixSize = numDocuments * (numDocuments - 1) / 2;
    upperTriangle.resize(matrixSize, 0.0);

    // Populate the document-topic matrix
    readDocumentTopics(statements);
}


// --- Private Methods ---

// Reads document topics from the Statements class and fills the `documents` matrix
void Network_Synthesizer::readDocumentTopics(const Statements& statements) {
    for (int i = 0; i < numDocuments; ++i) {
        documents[i] = statements.getTopics(i);
    }
}

// Calculates the modulus (norm) of each document's topic vector
void Network_Synthesizer::calculateDocumentModulus() {
    std::cout << "Calculating document moduli...\n";
    for (int i = 0; i < numDocuments; ++i) {
        double sum = 0.0;
        for (int j = 0; j < numTopics; ++j) {
            sum += documents[i][j] * documents[i][j];
        }
        modulus[i] = std::sqrt(sum);
    }
    std::cout << "Document moduli calculated successfully.\n";
}

// Calculates the cosine similarity between two documents
double Network_Synthesizer::calculateCosineSimilarity(int doc1, int doc2) const {
    double dotProduct = 0.0;
    for (int k = 0; k < numTopics; ++k) {
        dotProduct += documents[doc1][k] * documents[doc2][k];
    }
    return (modulus[doc1] > 0 && modulus[doc2] > 0) ? dotProduct / (modulus[doc1] * modulus[doc2]) : 0.0;
}

// --- Public Methods ---

// Calculates the similarity matrix for all document pairs
void Network_Synthesizer::calculateSimilarity() {
    std::cout << "Calculating similarity matrix...\n";
    calculateDocumentModulus();

    // Resize upperTriangle to fit the required size
    size_t matrixSize = numDocuments * (numDocuments - 1) / 2; // Upper triangular size
    upperTriangle.resize(matrixSize, 0.0);

    // Lambda function for indexing the upper triangular matrix
    auto index = [this](int i, int j) {
        return (i * (2 * numDocuments - i - 1)) / 2 + (j - i - 1);
    };

    // Populate the upper triangular matrix
    for (int i = 0; i < numDocuments; ++i) {
        for (int j = i + 1; j < numDocuments; ++j) {
            double similarity = calculateCosineSimilarity(i, j);
            upperTriangle[index(i, j)] = similarity;
        }
    }

    std::cout << "Upper triangular similarity matrix calculated successfully.\n";
}


void Network_Synthesizer::buildNetwork() {
    std::cout << "Building network...\n";
    Network network; // Create a Network object

    auto start = std::chrono::high_resolution_clock::now();
    network.buildFromSimilarityMatrix(upperTriangle, numDocuments);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Time taken: " << std::chrono::duration<double>(end - start).count() << " seconds\n";

    edges = network.getEdges(); // Store the edges in the synthesizer for further processing
    std::cout << "Network successfully built with " << edges.size() << " edges.\n";
}

void Network_Synthesizer::findMST() {
    std::cout << "Finding minimum spanning tree...\n";
    mst.clear(); // Clear any existing MST
    mstWeight = 0.0; // Reset MST weight

    // Process edges sorted by weight
    for (const auto& edge : edges) {
        int u = edge.getNode1();
        int v = edge.getNode2();

        // If the two nodes are not connected, add the edge to the MST
        if (uf.find(u) != uf.find(v)) {
            mst.push_back(edge);
            uf.unite(u, v);
            mstWeight += edge.getWeight();
        }
    }

    std::cout << "MST successfully calculated. Total weight: " << mstWeight << "\n";
}


// --- Debugging/Utility Methods ---

// Prints the similarity matrix
void Network_Synthesizer::printSimilarityMatrix() const {
    auto index = [this](int i, int j) {
        return (i * (2 * numDocuments - i - 1)) / 2 + (j - i - 1);
    };

    std::cout << "Similarity Matrix (upper triangular):\n";
    for (int i = 0; i < numDocuments; ++i) {
        for (int j = i + 1; j < numDocuments; ++j) {
            std::cout << "Similarity[" << i << "][" << j << "] = " 
                      << upperTriangle[index(i, j)] << "\n";
        }
    }
}


void Network_Synthesizer::printMST() const {
    std::cout << "Edges in MST:\n";
    for (const auto& edge : mst) {
        std::cout << edge.getNode1() << " <--> " << edge.getNode2() 
                  << " (Weight: " << edge.getWeight() << ")\n";
    }
    std::cout << "Total MST weight: " << mstWeight << "\n";
}

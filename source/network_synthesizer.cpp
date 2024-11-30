#include "network_synthesizer.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <set>

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
    double cosineSimilarity = (modulus[doc1] > 0 && modulus[doc2] > 0) ? dotProduct / (modulus[doc1] * modulus[doc2]) : 0.0;
    return 1.0 - cosineSimilarity; // Invert the similarity to represent stronger relations with lower values
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

void Network_Synthesizer::exportMSTWithNodeData(const std::string& edgeFilename, const std::string& nodeFilename, const Statements& statements) const {
    // Export MST edges
    std::ofstream edgeFile(edgeFilename);
    if (!edgeFile.is_open()) {
        std::cerr << "Error: Could not open file " << edgeFilename << " for writing." << std::endl;
        return;
    }

    edgeFile << "source,target,weight\n"; // Header for edges CSV
    for (const auto& edge : mst) {
        edgeFile << edge.getNode1() << "," << edge.getNode2() << "," << edge.getWeight() << "\n";
    }
    edgeFile.close();
    std::cout << "MST edges exported to " << edgeFilename << std::endl;

    // Export node data
    std::ofstream nodeFile(nodeFilename);
    if (!nodeFile.is_open()) {
        std::cerr << "Error: Could not open file " << nodeFilename << " for writing." << std::endl;
        return;
    }

    nodeFile << "node_id,verdict\n"; // Header for nodes CSV
    for (int i = 0; i < numDocuments; ++i) {
        std::string verdict = statements.getVerdict(i); // Retrieve the verdict for this node
        nodeFile << i << "," << verdict << "\n";
    }
    nodeFile.close();
    std::cout << "Node data exported to " << nodeFilename << std::endl;
}

void Network_Synthesizer::exportMSTToGraphMLWithNodeData(const std::string& filename, const Statements& statements) const {
    // Open the GraphML file for writing
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
        return;
    }

    // Write the GraphML header
    outputFile << R"(<?xml version="1.0" encoding="UTF-8"?>)" << '\n';
    outputFile << R"(<graphml xmlns="http://graphml.graphdrawing.org/xmlns")" << '\n';
    outputFile << R"(    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance")" << '\n';
    outputFile << R"(    xsi:schemaLocation="http://graphml.graphdrawing.org/xmlns )" << '\n';
    outputFile << R"(     http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd">)" << '\n';
    outputFile << R"(<graph id="G" edgedefault="undirected">)" << '\n';

    // Add node attributes for verdict
    outputFile << R"(<key id="verdict" for="node" attr.name="verdict" attr.type="string"/>)" << '\n';
    outputFile << R"(<key id="weight" for="edge" attr.name="weight" attr.type="double"/>)" << '\n';
    outputFile << R"(<key id="label" for="edge" attr.name="label" attr.type="string"/>)" << '\n';

    // Write nodes
    std::set<int> nodes;
    for (const auto& edge : mst) {
        nodes.insert(edge.getNode1());
        nodes.insert(edge.getNode2());
    }

    for (const int node : nodes) {
        std::string verdict = statements.getVerdict(node); // Retrieve the verdict for this node
        outputFile << "  <node id=\"" << node << "\">" << '\n';
        outputFile << "    <data key=\"verdict\">" << verdict << "</data>" << '\n';
        outputFile << "  </node>" << '\n';
    }

    // Write edges
    int edgeId = 0;

    // Write edges with corrected weight format
    for (const auto& edge : mst) {
        double scaledWeight = (1.0 - edge.getWeight()) * 100; // Scale weight for better visualization
        outputFile << "  <edge id=\"e" << edgeId++ 
                << "\" source=\"" << edge.getNode1()
                << "\" target=\"" << edge.getNode2() << "\">" << '\n';
        outputFile << "    <data key=\"weight\">" << scaledWeight << "</data>" << '\n';
        outputFile << "    <data key=\"label\"> " << scaledWeight << "</data>" << '\n';
        outputFile << "  </edge>" << '\n';
    }



    // Write the GraphML footer
    outputFile << "</graph>" << '\n';
    outputFile << "</graphml>" << '\n';

    outputFile.close();
    std::cout << "MST exported to GraphML successfully with node data to " << filename << std::endl;
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

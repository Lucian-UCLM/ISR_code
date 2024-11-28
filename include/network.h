#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <string>
#include "Edge.h"

class Network {
public:
    // Constructor
    explicit Network() = default;

    // Add an edge to the network
    void addEdge(int source, int target, double weight, int id);

    // Build the network from a similarity matrix
    void buildFromSimilarityMatrix(const std::vector<double>& upperTriangle, int numNodes);

    // Get all edges in the network
    const std::vector<Edge>& getEdges() const;

    // Print the network details
    void printNetwork() const;

private:
    std::vector<Edge> edges;  // List of edges in the network
    int edgeId = 0;           // Counter for assigning edge IDs
};

#endif // NETWORK_H

#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <string>
#include "edge.h"

/**
 * @brief Represents a network of nodes connected by edges.
 */
class Network {
public:
    /**
     * @brief Default constructor for the Network class.
     */
    explicit Network() = default;

    /**
     * @brief Adds an edge to the network.
     * @param source Source node of the edge.
     * @param target Target node of the edge.
     * @param weight Weight of the edge.
     * @param id Unique identifier for the edge.
     */
    void addEdge(int source, int target, double weight, int id);

    /**
     * @brief Builds the network from a similarity matrix.
     * @param upperTriangle 1D array representing the upper triangular part of the similarity matrix.
     * @param numNodes Number of nodes in the network.
     */
    void buildFromSimilarityMatrix(const std::vector<double>& upperTriangle, int numNodes);

    /**
     * @brief Retrieves all edges in the network.
     * @return A reference to the vector of edges.
     */
    const std::vector<Edge>& getEdges() const;

    /**
     * @brief Prints the details of all edges in the network to the console.
     */
    void printNetwork() const;

private:
    std::vector<Edge> edges;  ///< List of edges in the network.
    int edgeId = 0;           ///< Counter for assigning unique edge IDs.
};

#endif // NETWORK_H

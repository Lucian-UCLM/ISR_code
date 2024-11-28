#ifndef EDGE_H
#define EDGE_H

/**
 * @class Edge
 * @brief Represents an edge in a network with two nodes, a weight and an identifier.
 */
class Edge {
private:
    int node1; ///< Identifier of the first node.
    int node2; ///< Identifier of the second node.
    double weight; ///< Weight of the edge.
    int id; ///< Unique identifier of the edge.

public:
    /**
     * @brief Default constructor for Edge.
     * 
     * Initializes an Edge object with default values.
     */
    Edge() = default;

    /**
     * @brief Parameterized constructor for Edge.
     * 
     * Initializes an Edge object with specified values for node1, node2, weight and id.
     * 
     * @param node1 Identifier of the first node.
     * @param node2 Identifier of the second node.
     * @param weight Weight of the edge.
     * @param id Unique identifier of the edge.
     */
    Edge(int node1, int node2, double weight, int id);

    /**
     * @brief Gets the identifier of the first node.
     * 
     * @return int Identifier of the first node.
     */
    int getNode1() const;

    /**
     * @brief Gets the identifier of the second node.
     * 
     * @return int Identifier of the second node.
     */
    int getNode2() const;

    /**
     * @brief Gets the weight of the edge.
     * 
     * @return double Weight of the edge.
     */
    double getWeight() const;

    /**
     * @brief Gets the unique identifier of the edge.
     * 
     * @return int Unique identifier of the edge.
     */
    int getId() const;
};

#endif

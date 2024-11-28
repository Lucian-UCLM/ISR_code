#include "Edge.h"

/**
 * @brief Constructs an Edge with specified node identifiers, weight, and id.
 * 
 * @param node1 Identifier of the first node.
 * @param node2 Identifier of the second node.
 * @param weight Weight of the edge.
 * @param id Unique identifier of the edge.
 */
Edge::Edge(int node1, int node2, double weight, int id) {
    this->node1 = node1;
    this->node2 = node2;
    this->weight = weight;
    this->id = id;
}

/**
 * @brief Gets the identifier of the first node.
 * 
 * @return int Identifier of the first node.
 */
int Edge::getNode1() const {
    return node1;
}

/**
 * @brief Gets the identifier of the second node.
 * 
 * @return int Identifier of the second node.
 */
int Edge::getNode2() const {
    return node2;
}

/**
 * @brief Gets the weight of the edge.
 * 
 * @return double Weight of the edge.
 */
double Edge::getWeight() const {
    return weight;
}

/**
 * @brief Gets the unique identifier of the edge.
 * 
 * @return int Unique identifier of the edge.
 */
int Edge::getId() const {
    return id;
}

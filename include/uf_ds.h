#ifndef UF_DS_H
#define UF_DS_H

#include "Edge.h"

/**
 * @class UF_DS
 * @brief Union-Find / Disjoint Set data structure.
 * 
 * The UF_DS class implements the Union-Find data structure with path compression
 * and union by rank to efficiently manage and merge disjoint sets.
 */
class UF_DS {
public:
    /**
     * @brief Constructs a Union-Find data structure.
     * 
     * Initializes the Union-Find data structure with a specified size.
     * 
     * @param size The number of elements in the Union-Find data structure.
     */
    UF_DS(int size);

    /**
     * @brief Finds the representative of the set containing x.
     * 
     * Uses path compression to make future queries faster.
     * 
     * @param u The element to find the representative of.
     * @return int The representative of the set containing x.
     */
    int find(int u);

    /**
     * @brief Unites the sets containing x and y.
     * 
     * Uses union by rank to keep the tree shallow.
     * 
     * @param u The first element.
     * @param v The second element.
     */
    void unite(int u, int v);

private:
    int* parent; ///< Array to store the parent of each element.
    int* rank; ///< Array to store the rank of each element's tree.
    int size; ///< The number of elements in the Union-Find data structure.
};

#endif

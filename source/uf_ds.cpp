#include "uf_ds.h"

UF_DS::UF_DS(int size) : size(size) {
    parent = new int[size];
    rank = new int[size];
    for (int i = 0; i < size; ++i) {
        parent[i] = i;
        rank[i] = 0;
    }
}

int UF_DS::find(int u) {
    if (parent[u] != u) {
        parent[u] = find(parent[u]); // Path compression
    }
    return parent[u];
}

void UF_DS::unite(int u, int v) {
    int rootU = find(u);
    int rootV = find(v);

    if (rootU != rootV) {
        if (rank[rootU] > rank[rootV]) {
            parent[rootV] = rootU;
        } else if (rank[rootU] < rank[rootV]) {
            parent[rootU] = rootV;
        } else {
            parent[rootV] = rootU;
            rank[rootU]++;
        }
    }
}
import pandas as pd
import igraph as ig
from leidenalg import find_partition, RBConfigurationVertexPartition
import numpy as np


def load_graph_igraph(edge_file):
    edges = pd.read_csv(edge_file)
    edges['weight'] = edges['weight'].astype(float)

    # Apply transformation to weights
    edges['weight'] = 1.0 - edges['weight']

    G = ig.Graph()
    G.add_vertices(edges[['source', 'target']].stack().unique())
    G.add_edges(edges[['source', 'target']].to_records(index=False))
    G.es['weight'] = edges['weight']

    return G


def test_community_detection(G, resolution_range, use_weights=True):
    previous_communities = None
    interval_start = resolution_range[0]

    for resolution in resolution_range:
        partition = find_partition(
            G,
            partition_type=RBConfigurationVertexPartition,
            resolution_parameter=resolution,
            weights=G.es["weight"] if use_weights else None
        )
        current_communities = len(partition)

        if previous_communities is None:
            previous_communities = current_communities

        if current_communities != previous_communities:
            print(f"Resolution interval: [{interval_start}, {resolution}), Communities: {previous_communities}")
            interval_start = resolution

        previous_communities = current_communities

    # Print the last interval
    print(f"Resolution interval: [{interval_start}, {resolution_range[-1]}], Communities: {previous_communities}")


if __name__ == "__main__":
    edge_file = "./temp/mst_edges.csv"

    # Load the graph with log-transformed weights
    G = load_graph_igraph(edge_file)

    # Check graph properties
    print(f"Is the graph connected? {G.is_connected()}")
    print(f"Number of nodes: {G.vcount()}, Number of edges: {G.ecount()}")

    # Test community detection
    print("Testing with log-transformed weights:")
    resolution_range = [i / 10000 for i in range(1, 100)]
    test_community_detection(G, resolution_range, use_weights=True)

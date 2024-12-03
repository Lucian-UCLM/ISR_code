import pandas as pd
import networkx as nx
import leidenalg as la
import igraph as ig


def load_graph_networkx(edge_file):
    edges = pd.read_csv(edge_file)
    edges['weight'] = edges['weight'].astype(float)

    # Apply transformation to weights
    edges['weight'] = 1.0 - edges['weight']

    # Create a NetworkX graph
    G = nx.Graph()
    for _, row in edges.iterrows():
        G.add_edge(row['source'], row['target'], weight=row['weight'])

    return G


def convert_nx_to_igraph(G):
    """Converts a NetworkX graph to an iGraph graph."""
    ig_graph = ig.Graph()
    mapping = {node: idx for idx, node in enumerate(G.nodes())}
    reverse_mapping = {idx: node for node, idx in mapping.items()}
    
    ig_graph.add_vertices(len(mapping))
    ig_graph.add_edges([(mapping[u], mapping[v]) for u, v in G.edges()])
    ig_graph.es['weight'] = [G[u][v]['weight'] for u, v in G.edges()]
    
    return ig_graph, reverse_mapping


def test_community_detection(G, resolution_range, use_weights=True):
    ig_graph, reverse_mapping = convert_nx_to_igraph(G)
    
    previous_communities = None
    interval_start = resolution_range[0]

    for resolution in resolution_range:
        partition = la.find_partition(
            ig_graph,
            la.RBConfigurationVertexPartition,
            resolution_parameter=resolution,
            weights=ig_graph.es["weight"] if use_weights else None
        )
        current_communities = len(partition)

        if previous_communities is None:
            previous_communities = current_communities

        if current_communities != previous_communities:
            print(f"Resolution interval: [{interval_start}, {resolution}), Communities: {previous_communities}")
            interval_start = resolution

        previous_communities = current_communities

    print(f"Resolution interval: [{interval_start}, {resolution_range[-1]}], Communities: {previous_communities}")


if __name__ == "__main__":
    edge_file = "./temp/mst_edges.csv"

    # Load the graph with log-transformed weights
    G = load_graph_networkx(edge_file)

    # Check graph properties
    print(f"Is the graph connected? {nx.is_connected(G)}")
    print(f"Number of nodes: {G.number_of_nodes()}, Number of edges: {G.number_of_edges()}")

    # Test community detection
    print("Testing with log-transformed weights:")
    resolution_range = [i / 10000 for i in range(1, 100)]
    test_community_detection(G, resolution_range, use_weights=True)

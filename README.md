<p align="center">
  <img src="./readme_resources/factify-high-resolution-logo-transparent.png" height="175" width="auto" />
</p>

# Factify

Factify is a powerful tool designed to analyze statements, detect patterns, and perform topic modeling and community detection on structured data. It leverages algorithms for data preprocessing, community detection, and visualization, making it a valuable resource for data scientists and analysts working with large datasets.

## Features

- **Data Preprocessing**: Import, clean, and filter structured statement data for analysis.
- **Topic Modeling**: Generate and assign topic distributions using Mallet.
- **Community Detection**: Perform community detection on graphs using the Leiden algorithm.
- **Graph-Based Analysis**: Build and analyze networks, detect minimum spanning trees (MSTs), and export results in various formats (e.g., CSV, GraphML).
- **Perplexity Metrics**: Evaluate model quality with perplexity calculations and export results for further analysis.

## Usage

### Build and run the standalone target

Use the following commands to build and run the executable target:

```bash
cmake -S all -B build
cmake --build build
.\build\standalone\Debug\Factify.exe
```

### Outputs

Factify generates the following files:

- Mallet Profile Files: Generated `profile1.mallet` file for topic modeling. Other reports are generated with the prefix `profile1` using this `.mallet` file.

- Graph Exports:

  - `mst_with_data.graphml`: A GraphML representation of the minimum spanning tree with node data.
  - `mst_edges.csv`: CSV file containing edges of the MST.
  - `node_data.csv`: CSV file with node metadata.

- Perplexity and Metrics:

  - `means_data.csv`: Perplexity and metrics for evaluating topic models.

### Community Detection

Factify includes a Python utility to detect communities at different resolution parameters using the Leiden algorithm.

Run the Python script to analyze your graph:

```bash
python community_detection.py
```

### Directory Structure

```bash
Factify/
├── input/                # Input files (e.g., JSON, CSV)
├── temp/                 # Generated files, including outputs
├── readme_resources/     # Images and other assets
├── source/               # Source code for Factify
├── include/              # Source code for the interfaces of Factify
├── standalone/source     # Main workflow
└── pyfiles/              # Python script for community detection
```

<p align="center"> Made with ❤️ by the Lucian </p>

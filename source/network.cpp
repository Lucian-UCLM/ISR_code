#include "network.h"
#include <iostream>
#include <algorithm>
#include <thread>
#include <mutex>

void Network::addEdge(int source, int target, double weight, int id) {
    edges.emplace_back(source, target, weight, id);
}

void Network::buildFromSimilarityMatrix(const std::vector<double>& upperTriangle, int numNodes) {
    edges.clear(); // Clear any existing edges
    std::mutex edgeMutex; // Mutex to protect shared resources

    // Determine the number of threads to use
    const size_t numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) {
        throw std::runtime_error("No available threads for multithreading!");
    }

    // Lambda function for calculating the index in the 1D upper triangular matrix
    auto index = [&](size_t i, size_t j) -> size_t {
        return (i * (2 * static_cast<size_t>(numNodes) - i - 1)) / 2 + (j - i - 1);
    };

    // Function to process a range of rows
    auto processRange = [&](size_t start, size_t end, std::vector<Edge>& threadEdges) {
        for (size_t i = start; i < end; ++i) {
            for (size_t j = i + 1; j < static_cast<size_t>(numNodes); ++j) {
                double weight = upperTriangle[index(i, j)];
                if (weight > 0.0) {
                    threadEdges.emplace_back(static_cast<int>(i), static_cast<int>(j), weight, 0);
                }
            }
        }
    };

    // Divide the work among threads
    std::vector<std::thread> threads;
    std::vector<std::vector<Edge>> threadResults(numThreads); // Store results from each thread
    size_t rowsPerThread = numNodes / numThreads;

    for (size_t t = 0; t < numThreads; ++t) {
        size_t start = t * rowsPerThread;
        size_t end = (t == numThreads - 1) ? static_cast<size_t>(numNodes) : start + rowsPerThread;

        threads.emplace_back(processRange, start, end, std::ref(threadResults[t]));
    }

    // Join all threads
    for (auto& thread : threads) {
        thread.join();
    }

    // Combine results from all threads
    for (const auto& threadEdges : threadResults) {
        edges.insert(edges.end(), threadEdges.begin(), threadEdges.end());
    }

    // Assign unique IDs to edges
    for (size_t id = 0; id < edges.size(); ++id) {
        edges[id] = Edge(edges[id].getNode1(), edges[id].getNode2(), edges[id].getWeight(), static_cast<int>(id));
    }

    // Sort edges by weight
    std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.getWeight() < b.getWeight();
    });
}


const std::vector<Edge>& Network::getEdges() const {
    return edges;
}

void Network::printNetwork() const {
    for (const auto& edge : edges) {
        std::cout << "Edge ID: " << edge.getId()
                  << " Source: " << edge.getNode1()
                  << " Target: " << edge.getNode2()
                  << " Weight: " << edge.getWeight()
                  << std::endl;
    }
}

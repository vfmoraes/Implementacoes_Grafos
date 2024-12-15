#ifndef DISJOINTSET_HPP // Check if STRUCTURES_HPP is not defined
#define DISJOINTSET_HPP // Define STRUCTURES_HPP

#include <iostream>
#include <vector>
#include <climits>
#include "./structures.hpp"

using namespace std;



/**
 * @class DisjointSet
 * @brief Implements a Disjoint Set (Union-Find) data structure with additional functionalities for segmentation.
 * 
 * Provides an efficient data structure for managing disjoint sets, with support for:
 * - Union-Find operations with rank and path compression optimizations.
 * - Tracking additional properties of sets, such as maximum edge weight and size.
 *   
 * As opposed to the ordinary data structure, this data structure includes additional fields such as 
 * maximum edge weight and size. These fields are used for efficient discovery of each set properties.
 * 
 * @note The class is primarily used for image segmentation and other graph-based algorithms.
 */
class DisjointSet {
 private:
    int quantity; // Number of sets
    vector<int> parent; // Tracks the parent of each element
    vector<int> rank;   // Tracks the rank (approximate tree height)
    vector<int> maxWeight;   // Tracks the maximum edge of each set
    vector<int> size;   // Tracks the size of each set


    // Update the maximum weight of a conjunct
    void updateWeight(int weight, int parent) {
        if(weight > maxWeight[find(parent)]) {
            maxWeight[find(parent)] = weight;
        }
    }
    
    // Find operation with path compression
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // Path compression
        }
        return parent[x];
    }

    // Get the maximum weight of a conjunct
    int getMaxWeight(int x) {
        return maxWeight[find(x)];
    }

    // Update the size of a conjunct
    void updateSize(int x) {
        size[find(x)]++;
    }

    // Calculate the threshold of a conjunct
    int Threshold(int x, int k) {
        return k/getSize(x);
    }
 public:
     // Get the size of a conjunct
    int getSize(int x) {
        return size[find(x)];
    }
    
    // Constructor: Initialize the data structure
    DisjointSet(int n) {
        parent.resize(n);
        rank.resize(n, 0); // Initialize all ranks to 0
        maxWeight.resize(n, 0); // Initialize all weight to 0
        size.resize(n, 1); // Initialize all size to 1
        quantity = n;
        for (int i = 0; i < n; ++i) {
            parent[i] = i; // Each element is its own parent initially
        }
    }

    // Union operation with rank optimization
    void unionSets(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);

        if (rootX != rootY) {
            if (rank[rootX] > rank[rootY]) {
                parent[rootY] = rootX; // Attach the smaller tree under the larger one
            } else if (rank[rootX] < rank[rootY]) {
                parent[rootX] = rootY;
            } else {
                parent[rootY] = rootX;
                rank[rootX]++; // Increment rank if both trees are of the same height
            }
            quantity--; // Decrement the number of sets
        }
    }

    // Union operation with rank optimization of an edge
    void unionSets(Edge e) {
        int rootX = find(e.v1);
        int rootY = find(e.v2);

        if (rootX != rootY) {
            if (rank[rootX] > rank[rootY]) {
                parent[rootY] = rootX; // Attach the smaller tree under the larger one
                updateWeight(e.weight, rootY);
                updateSize(rootX);
            } else if (rank[rootX] < rank[rootY]) {
                parent[rootX] = rootY;
                updateWeight(e.weight, rootX);
                updateSize(rootY);
            } else {
                parent[rootY] = rootX;
                rank[rootX]++; // Increment rank if both trees are of the same height
                updateWeight(e.weight, rootX);
                updateSize(rootX);
            }
        }
        quantity--; // Decrement the number of sets
    }

    // Utility to check if two elements are in the same set
    bool isSameSet(int x, int y) {
        return find(x) == find(y);
    }

    // Get the minimum weight of conjuncts disjoint
    int MinInt(int a, int b, int k) {
        return min(getMaxWeight(a) + Threshold(a, k), getMaxWeight(b) + Threshold(b, k)); //Minimum of the maximum weight of the conjuncts disjoint
    }

    int getQuantity() {
        return quantity;
    }

    vector<vector<int>> toMatrix(int width, int height) {
        vector<vector<int>> matrix(width, vector<int>(height));
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                matrix[i][j] = find(i * height + j);
            }
        }
        return matrix;
    }

    //----------------- Debugging functions -----------------//
    void printSets() {
        for (int i = 0; i < parent.size(); ++i) {
            cout << "Element: " << i << " Parent: " << parent[i] << " Rank: " << rank[i] << " MaxWeight: " << maxWeight[i] << " Size: " << size[i] << endl;
        }
    }
    // Utility to print the elements and their sets
    void isSameSetPrint(int x, int y) {
        if (isSameSet(x, y)) {
            cout << x << " and " << y << " are in the same set" << endl;
        } else {
            cout << x << " and " << y << " are in different sets" << endl;
        }
    }
    void printConjuncts(int width, int height) {
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                cout << find(i * width + j) << " ";
            }
            cout << "\n";
        }
    }
};


/** 
 * @brief Segments a graph using the DisjointSet data structure.
 * 
 * Execution of algorithm directed for image segmentation proposed by Pedro F. Felzenszwalb and Daniel P. Huttenlocher
 * 
 * Each edge received corresponds to the dissimilarity between two vertexes. 
 * The higher the value, the more dissimilar they are. The edges are ordered in
 * non decrescent form, as to priorize edges that represent similarity
 *
 * Use of disjoint set data structure unite different components as trees when they 
 * are similar enough when compared to their internal dissimilarity. 
 * 
 * As the edges of lowest weight are always chosen first, each generated tree is also
 * a minimum spanning tree for the vertexes contained inside
 *
 * 
 * @param n The number of nodes in the graph.
 * @param k A constant parameter for threshold calculation.
 * @param edges A list of edges representing the graph. Each edge connects two nodes with a specified weight.
 *              The edge list is supposed to be ordered non decrescently
 *
 * @return A DisjointSet representing the segmented graph.
 * 
 * @note The input edges should be sorted in non-decreasing order of weight for optimal results.
 */
DisjointSet segmentation(int n, int k, std::vector<Edge> edges) {
    DisjointSet ds(n);
    for (Edge e : edges){
        if (!ds.isSameSet(e.v1, e.v2) && e.weight <= ds.MinInt(e.v1, e.v2, k)) {
            ds.unionSets(e);
        }
    }
    return ds;
}

#endif
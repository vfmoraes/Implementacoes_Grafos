#ifndef STRUCTURES_HPP // Check if STRUCTURES_HPP is not defined
#define STRUCTURES_HPP // Define STRUCTURES_HPP

#include <iostream>
#include <vector>
#include <sstream>
#include <string> // Ensure all dependencies are included

using std::vector;
using std::string;
using std::ostringstream;
using namespace std; 

/**
 * @class Edge
 * @brief Edge struct used for a list of Edges without adjacency context
 */        
struct Edge {
    int v1, v2, weight;
};


/**
 * Obtains formatted string containing vertexes and edges of the graph
 * 
 * The vertexes are displayed as their position inside the image matrix, as a pair
 * of vertical and horizontal values (column and row)
 * 
 * Format: 
 * Edge(neighbor: {(h1, w1), (h2,hw)}, weight: w
 * 
 * @param edge The edge to be converted to string
 * @param width The width used to calculate which row and column we are in
 * @return String result
 */
string matrixEdgeToString(const Edge& adjEdge, int width) {
    ostringstream os;

    int weight = adjEdge.weight;
    // Calculate the row (i) and column (j) based on the 1D index
    int h1 = adjEdge.v1 / width;
    int w1 = adjEdge.v1 % width;

    int h2 = adjEdge.v2 / width;
    int w2 = adjEdge.v2 % width;
    
    os << "Edge(neighbor: {";
    os << "(" + std::to_string(h1) + ", " + std::to_string(w1) + "), ";
    os << "(" + std::to_string(h2) + ", " + std::to_string(w2) + ")}, ";
    os <<  "weight: " + std::to_string(weight) + ")";

    return os.str();
}


/**
 * Prints list of edges passed as parameter
 * 
 * Format: 
 * [
 *    Edge(neighbor: {(h1, w1), (h2,hw)}, weight: w
 *    ...
 *    Edge(neighbor: {(h1, w1), (h2,hw)}, weight: w
 * ]
 * 
 * @param edge List of edges
 * @param width Width used to calculate which row and column we are in
 * @return String result
 */
string matrixEdgeListToString(const vector<Edge>& edges, int width) {
    std::ostringstream os;

    os << "[" << endl;
    for (const auto& edge : edges) {
        os << "  " << matrixEdgeToString(edge, width) << endl; 
    }
    os << "]";

    return os.str();
}


/**
 * Overcharge << operator for std::ostream
 * 
 * Format:
 * 
 * Matrix:
 * [
 *    r0c0 r0c1 ... r0cn 
 *    ...
 *    rnc0 rnc1 ... rncn
 * ]
 * 
 * @param edge Matrix
 * @return String result
 */
std::ostream& operator<<(std::ostream& os, const vector<vector<int>>& matrix) {
    os << "Matrix:" << endl << "[" << endl;
    for (const vector<int>& row : matrix) {
        os << "  ";
        for (const int pixel : row) {
            os << pixel << " ";
        }
        os << endl;
    }
    os << "]";
    return os;
}

#endif // STRUCTURES_HPP
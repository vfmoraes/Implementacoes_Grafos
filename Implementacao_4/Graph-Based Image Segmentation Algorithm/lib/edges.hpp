#ifndef M_PI 
    #define M_PI 3.14159265358979323846
#endif


#ifndef EDGES_HPP // Check if STRUCTURES_HPP is not defined
#define EDGES_HPP // Define STRUCTURES_HPP


#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "./PgmToMatrix.hpp"
#include "./PythonScripts.hpp"
#include "./DisjointSet.hpp"
#include "./MatrixToPgm.hpp"





using namespace std;



/** 
 * Ensures PgmMatrix is a proper matrix with rows of same size
 * 
 * @param pgmMatrix Matrix asserted
 * @return false when the assert failed and on the rows is of different size
 *         true otherwise 
 */
bool assertPgmMatrixSize(const vector<vector<int>>& pgmMatrix) {
    int height = pgmMatrix[0].size();
    for (int i = 1; i < pgmMatrix.size(); i++) {
        if (height != pgmMatrix[i].size()) return false;
    }


    return true;
}



/** 
 * Creates non directional edge list from image matrix
 * 
 * The edges created follow a matrix graph, where each vertex v is connected 
 * to 8 surrounding vertexes as an neighbor
 * 
 * @param pgmMatrix Base matrix 
 * @return List of non ordered edges
 */
vector<Edge> createEdgeList(const vector<vector<int>>& pgmMatrix) {
    bool validMatrix = assertPgmMatrixSize(pgmMatrix);
    if (!validMatrix) throw invalid_argument("Invalid matrix");

    int height = pgmMatrix.size();
    int width = pgmMatrix[0].size();

    int edgesListSize = (height+width)*4 - width - height;
    vector<Edge> edges;

    vector<pair<int, int>> directions = {
        {-1, -1}, {-1, 0}, {-1, 1}, 
        {0, -1}, {0, 1}, 
        {1, -1}, {1, 0}, {1, 1}
    };
    // vector<pair<int, int>> directions = {
    //     {-1, 0}, {0, -1}, {0, 1}, 
    //     {1, 0}
    // };
    for(int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int curVertex = i * width + j;

            for (const auto& dir : directions) {
                // neighbors i and j
                int ni = i + dir.first;  
                int nj = j + dir.second; 


                // within bounds
                if (ni >= 0 && ni < height && nj >= 0 && nj < width) {
                    
                    int nextVertex = ni * width + nj;  

                    //std::cout << "curVertex: " << curVertex << ", nextVertex: " << nextVertex << "\n";

                    int weight = abs(pgmMatrix[i][j] - pgmMatrix[ni][nj]);
                    
                    //std::cout << "Weight between (" << i << ", " << j << ") and (" << ni << ", " << nj << "): " << weight << "\n";

                    // Add edge to edge list (avoid duplicates)
                    if (i < ni || (i == ni && j < nj)) {
                        Edge edge = {curVertex, nextVertex, weight};
                        edges.push_back(edge);
                    }

   
                }
            }        
        }
    }

    return edges;
}




/** 
 * Helper function for gaussian calculations that generates a gaussian kernel
 * Both the sigma and size can be provided to alter the effects of the smoothing
 * 
 * @param size Size of square matrix, which can be seen as the size of its edge
 * @param sigma Constant used to determine smoothing level
 * @return A square matrix containing the normalized kernel values
 */
vector<vector<double>> _generateGaussianKernel(int size, double sigma) {
    vector<vector<double>> kernel(size, vector<double>(size));
    double sum = 0.0; // To normalize the kernel
    int half = size / 2;

    // Create the kernel
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            double x = i - half;
            double y = j - half;
            kernel[i][j] = exp(-(x * x + y * y) / (2 * sigma * sigma)) / (2 * M_PI * sigma * sigma);
            sum += kernel[i][j];
        }
    }

    // Normalize the kernel
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            kernel[i][j] /= sum;

    return kernel;
}


/** 
 * Gaussian filter smoothing to promote proximity between similar neighboring values in the matrix.
 * Pixels with non similar neighboring values will be less likened
 * 
 * Both the sigma and kernel size can be provided to alter the effects of the smoothing
 * 
 * @param matrix Matrix of ints containing structure data (in most cases here, an image)
 * @param kernelSize Constant used to determine size of one of the square edges o the kernel
 * @param sigma Constant used to determined smoothing level
 * 
 * @return New corresponding matrix with smoothed values
 */
vector<vector<int>> applyGaussianFilter(const vector<vector<int>> &matrix, int kernelSize, double sigma) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    int half = kernelSize / 2;

    // Generate the Gaussian kernel
    auto kernel = _generateGaussianKernel(kernelSize, sigma);

    // Create an output matrix initialized to 0
    vector<vector<int>> filteredMatrix(rows, vector<int>(cols, 0));

    // Apply the kernel to the matrix
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            double sum = 0.0;

            // Convolve the kernel
            for (int ki = -half; ki <= half; ++ki) {
                for (int kj = -half; kj <= half; ++kj) {
                    int ni = i + ki; // Neighbor row
                    int nj = j + kj; // Neighbor column

                    // Check boundaries
                    if (ni >= 0 && ni < rows && nj >= 0 && nj < cols) {
                        sum += matrix[ni][nj] * kernel[ki + half][kj + half];
                    }
                }
            }

            // Assign the computed value to the output matrix
            filteredMatrix[i][j] = static_cast<int>(std::round(sum));
        }
    }

    return filteredMatrix;
}




bool compareEdges(const Edge& e1, const Edge& e2) {
    if (e1.weight != e2.weight) return e1.weight < e2.weight;
    if (e1.v1 != e2.v1) return e1.v1 < e2.v1;
    return e1.v2 < e2.v2;
}

#endif
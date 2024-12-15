#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "./lib/pgmToMatrix.hpp"
#include "./lib/PythonScripts.hpp"
#include "./lib/DisjointSet.hpp"
#include "./lib/MatrixToPgm.hpp"
#include "./lib/edges.hpp"


int main() {
    try {
        generatePgmFromFilename(); // Generate a PGM file from a PNG file

        vector<vector<int>> matrix = readPGM("./output/original.pgm"); // Read the PGM file and return a matrix of pixels

        vector<vector<int>> smoothedMatrix = applyGaussianFilter(matrix, 3, 0.8f);

        // cout << matrix.size() << endl << smoothedMatrix[0].size() << endl;
        // cout << matrix << endl;

        auto res = createEdgeList(smoothedMatrix);
        vector<Edge> edges = res;
        sort(edges.begin(), edges.end(), compareEdges);
        
        // std::cout << matrixEdgeListToString(edges, matrix[0].size()) << endl;
        // std::cout << matrixGraphToString(res.first, matrix[0].size()) << endl;

        int halfPerimeter = (matrix.size() + matrix[0].size());
        int threshold = halfPerimeter <= 260 ? 150 : halfPerimeter <= 600 ? 300 : 500;
        // int threshold = 250

        DisjointSet ds = segmentation(matrix.size()*matrix[0].size(), threshold, edges); // Segment the image (number of vertices, threshold, edges)
        
        // ds.printConjuncts(matrix[0].size(), matrix.size());

        std::cout << endl << ds.getQuantity() << endl;

        colorpgm::MatrixToPGM(ds.getQuantity(), ds.toMatrix(matrix.size(), matrix[0].size()), "./output/converted.pgm");
        
    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
    }
} 
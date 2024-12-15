#ifndef PGMTOMATRIX_HPP // Check if STRUCTURES_HPP is not defined
#define PGMTOMATRIX_HPP // Define STRUCTURES_HPP


#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

typedef int Pixel;

/** 
 * Open readily formatted pgm image file 
 * 
 * Read and return pgm image as a matrix of integers
 * The pgm file read here is a simpler version where the values are stored as char
 *
 * @param filename The file name to read.
 * @return A matrix composed of a vector of vectors of int.
 *
 * @error If either the file could not be open or does not exist, 
 *        a runtime_error will be sent and the function stop
 * 
 * @error If the read file does not contain expected prefix, it is marked invalid 
 *        and a runtime_error will be sent
 */
vector<vector<Pixel>> readPGM(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Não foi possível abrir o arquivo.");
    }

    string line;
    getline(file, line);  // Read P2 header
    if (line != "P2") {
        throw runtime_error("Formato não suportado. Apenas P2 é suportado.");
    }

    // Ignore comments
    while (getline(file, line) && line[0] == '#');

    // read dimensions
    istringstream dimensions(line);
    int width, height;
    dimensions >> width >> height;

    int maxVal;
    file >> maxVal;

    // Read pixel values
    vector<vector<Pixel>> matrix(height, vector<Pixel>(width));

    for (int x = 0; x < height; ++x) {
        for (int y = 0; y < width; ++y) {
            int pixelValue;
            file >> pixelValue;
            matrix[x][y] = pixelValue;
            // cout << pixelValue << " ";
        }
        // cout << endl;
    }

    file.close();
    return matrix;
}

// int main() {
//     try {
//         string filename = "teste.pgm";
//         auto matrix = readPGM(filename);

//         // Exemplo: imprimir os valores da matriz
//         for (const auto& row : matrix) {
//             for (const auto& pixel : row) {
//                 cout << pixel.value << " ";
//             }
//             cout << endl;
//         }
//     } catch (const exception& e) {
//         cerr << "Erro: " << e.what() << endl;
//     }
//     return 0;
// }

#endif
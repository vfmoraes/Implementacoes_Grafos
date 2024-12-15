#ifndef MATRIXTOPGM_HPP // Check if STRUCTURES_HPP is not defined
#define MATRIXTOPGM_HPP // Define STRUCTURES_HPP


#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <cmath>
#include "./PythonScripts.hpp"


using namespace std;

namespace colorpgm {

    static const int COLOR_MAX = 255;

    /**
    * @class RGBPixel
    * @brief Stores red green blue values
    */
    struct RGBPixel {
        int r, g, b;
    };


    /**
    * Function to remap a matrix of integers to RGB color values.
    *
    * Considers a pixel as part of the background, painted as black.
    * Every group that is not the group of the background is painted as white
    *
    * @param matrix The 2D matrix of integers to remap.
    * @param groupSize The number of unique colors to use for remapping.
    * @return A 2D vector of `RGBPixel` representing the remapped colors.
    */
    vector<vector<RGBPixel>> _dualColorRepresentation(const vector<vector<int>>& matrix, int groupSize, int bgGroup) {  
        vector<vector<RGBPixel>> rgbMatrix(matrix.size(), vector<RGBPixel>(matrix[0].size())); 
        
        for (int i = 0; i < matrix.size(); i++) {
            for (int j = 0; j < matrix[0].size(); j++) {
            int value = matrix[i][j]; 
            
            if (matrix[i][j] == bgGroup) {
                rgbMatrix[i][j] = {0, 0, 0};
            } else {
                rgbMatrix[i][j] = {255, 255, 255};
            }
            }
        }

        return rgbMatrix;

    }

    /**
    * Implentation of dualColorRepresentation
    *
    * Considers top left corner pixel as always part of the background group
    *
    * @param matrix The 2D matrix of integers to remap.
    * @param groupSize The number of unique colors to use for remapping.
    * @return A 2D vector of `RGBPixel` representing the remapped colors.
    */
    vector<vector<RGBPixel>> _bgDualColorRepresentation(const vector<vector<int>>& matrix, int groupSize) {  
        return _dualColorRepresentation(matrix, groupSize, matrix[0][0]);

    }




    /**
    * Function to remap a matrix of integers to RGB color values.
    *
    * Uses only one channel (red) and the other channels are offset of red channel by a constant (1)
    * Does not represent a lot of groups well!
    *
    * @param matrix The 2D matrix of integers to remap.
    * @param groupSize The number of unique colors to use for remapping.
    * @return A 2D vector of `RGBPixel` representing the remapped colors.
    */
    vector<vector<RGBPixel>> _uniqueChannelMatrixToColorRGB(const vector<vector<int>>& matrix, int groupSize) {
        vector<vector<RGBPixel>> rgbMatrix(matrix.size(), vector<RGBPixel>(matrix[0].size()));

        vector<int> colors(groupSize);
        int divisions = COLOR_MAX/min((groupSize-1), 255);
        for (int i = 0; i < groupSize; i++) {
            colors[i] = (divisions*i)%255;
        }

        unordered_map<int, RGBPixel> colorMap; // Mapping matrix values to RGB colors
        int k = 0;

        for (int i = 0; i < matrix.size(); i++) {
            for (int j = 0; j < matrix[0].size(); j++) {
                int value = matrix[i][j];

                // If the value is already mapped to a color, use it
                if (colorMap.find(value) != colorMap.end()) {
                    rgbMatrix[i][j] = colorMap[value];
                } else {
                    // If it's a new value, assign an RGB color
                    if (k < groupSize) {
                        RGBPixel newColor = {
                            colors[k % groupSize],       // Red
                            colors[(k + 1) % groupSize], // Green
                            colors[(k + 2) % groupSize]  // Blue
                        };

                        colorMap[value] = newColor;
                        rgbMatrix[i][j] = newColor;
                        k++;
                    } else {
                        cerr << "Error: More unique values than available colors." << endl;
                        return {};
                    }
                }
            }
        }

        return rgbMatrix;
    }


    /**
    * Function to remap a matrix of integers to RGB color values.
    *
    * Attempts to spread colors evenly between channels
    * A step is generated where colorNum * step will result in the maximum spectrum of colors
    * 
    * Each step you move from red to green - a single step might also update all colors
    *
    * @param matrix The 2D matrix of integers to remap.
    * @param groupSize The number of unique colors to use for remapping.
    * @return A 2D vector of `RGBPixel` representing the remapped colors.
    */
    constexpr int spectrum = 255.0 * 255.0 * 255.0;
    vector<vector<RGBPixel>> _matrixToColorRGB(const vector<vector<int>>& matrix, int colorNum) {
        vector<vector<RGBPixel>> rgbMatrix(matrix.size(), vector<RGBPixel>(matrix[0].size()));

        // Use an unordered map to assign unique RGB colors to each value in the matrix
        unordered_map<int, RGBPixel> colorMap;
        
        // Square root of 3 over the spectrum over colorNum
        int step = max(1, static_cast<int>(pow(spectrum / colorNum, 1.0 / 3.0)));
        
        int r = 0, g = 0, b = 0; // Initial RGB values

        for (int i = 0; i < matrix.size(); i++) {
            for (int j = 0; j < matrix[0].size(); j++) {
                int value = matrix[i][j];

                // If the value already has a color, reuse it
                if (colorMap.find(value) != colorMap.end()) {
                    rgbMatrix[i][j] = colorMap[value];
                } else {
                    // Assign a new RGB color
                    RGBPixel newColor = { r, g, b };
                    colorMap[value] = newColor;
                    rgbMatrix[i][j] = newColor;

                    // Increment RGB values using the calculated step
                    r += step;
                    if (r > 255) {
                        r -= 256;
                        g += step;
                        if (g > 255) {
                            g -= 256;
                            b += step;
                            if (b > 255) {
                                cerr << "Error: Exceeded maximum color combinations." << endl;
                                return {};
                            }
                        }
                    }
                }
            }
        }

        return rgbMatrix;
    }




    /**
    * Function that creates a PGM-like file using RGB values.
    *
    * @param colorQuantity The number of colors in the palette.
    * @param matrix The input matrix of integers.
    * @param filename The name of the output file.
    * @param colorFunction Function used to define how to convert the matrix groupings into color
    */
    void _baseMatrixToPGM(
        int colorQuantity, 
        vector<vector<int>> matrix, 
        string filename,
        vector<vector<RGBPixel>> (*colorFunction)(const vector<vector<int>>&, int)
    ) {
        ofstream file = ofstream(filename);
        
        unordered_map<int, int> colorMapR, colorMapG, colorMapB;
        vector<vector<RGBPixel>> rgbMatrix = colorFunction(matrix, colorQuantity);

        if (file.is_open()) {
            file << "P2" << endl; // P3 for ASCII RGB format
            file << matrix[0].size() << " " << matrix.size() << endl;
            file << "255" << endl;

            for (int i = 0; i < matrix.size(); i++) {
            for (int j = 0; j < matrix[i].size(); j++) {
                RGBPixel value = rgbMatrix[i][j];
                file << value.r << " " 
                    << value.g << " " 
                    << value.b << " ";
            }
            file << endl;
            }

            // Call an external script if needed
            executePythonScript(pgm3ToPngScript);
        } else {
            cout << "Error: Unable to open file" << endl;
        }
    }

    void MatrixToPGM(int colorQuantity, vector<vector<int>> matrix, string filename) {
        _baseMatrixToPGM(colorQuantity, matrix, filename, _matrixToColorRGB);
    }

    void UniqueChannelMatrixToPGM(int colorQuantity, vector<vector<int>> matrix, string filename) {
        _baseMatrixToPGM(colorQuantity, matrix, filename, _uniqueChannelMatrixToColorRGB);
    }

    void DualRepresentationMatrixToPGM(int colorQuantity, vector<vector<int>> matrix, string filename) {
        _baseMatrixToPGM(colorQuantity, matrix, filename, _bgDualColorRepresentation);
    }
} 

#endif
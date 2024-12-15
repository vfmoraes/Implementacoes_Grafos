#include <algorithm>
#include <cmath>
#include <iostream>
#include <queue>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <limits>
#include <unordered_map>
#include "graph.cpp"

using namespace std;

vector<bool> createSegmentationMask(int width, int height, const std::vector<int> &setS, const std::vector<int> &setT)
{
    vector<bool> segmentationMask(width * height, false);

    for (int pixel : setS)
    {
        if (pixel < width * height)
        {
            segmentationMask[pixel] = true;
        }
    }

    return segmentationMask;
}

vector<int> readPixelsLine(ifstream &file, int width)
{
    vector<int> pixels(width);
    for (int i = 0; i < width; ++i)
    {
        file >> pixels[i];
    }
    return pixels;
}

vector<int> readPGM(const string &filename, int &width, int &height)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        throw runtime_error("Não foi possível abrir o arquivo.");
    }

    string line;
    getline(file, line);
    if (line != "P2")
    {
        throw runtime_error("Formato não suportado. Apenas P2 é suportado.");
    }

    while (getline(file, line) && line[0] == '#')
        ;

    istringstream dimensions(line);
    dimensions >> width >> height;

    int maxVal;
    file >> maxVal;

    vector<int> pixels(width * height);

    int index = 0;
    while (index < width * height)
    {
        vector<int> linePixels = readPixelsLine(file, width);
        for (int i = 0; i < width; ++i)
        {
            pixels[index++] = linePixels[i];
        }
    }

    file.close();
    return pixels;
}

void MatrixToPGM(const vector<bool>& a, const vector<int>& matrix, int width, int height, const string& filename) {
    ofstream file(filename);

    if (file.is_open()) {
        file << "P2" << endl;
        file << width << " " << height << endl;
        file << "255" << endl;

        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                int index = i * width + j;

                if (a[index]) {
                    file << " 0 0 " << matrix[index] << " ";
                } else {
                    file << " " << matrix[index] << " 0 0 ";
                }
            }
            file << endl;
        }
    } else {
        cout << "Erro ao abrir o arquivo!" << endl;
    }
}

void computeHistograms(const vector<int> &image, vector<int> &objectHistogram, vector<int> &backgroundHistogram)
{
    objectHistogram.assign(256, 0);
    backgroundHistogram.assign(256, 0);

    int sum = 0;
    int count = 0;
    for (int intensity : image)
    {
        sum += intensity;
        count++;
    }
    int threshold = sum / count;

    for (int intensity : image)
    {
        if (intensity <= threshold)
        {
            backgroundHistogram[intensity]++;
        }
        else
        {
            objectHistogram[intensity]++;
        }
    }
}

int main()
{
    int width = 150;
    int height = 150;

    vector<int> image = readPGM("teste.pgm", width, height);

    vector<int> objectHistogram(256, 0);
    vector<int> backgroundHistogram(256, 0);

    computeHistograms(image, objectHistogram, backgroundHistogram);

    double sigma = 100.0;
    double lambda = 20.0;

    int source = width * height;
    int sink = source + 1;

    Graph graph(source + 2, source, sink);

    graph.compute_tlinks(image, objectHistogram, backgroundHistogram);
    graph.compute_nlinks(image, width, height, sigma, lambda);

    vector<int> setS, setT;
    int maxFlow = graph.fordFulkerson(setS, setT);

    vector<bool> segmentationMask = createSegmentationMask(width, height, setS, setT);

    cout << "Segmentation Mask:" << endl;
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int pixel = y * width + x;
            cout << (segmentationMask[pixel] ? "1 " : "0 ");
        }
        cout << endl;
    }

    MatrixToPGM(segmentationMask, image, width, height, "segmented_output.pgm");

    return 0;
}
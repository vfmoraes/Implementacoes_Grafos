#include <algorithm>
#include <cmath>
#include <iostream>
#include <queue>
#include <vector>
#include <climits>
#include <unordered_map>

using namespace std;

class Graph
{
private:
    int vertices;
    int source;
    int sink;
    unordered_map<int, unordered_map<int, int>> capacity;
    unordered_map<int, unordered_map<int, int>> flow;

public:
    Graph(int vertices, int source, int sink)
        : vertices(vertices), source(source), sink(sink) {}

    void add_edge(int u, int v, int cap)
    {
        capacity[u][v] = cap;
        capacity[v][u] = 0; // aresta inversa (grafo residual)
    }

    void add_tlink(int pixel, int sourceWeight, int sinkWeight)
    {
        add_edge(source, pixel, sourceWeight);
        add_edge(pixel, sink, sinkWeight);
    }

    void add_nlink(int pixel1, int pixel2, int weight)
    {
        add_edge(pixel1, pixel2, weight);
        add_edge(pixel2, pixel1, weight); // Aresta bidirecional
    }

    void compute_tlinks(const vector<int> &image,
                        const vector<int> &objectHistogram,
                        const vector<int> &backgroundHistogram)
    {
        int maxIntensity = objectHistogram.size();
        for (int pixel = 0; pixel < image.size(); ++pixel)
        {
            int intensity = image[pixel];
            double P_object = max(min(static_cast<double>(objectHistogram[intensity]) / maxIntensity, 1.0), 1e-6);
            double P_background = max(min(static_cast<double>(backgroundHistogram[intensity]) / maxIntensity, 1.0), 1e-6);

            int sourceWeight = -log(P_object);
            int sinkWeight = -log(P_background);

            add_tlink(pixel, sourceWeight, sinkWeight);
        }
    }

    void compute_nlinks(const vector<int> &image, int width, int height, double sigma, double lambda)
    {
        auto squaredDifference = [](int a, int b)
        {
            return (a - b) * (a - b);
        };

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                int pixel = y * width + x;

                if (x + 1 < width)
                {
                    int neighbor = pixel + 1;
                    int weight = static_cast<int>(lambda * exp(-squaredDifference(image[pixel], image[neighbor]) / (2 * sigma * sigma)));
                    add_nlink(pixel, neighbor, weight);
                }

                if (y + 1 < height)
                {
                    int neighbor = pixel + width;
                    int weight = static_cast<int>(lambda * exp(-squaredDifference(image[pixel], image[neighbor]) / (2 * sigma * sigma)));
                    add_nlink(pixel, neighbor, weight);
                }
            }
        }
    }

    bool findAugmentingPath(vector<int> &parent)
    {
        vector<bool> visited(vertices, false);
        queue<int> q;

        q.push(source);
        visited[source] = true;
        parent[source] = -1;

        while (!q.empty())
        {
            int u = q.front();
            q.pop();

            for (const auto &[v, cap] : capacity[u])
            {
                if (!visited[v] && cap - flow[u][v] > 0)
                {
                    parent[v] = u;
                    if (v == sink)
                    {
                        return true;
                    }
                    q.push(v);
                    visited[v] = true;
                }
            }
        }
        return false;
    }

    int pushFlow(vector<int> &parent)
    {
        int pathFlow = INT_MAX;

        for (int v = sink; v != source; v = parent[v])
        {
            int u = parent[v];
            pathFlow = min(pathFlow, capacity[u][v] - flow[u][v]);
        }

        for (int v = sink; v != source; v = parent[v])
        {
            int u = parent[v];
            flow[u][v] += pathFlow;
            flow[v][u] -= pathFlow;
        }

        return pathFlow;
    }

    int fordFulkerson(vector<int> &setS, vector<int> &setT)
    {
        int maxFlow = 0;
        vector<int> parent(vertices);

        while (findAugmentingPath(parent))
        {
            maxFlow += pushFlow(parent);
        }

        vector<bool> visited(vertices, false);
        queue<int> q;
        q.push(source);
        visited[source] = true;

        while (!q.empty())
        {
            int u = q.front();
            q.pop();

            for (const auto &[v, cap] : capacity[u])
            {
                if (!visited[v] && cap - flow[u][v] > 0)
                {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }

        for (int i = 0; i < vertices; ++i)
        {
            if (visited[i])
            {
                setS.push_back(i);
            }
            else
            {
                setT.push_back(i);
            }
        }

        return maxFlow;
    }
};
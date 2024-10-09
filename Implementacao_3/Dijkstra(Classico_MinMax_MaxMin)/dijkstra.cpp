#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <climits>

using namespace std;

// Definição do tipo para representar o grafo (lista de adjacência)
typedef pair<int, int> pii;

int dijkstra(int n, vector<vector<pii>>& graph, int src, int dest) {
    vector<int> dist(n, INT_MAX);    

    priority_queue<pii, vector<pii>, greater<pii>> pq;
    
    dist[src] = 0;
    pq.push({0, src});
    
    while (!pq.empty()) {
        int current_dist = pq.top().first;
        int current_node = pq.top().second;
        pq.pop();
        
        if (current_node == dest) {
            break;
        }
        
        if (current_dist > dist[current_node]) {
            continue;
        }
        
        for (auto& neighbor : graph[current_node]) {
            int adjacent = neighbor.first;
            int weight = neighbor.second;
            int distance = current_dist + weight;
            
            if (distance < dist[adjacent]) {
                dist[adjacent] = distance;
                pq.push({distance, adjacent});
            }
        }
    }

    return (dist[dest] == INT_MIN) ? -1 : dist[dest];
}

int dijkstraMinimax(int n, vector<vector<pii>>& graph, int src, int dest) {
    vector<int> maxWeight(n, INT_MAX);
    priority_queue<pii, vector<pii>, greater<pii>> pq;

    maxWeight[src] = 0;
    pq.push({0, src}); // (peso, nó)


    while (!pq.empty()) {
        int currentMax = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (u == dest) {
            break;
        }

        if (currentMax > maxWeight[u]) {
            continue;
        }


        for (const auto& neighbor : graph[u]) {
            int v = neighbor.first;
            int weight = neighbor.second;

            int newMaxWeight = max(currentMax, weight);

            if (newMaxWeight < maxWeight[v]) {
                maxWeight[v] = newMaxWeight;
                pq.push({newMaxWeight, v});
            }
        }
    }

    return (maxWeight[dest] == INT_MAX) ? -1 : maxWeight[dest];
}

int dijkstraMaximin(int n, vector<vector<pii>>& graph, int src, int dest) {
    vector<int> minWeight(n, INT_MIN);
    priority_queue<pii> pq;

    minWeight[src] = INT_MAX;
    pq.push({INT_MAX, src});

    while (!pq.empty()) {
        int currentMin = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (u == dest) {
            break; 
        }

        if (currentMin < minWeight[u]) {
            continue;
        }


        for (const auto& neighbor : graph[u]) {
            int v = neighbor.first;
            int weight = neighbor.second;

            int newMinWeight = min(currentMin, weight);

            if (newMinWeight > minWeight[v]) {
                minWeight[v] = newMinWeight;
                pq.push({newMinWeight, v});
            }
        }
    }

    return (minWeight[dest] == INT_MIN) ? -1 : minWeight[dest];
}

int main() {
    int n = 4;

    vector<vector<pii>> graph(n);

    // Arestas do grafo (u, v, peso)
    graph[0].push_back({1, 1});
    graph[0].push_back({2, 4});

    graph[1].push_back({2, 2});
    graph[1].push_back({3, 5});

    graph[2].push_back({3, 1});


    /*
    Visualização do Grafo
    
    A-----1----→B-----5-----|
    |           |           |
    |           |           ↓
    4           2           D
    |           |           ↑
    ↓           |           |
    C←----------|           |
    |                       |
    |------------1----------|


    */

    // Definindo o nó de origem e o nó de destino
    int src = 0, dest = 3;

    // Chamamos o Dijkstra clássico e modificado para o caminho maximin e minmax
    int resp = dijkstra(n, graph, src, dest);

    if (resp != -1) {
        cout << "O menor caminho de " << src << " para " << dest << " é: " << resp << endl;
    } else {
        cout << "Não há caminho de " << src << " para " << dest << endl;
    }

    resp = dijkstraMinimax(n, graph, src, dest);

    if (resp != -1) {
        cout << "O menor dos maiores pesos do caminho minimax de " << src << " para " << dest << " é: " << resp << endl;
    } else {
        cout << "Não há caminho de " << src << " para " << dest << endl;
    }

    resp = dijkstraMaximin(n, graph, src, dest);

    if (resp != -1) {
        cout << "O maior dos menores pesos do caminho maximin de " << src << " para " << dest << " é: " << resp << endl;
    } else {
        cout << "Não há caminho de " << src << " para " << dest << endl;
    }

    return 0;
}

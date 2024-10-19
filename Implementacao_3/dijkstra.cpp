#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <climits>

using namespace std;

// Definição do tipo para representar o grafo (lista de adjacência)
typedef pair<int, int> pii;

/* Simple dijkstra function through min-heap priority queue
 * Gets shortest path from defined graph structure, and also the path identified
 * 
 * Return:
 *   A pair consisting of:
 *     first: vector<int> with the desired path from src to dest
 *     second: int with the distance of said path 
 */
pair<vector<int>,int> dijkstra(vector<vector<pii>>& graph, int src, int dest) {
    
    // Store both parents and dist for each update
    vector<int> dist(graph.size(), INT_MAX);  
    dist[src] = 0;
    
    vector<int> parent(graph.size(), -1); 
    
    // Min-heap priority queue -> always ensure best distance possible once node is explored
    // Organized as: {weight: int, vertex: int}
    priority_queue<pii, vector<pii>, greater<pii>> pq;
    pq.push({0, src}); 
    
    while (!pq.empty()) {
        int current_dist = pq.top().first;
        int current_node = pq.top().second;
        pq.pop();
        
        // Exit if path to dest is already found (other distances are skipped)        
        if (current_node == dest) {
            break;
        }
        
        // Ignore other worse values for a given vertex, when more than one change happens to its dist
        // By the min-heap order, the first pair that contains vertex will always be the best one; and the 
        // others will be ignored through continue
        if (current_dist > dist[current_node]) {
            continue;
        }
        
        // Explore neighbors
        for (auto& neighbor : graph[current_node]) {
            int adjacent = neighbor.first;
            int weight = neighbor.second;
            int distance = current_dist + weight;
            
            // If a shorter path is found -> add pair to the queue
            if (distance < dist[adjacent]) {
                dist[adjacent] = distance;
                parent[adjacent] = current_node;  
                pq.push({distance, adjacent});
            }
        }
    }

    // Unreachable 
    if (dist[dest] == INT_MAX) {
        return {{}, -1};
    }
    
    // Get path through Backtrack
    vector<int> path;
    for (int at = dest; at != -1; at = parent[at]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());

    return {path,dist[dest]};
}


/* Dijkstra modification for minmax algorithm 
 * Gets path with lowest maximum weight   
 * 
 * Return:
 *   A pair consisting of:
 *     first: vector<int> with the desired path from src to dest
 *     second: int with the lowest maximum weight of said path 
 */
pair<vector<int>,int> dijkstraMinimax(vector<vector<pii>>& graph, int src, int dest) {

    // Store both parents and maxWeight for each update
    vector<int> maxWeight(graph.size(), INT_MAX);
    maxWeight[src] = 0;

    vector<int> parent(graph.size(), -1); 

    // Min-heap priority queue -> always ensure min weight is first explored
    // Organized as: {weight: int, vertex: int}
    priority_queue<pii, vector<pii>, greater<pii>> pq;
    pq.push({0, src});


    while (!pq.empty()) {
        int currentMax = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        // Exit if path to dest is already found (other distances are skipped)  
        if (u == dest) {
            break;
        }

        // Ignore other worse values for a given vertex, when more than one change happens to its weight
        // By the min-heap order, the first pair that contains vertex will always be the best one (min)
        // and the rest will be ignored through continue
        if (currentMax > maxWeight[u]) {
            continue;
        }


        // Explore neighbors
        for (const auto& neighbor : graph[u]) {
            int v = neighbor.first;
            int weight = neighbor.second;

            int newMaxWeight = max(currentMax, weight);

            // If lower maximum edge is found, add better pair to queue   
            if (newMaxWeight < maxWeight[v]) {
                maxWeight[v] = newMaxWeight;
                parent[v] = u;
                pq.push({newMaxWeight, v});
            }
        }
    }

    // Unreachable 
    if (maxWeight[dest] == INT_MAX) {
        return {{}, -1};
    }
    
    // Get path through Backtrack
    vector<int> path;
    for (int at = dest; at != -1; at = parent[at]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());

    return {path, maxWeight[dest]};
}

/* Dijkstra modification for maxmin  algorithm 
 * Gets path with maximum lowest weight   
 * 
 * Return:
 *   A pair consisting of:
 *     first: vector<int> with the desired path from src to dest
 *     second: int with the maximum lowest weight of said path 
 */
pair<vector<int>, int> dijkstraMaximin(vector<vector<pii>>& graph, int src, int dest) {

    // Store both parents and min weights for each update
    vector<int> minWeight(graph.size(), INT_MIN);
    minWeight[src] = INT_MAX;

    vector<int> parent(graph.size(), -1); 

    // Max-heap priority queue -> always ensure max weight possible node is first explored
    // Organized as: {weight: int, vertex: int}
    priority_queue<pii> pq;
    pq.push({INT_MAX, src});

    while (!pq.empty()) {
        int currentMin = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        // Exit if path to dest is already found (other distances are skipped)  
        if (u == dest) {
            break; 
        }

        // Ignore other worse values for a given vertex, when more than one change happens to its weight
        // By the max-heap order, the first pair that contains vertex will always be the best one (max)
        // and the rest will be ignored through continue
        if (currentMin < minWeight[u]) {
            continue;
        }

        // Explore neighbors
        for (const auto& neighbor : graph[u]) {
            int v = neighbor.first;
            int weight = neighbor.second;

            int newMinWeight = min(currentMin, weight);

            // If better max if found add better pair
            if (newMinWeight > minWeight[v]) {
                minWeight[v] = newMinWeight;
                parent[v] = u;
                pq.push({newMinWeight, v});
            }
        }
    }

    // Unreachable 
    if (minWeight[dest] == INT_MAX) {
        return {{}, -1};
    }
    
    // Get path through Backtrack
    vector<int> path;
    for (int at = dest; at != -1; at = parent[at]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());

    return {path, minWeight[dest]};
}


void dijkstra_test(vector<vector<pii>> graph, int src, int dest) {
    pair<vector<int>, int> resp = dijkstra(graph, src, dest);
    
    
    vector<int> path = resp.first;
    int dist = resp.second;

    if (!path.empty()) {
        cout << "Shortest path from " << src << " to " << dest << ": ";
        for (int node : path) {
            cout << node << " ";
        }
        cout << "\nTotal distance: " << dist << endl;
    } else {
        cout << "No path found from " << src << " to " << dest << endl;
    }

}

void dijsktra_minmax_test(vector<vector<pii>> graph, int src, int dest) {
    pair<vector<int>, int> resp = dijkstraMinimax(graph, src, dest);

    vector<int> path = resp.first;
    int dist = resp.second;

    if (!path.empty()) {
        cout << "MinMax from " << src << " to " << dest << ": ";
        for (int node : path) {
            cout << node << " ";
        }
        cout << "\nLowest max of path: " << dist << endl;
    } else {
        cout << "No path found from " << src << " to " << dest << endl;
    }

}

void dijsktra_maxmin_test(vector<vector<pii>> graph, int src, int dest) {
    pair<vector<int>, int> resp = dijkstraMaximin(graph, src, dest);


    vector<int> path = resp.first;
    int dist = resp.second;

    if (!path.empty()) {
        cout << "Maxmin from " << src << " to " << dest << ": ";
        for (int node : path) {
            cout << node << " ";
        }
        cout << "\nMax minimum of path: " << dist << endl;
    } else {
        cout << "No path found from " << src << " to " << dest << endl;
    }
}

vector<vector<pii>> buildGraph1() {
    vector<vector<pii>> graph(5);

    graph[0].push_back({1, 10});
    graph[0].push_back({3, 4});
    graph[0].push_back({4, 5});

    graph[1].push_back({4, 2});
    graph[1].push_back({2, 1});
    
    graph[2].push_back({3, 4});

    graph[3].push_back({4, 3});

    graph[4].push_back({2, 2});

    return graph;
}


vector<vector<pii>> buildGraph2() {
    vector<vector<pii>> graph(4);

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

    return graph;
}



int main() {
    // Graph vector. Each index corresponds to current vertex
    // Each vertex graph[0] contains an edge list for edges to neighbors
    // Each edge is a pair of:
    //   first: neighbor vertex
    //   second: weight 
    vector<vector<pii>> graph = buildGraph1();

    // Definindo o nó de origem e o nó de destino
    int src = 0, dest = 2;

    dijkstra_test(graph, src, dest);
    dijsktra_minmax_test(graph, src, dest);
    dijsktra_maxmin_test(graph, src, dest);
    

    return 0;
}

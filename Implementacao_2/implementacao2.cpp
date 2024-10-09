#include <iostream>
#include <vector>
#include <set>
#include <cmath>

using namespace std;

// Função para calcular o fatorial
int fatorial(int n) {
    if (n <= 1) return 1;
    return n * fatorial(n - 1);
}

// Função para calcular a fórmula de combinações de subgrafos
int calcula_subgrafos(int N) {
    int total_subgrafos = 0;
    for (int p = 1; p <= N; ++p) {
        int combinacoes = fatorial(N) / (fatorial(p) * fatorial(N - p));
        int arestas_possiveis = pow(2, (p * (p - 1)) / 2);
        total_subgrafos += combinacoes * arestas_possiveis;
    }
    return total_subgrafos;
}

// Função para exibir um grafo
void exibe_grafo(const vector<bool>& vertices, const vector<bool>& arestas, int N) {
    cout << "Vertices: ";
    for (int i = 0; i < N; ++i) {
        if (vertices[i]) cout << i << " ";
    }
    cout << "\nVetor de vertices: ";
    cout << "|";
    for (int i = 0; i < N; ++i) {
        cout << vertices[i] << " ";
        cout << "|";
    }

    cout << "\nArestas: ";
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            if (arestas[i * N + j]) {
                cout << "(" << i << "," << j << ") ";
            }
        }
    }
    
    cout << "\nVetor de arestas: ";
    cout << "|";
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << arestas[i * N + j];
        }
        cout << "|";
    }

    cout << "\n\n";
}

// Função para gerar todos os subgrafos não direcionados únicos
void gera_subgrafos(int N) {
    vector<bool> vertices(N, false);
    vector<bool> arestas(N * N, false);

    set<pair<vector<bool>, vector<bool>>> subgrafos_unicos;

    // Gerar todos os subgrafos possíveis
    int total_vertices = pow(2, N);
    int total_arestas = pow(2, N * (N - 1) / 2);

    for (int qtdeLocalV = 1; qtdeLocalV < total_vertices; ++qtdeLocalV) {
        for (int qtdeLocalA = 0; qtdeLocalA < total_arestas; ++qtdeLocalA) {
            // Configurar vértices
            for (int i = 0; i < N; ++i) {
                vertices[i] = (qtdeLocalV & (1 << i)) != 0;
            }

            // Configurar arestas
            fill(arestas.begin(), arestas.end(), false);
            int index = 0;
            for (int i = 0; i < N-1; ++i) {
                for (int j = i + 1; j < N; ++j) {
                    if ((qtdeLocalA & (1 << index)) != 0) {
                        arestas[i * N + j] = true;
                    }
                    index++;
                }
            }

            // Verificar se o subgrafo é válido (não possui arestas com vértices inexistentes)
            bool valido = true;
            for (int i = 0; i < N; ++i) {
                for (int j = i + 1; j < N; ++j) {
                    if (arestas[i * N + j] && (!vertices[i] || !vertices[j])) {
                        valido = false;
                        break;
                    }
                }
                if (!valido) break;
            }

            // Se for válido, adicionar ao conjunto de subgrafos únicos
            if (valido) {
                subgrafos_unicos.insert({vertices, arestas});
            }
        }
    }

    // Exibir todos os subgrafos únicos
    int contagem = 0;
    for (const auto& subgrafo : subgrafos_unicos) {
        exibe_grafo(subgrafo.first, subgrafo.second, N);
        contagem++;
    }

    // Exibir a contagem de subgrafos
    cout << "Numero total de subgrafos unicos: " << contagem << endl;

    // Comparar com o cálculo teórico
    int subgrafos_teoricos = calcula_subgrafos(N);
    cout << "Numero teorico de subgrafos: " << subgrafos_teoricos << endl;

    if (contagem == subgrafos_teoricos) {
        cout << "A contagem corresponde ao calculo teorico!" << endl;
    } else {
        cout << "A contagem NAO corresponde ao calculo teorico!" << endl;
    }
}

int main() {
    int N;
    cout << "Digite o numero de vertices (N): ";
    cin >> N;

    gera_subgrafos(N);

    return 0;
}

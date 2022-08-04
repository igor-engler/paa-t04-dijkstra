#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <list>

#define INFINITE 1000000
#define TESTS 11

//comando para compilar: g++ main.cpp -o main.o
//comando para executar: ./main.o

struct graph_data {
    int vertex;
    int distance;
};

void printMatrix(std::vector < std::vector < int >> & graph) {
    for (int i = 0; i < graph.size(); i++) {
        for (int j = 0; j < graph[i].size(); j++) {
            std::cout << graph[i][j] << " ";
        }
        std::cout << "\n";
    }
}

void printList(std::vector < std::vector < graph_data >> & AdjList) {
    for (int i = 0; i < AdjList.size(); i++) {
        std::cout << i;
        for (int j = 0; j < AdjList[i].size(); j++) {
            if (j == AdjList[i].size() - 1) {
                std::cout << " -> " << AdjList[i][j].vertex << "(" << AdjList[i][j].distance << ")" << std::endl;
                break;
            } else
                std::cout << " -> " << AdjList[i][j].vertex << "(" << AdjList[i][j].distance << ")";
        }
    }
}

std::vector < std::vector < graph_data > > convert(std::vector < std::vector < int > > & graph) {
    std::vector < std::vector < graph_data >> adjList(graph.size());

    for (int i = 0; i < graph.size(); i++) {
        graph_data data;
        for (int j = 0; j < graph[i].size(); j++) {
            if (graph[i][j] != 0) {
                data.distance = graph[i][j];
                data.vertex = j;
                adjList[i].push_back(data);
            }
        }
    }
    return adjList;
}

int minDistance(int V, int * distances, bool * visited, long & count) {
    int min = INFINITE;
    int min_index;

    for (int v = 0; v < V; v++) {
        count++;
        if (visited[v] == false && distances[v] <= min) {
            min = distances[v];
            min_index = v;
        }
    }
    return min_index;
}

void printSolution(int V, int * distances) {
    std::cout << "Vertex \t Distance from Source" << std::endl;
    for (int i = 0; i < V; i++)
        std::cout << i << " \t\t" << distances[i] << std::endl;
}

void djikstraList(int V, std::vector < std::vector < graph_data > > & adjlist, long & list_count) {
    int distances[V];
    bool visited[V];

    for (int i = 0; i < V; i++) {
        distances[i] = INFINITE;
        visited[i] = false;
    }
    distances[0] = 0;

    for (int count = 0; count < V; count++) {
        int u = minDistance(V, distances, visited, list_count);

        visited[u] = true;

        for (int v = 0; v < adjlist[u].size(); v++) {
            list_count++;
            int w = adjlist[u][v].vertex;
            int dist = distances[u] + adjlist[u][v].distance;
            if (dist <= distances[w]) {
                distances[w] = dist;
            }
        }
    }
    //printSolution(V, distances);
}

void dijkstraMatrix(int V, std::vector < std::vector < int > > & graph, long & matrix_count) {
    int distances[V];
    bool visited[V];

    for (int i = 0; i < V; i++) {
        distances[i] = INFINITE;
        visited[i] = false;
    }

    distances[0] = 0;
    for (int count = 0; count < V - 1; count++) {
        int u = minDistance(V, distances, visited, matrix_count);

        visited[u] = true;
        for (int v = 0; v < V; v++){
            matrix_count++;
            if (!visited[v] && graph[u][v] && distances[u] != INFINITE &&
                distances[u] + graph[u][v] < distances[v])
                distances[v] = distances[u] + graph[u][v];
        }
    }
    //printSolution(V, distances);
}

int main() {

    std::list<std::string> files({"10", "25", "50", "75", "100", "150", "200", "250", "300", "400", "500", "650", "800", "1000", "1500"});

    for (std::string val: files) {
        int V = 0;

        std::ofstream ofs_output("saidas/" + val + ".csv");
        ofs_output << "SIZE ; List time (ns) ; List count ; Matrix time (ns) ; Matrix count" << std::endl;
        ofs_output << std::fixed;
        
        std::ifstream inputfile("entradas/Entrada " + val + ".txt");
        if (!inputfile) {
            std::cout << "could not open file" << std::endl;
            return -1;
        }
        inputfile >> V;

        //read_data(inputfile, graph, V);
        std::vector < std::vector < int >> graph(V, std::vector < int > (V, 0));
        int data = 0;
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                inputfile >> data;
                graph[i][j] = data;
            }
        }
        inputfile.close();

        std::vector < std::vector < graph_data > > adjlist = convert(graph);

        //printMatrix(graph);
        //printList(adjlist);

        using clock = std::chrono::high_resolution_clock;

        for(int tests = 0; tests < TESTS; tests++){
            long list_count = 0;
            auto list_t1 = clock::now();
            djikstraList(V, adjlist, list_count);
            auto list_t2 = clock::now();
            std::chrono::duration < double, std::nano > list_span = list_t2 - list_t1;

            long matrix_count = 0;
            auto matrix_t1 = clock::now();
            dijkstraMatrix(V, graph, matrix_count);
            auto matrix_t2 = clock::now();
            std::chrono::duration < double, std::nano > matrix_span = matrix_t2 - matrix_t1;

            ofs_output   << val
                << " ; " << list_span.count()
                << " ; " << list_count
                << " ; " << matrix_span.count()
                << " ; " << matrix_count
                << std::endl;
        }
    }

    return 0;
}
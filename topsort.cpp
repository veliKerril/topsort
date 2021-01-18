//----------------------------------------------------
#include <bits/stdc++.h>
//----------------------------------------------------
using namespace std;
//----------------------------------------------------
const int TEST_SIZE = 300;
const int MIN_V = 5;
const int MAX_V = 100;
const int STEP = 5;
//----------------------------------------------------

bool find_cycle(int v, const vector<vector<int>> &g, vector<int> &visited, int branch) {
    visited[v] = branch;
    bool if_cycle = false;
    for (const int &e: g[v])
        if (visited[e] == branch)
            if_cycle = true;
        else
            if_cycle |= find_cycle(e, g, visited, branch);
    return if_cycle;
}

bool acyclic(const vector<vector<int>> &g) {
    vector<int> visited(g.size(), -1);
    int branch = 0;
    for (int v = 0; v < g.size(); v++)
        if (visited[v] == -1 && find_cycle(v, g, visited, branch++))
            return false;
    return true;
}

vector<vector<int>> generate_graph(int n) {
    vector<vector<int>> graph(n, vector<int>());

    //Инициализация рандома
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(int(pow(n,0.5)), n * (n - 1) / 2);

    int m = dist(gen);

    //Генерация М ребер
    vector<int> range(n * n);
    for (int i = 0; i < n * n; i++)
        range[i] = i;
    shuffle(range.begin(), range.end(), gen);
    range = vector<int>(range.begin(), range.begin() + m);

    for (const int element : range) {
        int u = element / n, v = element % n;
        if (u != v)
            graph[u].push_back(v);
    }

    //Если граф имеет циклы, то создаём другой граф
    if (!acyclic(graph))
        graph = generate_graph(n);

    return graph;
}

//----------------------------------------------------

void dfs(int v, const vector<vector<int>> &g, vector<bool> &visited, vector<int> &vertices) {
    visited[v] = true;
    for (const int &e: g[v])
        if (!visited[e])
            dfs(e, g, visited, vertices);
    vertices.push_back(v);
}

vector<int> topological_sort(const vector<vector<int>> &g) {
    vector<int> vertices = {};
    vector<bool> visited(g.size(), false);
    for (int v = 0; v < g.size(); v++)
        if (!visited[v])
            dfs(v, g, visited, vertices);
    reverse(vertices.begin(), vertices.end());
    return vertices;
}

//----------------------------------------------------

int main() {

    vector<vector<long double>> test_ratios((MAX_V - MIN_V + STEP) / STEP, vector<long double>(TEST_SIZE));
    vector<vector<int>> test_durations((MAX_V - MIN_V + STEP) / STEP, vector<int>(TEST_SIZE));

    for (int VS = MIN_V; VS <= MAX_V; VS += STEP) {


        for (int test = 0; test < TEST_SIZE; test++) {
            //Генерация тестового случая
            const vector<vector<int>> graph = generate_graph(VS);

            //Топологическая сортировка
            auto start_time = std::chrono::high_resolution_clock::now();
            const vector<int> vertices = topological_sort(graph);
            auto end_time = std::chrono::high_resolution_clock::now();


            int m = 0, n = graph.size();
            for (const vector<int> &v: graph)
                m += v.size();

            //Записываем длительность теста
            test_durations[(VS - MIN_V) / STEP][test] = std::chrono::duration_cast<std::chrono::microseconds>(
                    end_time - start_time).count();

            //Записываем отношение T(n+m)/(n+m)
            test_ratios[(VS - MIN_V) / STEP][test] =
                    (long double) (test_durations[(VS - MIN_V) / STEP][test]) / (long double) (n + m);
        }
    }

    ofstream data;
    data.open("data.out");
    data << setprecision(10);
    for (int i = 0; i < (MAX_V - MIN_V + STEP) / STEP; i++) {
        data << (i + 1) * STEP << ", ";

        sort(test_durations[i].begin(), test_durations[i].end());
        data << test_durations[i][TEST_SIZE/2] << ", ";

        for (int j = 0; j < TEST_SIZE; j++)
            data << test_ratios[i][j] << ", ";
        data << "\n";
    }

}


#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>

#define NUMBER_OF_TRIES 100

using namespace std;
using namespace std::chrono;

class Undirected
{
private:
    vector<vector<int>> graph;
    int numEdges;
    void findEulerCycleP(int currIndex, vector<int> &path)
    {
        for (int next = 0; next < graph.size(); next++)
        {
            if (graph[currIndex][next] == 1)
            {
                graph[currIndex][next] = 0;
                graph[next][currIndex] = 0;
                findEulerCycleP(next, path);
            }
        }
        path.push_back(currIndex + 1);
    }

public:
    int size;
    void generate(int numVertices, int saturation)
    {
        graph.clear();
        numEdges = (saturation * (numVertices * (numVertices - 1)) / 2) / 100;
        graph.resize(numVertices, vector<int>(numVertices, 0));
        srand((unsigned)time(nullptr));
        while (numEdges)
        {
            int a = rand() % numVertices;
            int b = rand() % numVertices;
            if (a != b && graph[a][b] == 0)
            {
                graph[a][b] = 1;
                graph[b][a] = 1;
                numEdges--;
            }
        }
        size = numVertices;
    }
    void loadFromFile(string name = "input.txt")
    {
        graph.clear();
        ifstream file(name);
        if (!file.is_open())
        {
            cerr << "[ERR] File not found." << endl;
            return;
        }
        int numVertices;
        file >> numVertices >> numEdges;
        graph.resize(numVertices, vector<int>(numVertices, 0));
        for (int n = 0; n < numEdges; n++)
        {
            int a, b;
            file >> a >> b;
            a--;
            b--;
            graph[a][b] = 1;
            graph[b][a] = 1;
        }
        file.close();
        size = numVertices;
    }
    void print()
    {
        for (int n = 0; n < size; n++)
        {
            for (int m = 0; m < size; m++)
            {
                cout << graph[n][m] << " ";
            }
            cout << endl;
        }
    }
    bool findHamiltonianCycle(int currIndex, int startIndex, vector<bool> &visited, vector<int> &path)
    {
        visited[currIndex] = true;
        path.push_back(currIndex + 1);
        if (path.size() == graph.size())
        {
            if (graph[currIndex][startIndex] == 1)
            {
                path.push_back(startIndex + 1);
                return true;
            }
            path.pop_back();
            visited[currIndex] = false;
            return false;
        }
        for (int next = 0; next < graph.size(); next++)
            if (graph[currIndex][next] == 1 && !visited[next])
                if (findHamiltonianCycle(next, startIndex, visited, path))
                    return true;
        path.pop_back();
        visited[currIndex] = false;
        return false;
    }
    bool findEulerCycle(vector<int> &path)
    {
        for (vector<int> n : graph)
        {
            int sum = 0;
            for (int m : n)
            {
                if (m == 1)
                    sum++;
            }
            if (sum % 2 == 1)
                return false;
        }
        findEulerCycleP(0, path);
        return true;
    }
};

class Directed
{
private:
    vector<vector<int>> successors;
    int numEdges;
    bool isSuccessor(int index, int successor)
    {
        for (int n = 0; n < successors[index].size(); n++)
            if (successors[index][n] == successor)
                return true;
        return false;
    }
    void findEulerCycleP(int currIndex, vector<int> &path)
    {
        while (!successors[currIndex].empty())
        {
            int next = successors[currIndex].back();
            successors[currIndex].pop_back();
            findEulerCycleP(next, path);
        }
        path.push_back(currIndex + 1);
    }

public:
    int size;
    void generate(int numVertices, int saturation)
    {
        successors.clear();
        numEdges = (saturation * (numVertices * (numVertices - 1))) / 100;
        successors.resize(numVertices);
        srand((unsigned)time(nullptr));
        while (numEdges)
        {
            int a = rand() % numVertices;
            int b = rand() % numVertices;
            if (a != b && !isSuccessor(a, b))
            {
                successors[a].push_back(b);
                numEdges--;
            }
        }
        size = numVertices;
    }
    void loadFromFile(string name = "input.txt")
    {
        successors.clear();
        ifstream file(name);
        if (!file.is_open())
        {
            cerr << "[ERR] File not found." << endl;
            return;
        }
        int numVertices;
        file >> numVertices >> numEdges;
        successors.resize(numVertices);
        for (int n = 0; n < numEdges; n++)
        {
            int a, b;
            file >> a >> b;
            a--;
            b--;
            successors[a].push_back(b);
        }
        file.close();
        size = numVertices;
    }
    void print()
    {
        for (int n = 0; n < size; n++)
        {
            cout << n + 1 << ": ";
            for (int m = 0; m < successors[n].size(); m++)
            {
                cout << successors[n][m] + 1 << " ";
            }
            cout << endl;
        }
    }
    bool findHamiltonianCycle(int currIndex, int startIndex, vector<bool> &visited, vector<int> &path)
    {
        visited[currIndex] = true;
        path.push_back(currIndex + 1);
        if (path.size() == size)
        {
            if (isSuccessor(currIndex, startIndex))
            {
                path.push_back(startIndex + 1);
                return true;
            }
            path.pop_back();
            visited[currIndex] = false;
            return false;
        }
        for (int next : successors[currIndex])
            if (!visited[next])
                if (findHamiltonianCycle(next, startIndex, visited, path))
                    return true;
        path.pop_back();
        visited[currIndex] = false;
        return false;
    }
    bool findEulerCycle(vector<int> &path)
    {
        int in[size] = {0}, out[size] = {0};
        for (int n = 0; n < size; n++)
        {
            for (int m = 0; m < successors[n].size(); m++)
            {
                in[successors[n][m]]++;
                out[n]++;
            }
        }
        for (int n = 0; n < size; n++)
        {
            if (in[n] != out[n])
                return false;
        }
        findEulerCycleP(0, path);
        return true;
    }
};

void printPath(vector<int> &path)
{
    for (int n : path)
        cout << n << " ";
    cout << endl;
}

void test()
{
    ofstream outputFile("output.txt");
    if (!outputFile.is_open())
    {
        cerr << "[ERR] Can't open file." << endl;
        return;
    }
    vector<int> saturationList = {10, 20, 30, 40, 50, 60, 70, 80, 90};
    vector<int> nList = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    for (int s : saturationList)
    {
        for (int n : nList)
        {
            Undirected first;
            first.generate(n, s);
            vector<bool> visited(first.size, false);
            vector<int> path;
            int tries = NUMBER_OF_TRIES;
            auto start = high_resolution_clock::now();
            while (!first.findHamiltonianCycle(0, 0, visited, path) && tries)
            {
                tries--;
                start = high_resolution_clock::now();
            }
            auto end = high_resolution_clock::now();
            auto firstHamilton = duration_cast<microseconds>(end - start);

            path.clear();
            tries = NUMBER_OF_TRIES;
            start = high_resolution_clock::now();
            while (!first.findEulerCycle(path) && tries)
            {
                tries--;
                start = high_resolution_clock::now();
            }
            end = high_resolution_clock::now();
            auto firstEuler = duration_cast<microseconds>(end - start);

            Directed second;
            second.generate(n, s);
            visited.assign(second.size, false);
            path.clear();
            tries = NUMBER_OF_TRIES;
            start = high_resolution_clock::now();
            while (!second.findHamiltonianCycle(0, 0, visited, path) && tries)
            {
                tries--;
                start = high_resolution_clock::now();
            }
            end = high_resolution_clock::now();
            auto secondHamilton = duration_cast<microseconds>(end - start);

            path.clear();
            tries = NUMBER_OF_TRIES;
            start = high_resolution_clock::now();
            while (!second.findEulerCycle(path) && tries)
            {
                tries--;
                start = high_resolution_clock::now();
            }
            end = high_resolution_clock::now();
            auto secondEuler = duration_cast<microseconds>(end - start);

            // Save the results to the output file
            outputFile << "###" << s << "-" << n << "###" << endl;
            outputFile << "UN Ham: "
                       << "(" << n << "," << firstHamilton.count() << ")" << endl;
            outputFile << "UN Eul: "
                       << "(" << n << "," << firstEuler.count() << ")" << endl;
            outputFile << "D  Ham: "
                       << "(" << n << "," << secondHamilton.count() << ")" << endl;
            outputFile << "D  Eul: "
                       << "(" << n << "," << secondEuler.count() << ")" << endl;
        }
    }
    outputFile.close();
}
int main()
{
    // test();

    // Undirected m;
    // m.generate(6, 50);
    // m.print();
    // m.loadFromFile("testHND.txt");
    // m.print();
    // vector<int> path;
    // vector<bool> visited(m.size, false);
    // if (!m.findHamiltonianCycle(0, 0, visited, path))
    //     cout << "Graf wejściowy nie zawiera cyklu" << endl;
    // else
    //     printPath(path);
    // path.clear();
    // m.loadFromFile("testEND.txt");
    // m.print();
    // if (!m.findEulerCycle(path))
    //     cout << "Graf wejściowy nie zawiera cyklu" << endl;
    // else
    //     printPath(path);

    // Directed n;
    // n.generate(6, 50);
    // n.print();
    // n.loadFromFile("testHD.txt");
    // n.print();
    // vector<int> path;
    // vector<bool> visited(n.size, false);
    // if (!n.findHamiltonianCycle(0, 0, visited, path))
    //     cout << "Graf wejściowy nie zawiera cyklu" << endl;
    // else
    //     printPath(path);
    // path.clear();
    // n.loadFromFile("testED.txt");
    // n.print();
    // if (!n.findEulerCycle(path))
    //     cout << "Graf wejściowy nie zawiera cyklu" << endl;
    // else
    //     printPath(path);

    return 0;
}
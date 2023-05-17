#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <random>
#include <vector>

using namespace std;

class msasiedztwa
{
private:
    int **macierz, msize = 0;
    int createMatrix(int size, int filler)
    {
        msize = size;
        macierz = new int *[size];
        for (int n = 0; n < size; n++)
        {
            macierz[n] = new int[size];
            for (int m = 0; m < size; m++)
            {
                macierz[n][m] = filler;
                if (n == m)
                    macierz[n][m] = 0;
            }
        }
        return filler ? 0 : 1;
    }

public:
    msasiedztwa(){};
    ~msasiedztwa()
    {
        for (int n = 0; n < msize; n++)
        {
            delete[] macierz[n];
        }
        delete[] macierz;
    };
    void loadFromFile(string name = "input.txt")
    {
        ifstream file(name);
        if (file.is_open())
        {
            int a, b, validate;
            file >> a >> validate;
            createMatrix(a, 0);
            while (file >> a >> b)
            {
                a--;
                b--;
                macierz[a][b] = 1;
                macierz[b][a] = 1;
                validate--;
            }
            if (validate != 0)
            {
                cout << "[WARN] Incorrect graph." << endl;
            }
        }
        else
        {
            cout << "[ERR] File not found." << endl;
        }
    }
    void generateMatrix(int size, int saturation)
    {
        int edges = (((size * (size - 1)) / 2) * (saturation > 50 ? 100 - saturation : saturation)) / 100;
        int filler = createMatrix(size, saturation > 50);
        srand((unsigned)time(NULL));
        while (edges > 0)
        {
            int x = rand() % size;
            int y = rand() % size;
            if (x != y && macierz[y][x] != filler)
            {
                macierz[x][y] = filler;
                macierz[y][x] = filler;
                edges--;
            }
        }
    }
    void print()
    {
        if (!msize)
        {
            cout << "[WARN] Create matrix first, use generateMatrix(size) or loadFromFile(name)." << endl;
            return;
        }
        cout << setw(3) << "";
        for (int n = 1; n <= msize; n++)
        {
            cout << setw(3) << n;
        }
        cout << endl;
        for (int n = 0; n < msize; n++)
        {
            cout << setw(3) << n + 1;
            for (int m = 0; m < msize; m++)
            {
                cout << setw(3) << macierz[n][m];
            }
            cout << endl;
        }
    }
};
class listnastep
{
private:
    vector<vector<int>> lista;
    void createList(int size)
    {
        for (int n = 0; n < size; n++)
        {
            lista.push_back(vector<int>());
        }
    }
    bool hasValue(int index, int value)
    {
        for (int n = 0; n < lista[index].size(); n++)
            if (lista[index][n] == value)
                return true;
        return false;
    }

public:
    listnastep(){};
    ~listnastep()
    {
        for (int n = 0; n < lista.size(); n++)
        {
            lista[n].clear();
        }
        lista.clear();
    };
    void loadFromFile(string name = "input.txt")
    {
        ifstream file(name);
        if (file.is_open())
        {
            int a, b, validate;
            file >> a >> validate;
            createList(a);
            while (file >> a >> b)
            {
                a--;
                b--;
                lista[a].push_back(b);
                validate--;
            }
            if (validate != 0)
            {
                cout << "[WARN] Incorrect graph." << endl;
            }
        }
        else
        {
            cout << "[ERR] File not found." << endl;
        }
    }
    void generateList(int size, int saturation)
    {
        int edges = (size * size * saturation) / 100;
        createList(size);
        srand((unsigned)time(NULL));
        while (edges)
        {
            int x = rand() % size;
            int y = rand() % size;
            if (!hasValue(x, y))
            {
                lista[x].push_back(y);
                edges--;
            }
        }
    }
    void print()
    {
        for (int n = 0; n < lista.size(); n++)
        {
            cout << n + 1 << ": ";
            for (int m = 0; m < lista[n].size(); m++)
            {
                cout << setw(3) << lista[n][m] + 1;
            }
            cout << endl;
        }
    }
};

int main()
{
    // listnastep m;
    // m.loadFromFile("test.txt");
    // m.print();
    msasiedztwa m;
    m.generateMatrix(5, 60);
    m.print();
    return 0;
}
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;

class msasiedztwa
{
private:
    int **macierz, msize = 0;
    void createMatrix(int size)
    {
        msize = size;
        macierz = new int *[size];
        for (int n = 0; n < size; n++)
        {
            macierz[n] = new int[size];
            for (int m = 0; m < size; m++)
            {
                macierz[n][m] = 0;
            }
        }
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
            createMatrix(a);
            while (file >> a >> b)
            {
                a--;
                b--;
                macierz[a][b] = 1;
                macierz[b][a] = -1;
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
        createMatrix(size);
        // TODO generate matrix based on saturation
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

// TODO implement class child list

int main()
{
    return 0;
}
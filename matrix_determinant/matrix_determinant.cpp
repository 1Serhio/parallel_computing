#include <iostream>
#include <vector>
#include <thread>

using namespace std;

//функция для вычисления минора матрицы
vector<vector<int>> getMinor(const vector<vector<int>>& matrix, int row, int col) {
    int n = matrix.size();
    vector<vector<int>> minor(n - 1, vector<int>(n - 1));
    for (int i = 0, mi = 0; i < n; ++i) {
        if (i == row) continue;
        for (int j = 0, mj = 0; j < n; ++j) {
            if (j == col) continue;
            minor[mi][mj] = matrix[i][j];
            ++mj;
        }
        ++mi;
    }
    return minor;
}

//рекурсивное вычисление определителя (разложение по первой строке матрицы)
int determinant(const vector<vector<int>>& matrix) {
    int n = matrix.size();
    if (n == 1) return matrix[0][0];
    if (n == 2) return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];

    int det = 0;
    vector<thread> threads;
    vector<int> results(n, 0);

    for (int j = 0; j < n; ++j) {
        threads.emplace_back([&, j]() {
            vector<vector<int>> minor = getMinor(matrix, 0, j);
            results[j] = ((j % 2 == 0) ? 1 : -1) * matrix[0][j] * determinant(minor);
            });
    }

    for (auto& th : threads) {
        th.join();
    }

    for (int j = 0; j < n; ++j) {
        det += results[j];
    }

    return det;
}

int main() {
    setlocale(LC_ALL, "");
    int n;
    do {
        cout << "Введите размерность матрицы (от 1 до 10): ";
        cin >> n;
    } while (n < 1 || n > 10);

    vector<vector<int>> matrix(n, vector<int>(n));
    cout << "Введите элементы матрицы построчно:" << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> matrix[i][j];
        }
    }

    cout << "Определитель матрицы: " << determinant(matrix) << endl;
    return 0;
}
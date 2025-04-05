#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <omp.h>
#include <iomanip>

using namespace std;
using namespace chrono;

// Генерация случайной матрицы
vector<vector<double>> generate_matrix(int rows, int cols) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 2.0);

    vector<vector<double>> matrix(rows, vector<double>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = dis(gen);
        }
    }
    return matrix;
}

// Однопоточное умножение матриц
vector<vector<double>> matrix_multiply_sequential(
    const vector<vector<double>>& A,
    const vector<vector<double>>& B) {

    int rows_A = A.size();
    int cols_A = A[0].size();
    int cols_B = B[0].size();

    vector<vector<double>> C(rows_A, vector<double>(cols_B, 0.0));

    for (int i = 0; i < rows_A; ++i) {
        for (int j = 0; j < cols_B; ++j) {
            double sum = 0.0;
            for (int k = 0; k < cols_A; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }

    return C;
}

// Параллельное умножение матриц с распределением по строкам
vector<vector<double>> matrix_multiply_parallel(
    const vector<vector<double>>& A,
    const vector<vector<double>>& B) {

    int rows_A = A.size();
    int cols_A = A[0].size();
    int cols_B = B[0].size();

    vector<vector<double>> C(rows_A, vector<double>(cols_B, 0.0));

#pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < rows_A; ++i) {
        for (int j = 0; j < cols_B; ++j) {
            double sum = 0.0;
            for (int k = 0; k < cols_A; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }

    return C;
}

// Функция для вывода матрицы
void print_matrix(const vector<vector<double>>& matrix, int max_rows = 5, int max_cols = 5) {
    cout << fixed << setprecision(4);
    int rows = min((int)matrix.size(), max_rows);
    int cols = (rows > 0) ? min((int)matrix[0].size(), max_cols) : 0;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << setw(8) << matrix[i][j] << " ";
        }
        if (cols < matrix[0].size()) cout << "...";
        cout << endl;
    }
    if (rows < matrix.size()) cout << "... (показано " << rows << " из " << matrix.size() << " строк)" << endl;
    cout << endl;
}


int main() {
    setlocale(LC_ALL, "");
    const int N = 100; // Размер матрицы N x N

    // Генерация матриц
    auto A = generate_matrix(N, N);
    auto B = generate_matrix(N, N);

    // Вывод исходных матриц
    cout << "Матрица A (" << N << "x" << N << "):" << endl;
    print_matrix(A);

    cout << "Матрица B (" << N << "x" << N << "):" << endl;
    print_matrix(B);

    // Замер времени для последовательной версии
    auto start_seq = high_resolution_clock::now();
    auto C_seq = matrix_multiply_sequential(A, B);
    auto end_seq = high_resolution_clock::now();
    duration<double> elapsed_seq = end_seq - start_seq;

    // Замер времени для параллельной версии
    auto start_par = high_resolution_clock::now();
    auto C_par = matrix_multiply_parallel(A, B);
    auto end_par = high_resolution_clock::now();
    duration<double> elapsed_par = end_par - start_par;

    // Вывод результатов умножения
    cout << "Результат умножения (последовательная версия):" << endl;
    print_matrix(C_seq);

    cout << "Результат умножения (параллельная версия):" << endl;
    print_matrix(C_par);

    // Проверка корректности
    bool correct = true;
    for (int i = 0; i < 5 && correct; ++i) {
        for (int j = 0; j < 5 && correct; ++j) {
            if (abs(C_seq[i][j] - C_par[i][j]) > 1e-6) {
                correct = false;
            }
        }
    }

    // Вывод результатов
    cout << "Размер матрицы: " << N << " x " << N << endl;
    cout << "Время однопоточной версии : " << elapsed_seq.count() << " секунд" << endl;
    cout << "Время многопоточной версии: " << elapsed_par.count() << " секунд" << endl;
    cout << "Ускорение: " << elapsed_seq.count() / elapsed_par.count() << endl;
    cout << "Результат " << (correct ? "корректен" : "некорректен") << endl;

    return 0;
}
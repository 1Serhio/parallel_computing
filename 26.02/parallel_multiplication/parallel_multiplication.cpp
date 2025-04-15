#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>

void fill_matrix(std::vector<std::vector<double>>& matrix) {
    for (auto& row : matrix)
        for (auto& elem : row)
            elem = static_cast<double>(rand()) / RAND_MAX;
}

void fill_vector(std::vector<double>& vec) {
    for (auto& elem : vec)
        elem = static_cast<double>(rand()) / RAND_MAX;
}

int main() {
    setlocale(LC_ALL, "");
    const int N = 1000; // количество строк
    const int M = 1000; // количество столбцов

    srand(time(nullptr));

    // Инициализация матрицы и вектора
    std::vector<std::vector<double>> matrix(N, std::vector<double>(M));
    std::vector<double> vector(M);
    std::vector<double> result_seq(N, 0.0);

    fill_matrix(matrix);
    fill_vector(vector);

    // Последовательное умножение
    double start_time = omp_get_wtime();
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            result_seq[i] += matrix[i][j] * vector[j];
        }
    }
    double seq_time = omp_get_wtime() - start_time;

    std::cout << "Последовательное время: " << seq_time << " сек\n";

    // Тестирование параллельной версии с разным числом потоков
    for (int threads : {2, 4, 8}) {
        std::vector<double> result_par(N, 0.0);
        omp_set_num_threads(threads);

        double start = omp_get_wtime();
#pragma omp parallel for
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < M; ++j) {
                result_par[i] += matrix[i][j] * vector[j];
            }
        }
        double time = omp_get_wtime() - start;

        // Проверка корректности
        bool correct = true;
        for (int i = 0; i < N; ++i) {
            if (std::abs(result_seq[i] - result_par[i]) > 1e-6) {
                correct = false;
                break;
            }
        }

        std::cout << "Параллельное время (" << threads << " потока(ов)): " << time << " сек";
        std::cout << ", Результаты совпадают: " << (correct ? "да" : "нет") << "\n";
    }

    return 0;
}

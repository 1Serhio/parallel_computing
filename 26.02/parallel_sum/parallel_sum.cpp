#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>

int main() {
    setlocale(LC_ALL, "Russian");

    const int N = 10000000; // Размер массива 10^7
    std::vector<int> arr(N);

    // Инициализация генератора случайных чисел
    std::srand(std::time(nullptr));

    // Заполняем массив случайными числами
    for (int i = 0; i < N; ++i) {
        arr[i] = std::rand() % 100; // Числа от 0 до 99
    }

    // Последовательное суммирование
    double start_time = omp_get_wtime();
    long long sequential_sum = 0;
    for (int i = 0; i < N; ++i) {
        sequential_sum += arr[i];
    }
    double sequential_time = omp_get_wtime() - start_time;

    // Параллельное суммирование с OpenMP
    start_time = omp_get_wtime();
    long long parallel_sum = 0;
#pragma omp parallel for reduction(+:parallel_sum)
    for (int i = 0; i < N; ++i) {
        parallel_sum += arr[i];
    }
    double parallel_time = omp_get_wtime() - start_time;

    // Вывод результатов
    std::cout << "Последовательное суммирование: " << sequential_sum << ", Время: " << sequential_time << " сек\n";
    std::cout << "Параллельное суммирование: " << parallel_sum << ", Время: " << parallel_time << " сек\n";

    // Проверка корректности
    if (sequential_sum == parallel_sum) {
        std::cout << "Результаты совпадают.\n";
    }
    else {
        std::cout << "ОШИБКА: Результаты различаются!\n";
    }

    return 0;
}
#include <iostream>
#include <vector>
#include <omp.h>
#include <cstdlib>  // Для rand() и srand()
#include <ctime>    // Для time()

void parallelOddEvenSort(std::vector<int>& arr) {
    int n = arr.size();

    for (int i = 0; i < n; ++i) {
        // Определяем, четная ли итерация
        int phase = i % 2;

        // Параллельный проход по массиву
#pragma omp parallel for default(none) shared(arr, n, phase)
        for (int j = phase; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
        // Все потоки синхронизируются автоматически после завершения параллельного for
    }
}

int main() {
    setlocale(LC_ALL, "");
    const int size = 50;
    std::vector<int> data(size);

    // Инициализация случайными числами
    srand(static_cast<unsigned>(time(0)));
    for (int& num : data) {
        num = rand() % 100;
    }

    std::cout << "До сортировки:\n";
    for (int num : data) std::cout << num << " ";
    std::cout << "\n";

    parallelOddEvenSort(data);

    std::cout << "После сортировки:\n";
    for (int num : data) std::cout << num << " ";
    std::cout << "\n";

    return 0;
}

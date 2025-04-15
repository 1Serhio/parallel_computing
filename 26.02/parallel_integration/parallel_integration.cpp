#include <iostream>
#include <cmath>
#include <omp.h>

const double PI = 3.141592653589793;
using namespace std;

double f(double x) {
    return cos(x);
}

int main() {
    setlocale(LC_ALL, "");
    const int N = 10000000; // Количество разбиений
    double a = 0.0, b = PI;
    double h = (b - a) / N;

    // Последовательное интегрирование
    double start_time = omp_get_wtime();
    double sequential_integral = 0.0;
    for (int i = 0; i < N; ++i) {
        double x = a + (i + 0.5) * h;
        sequential_integral += f(x) * h;
    }
    double sequential_time = omp_get_wtime() - start_time;

    // Параллельное интегрирование
    start_time = omp_get_wtime();
    double parallel_integral = 0.0;
#pragma omp parallel for reduction(+:parallel_integral)
    for (int i = 0; i < N; ++i) {
        double x = a + (i + 0.5) * h;
        parallel_integral += f(x) * h;
    }
    double parallel_time = omp_get_wtime() - start_time;

    // Аналитическое решение
    double exact_integral = sin(b) - sin(a); // Для cos(x) на [0, pi] это sin(pi) - sin(0) = 0

    // Вывод результатов
    cout << "Интегрировании cos(x) на промежутке [0, Pi]\n";
    cout << "Последовательное интегрирование: " << sequential_integral << ", Время: " << sequential_time << " сек\n";
    cout << "Параллельное интегрирование: " << parallel_integral << ", Время: " << parallel_time << " сек\n";
    cout << "Аналитическое решение: " << exact_integral << "\n";

    return 0;
}

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

using namespace std;
using namespace chrono;

const int TARGET = 1000000; // счетчики считают до данного числа
const int NUM_THREADS = 4; // количество потоков

// общие счетчики
int counter_with_mutex = 0;
int counter_without_mutex = 0;
mutex counter_mutex;

// функция с мьютексом
void increment_with_mutex() {
    for (int i = 0; i < TARGET / NUM_THREADS; ++i) {
        counter_mutex.lock();
        ++counter_with_mutex;
        counter_mutex.unlock();
    }
}

// функция без мьютекса
void increment_without_mutex() {
    for (int i = 0; i < TARGET / NUM_THREADS; ++i) {
        ++counter_without_mutex;
    }
}

int main() {
    setlocale(LC_ALL, "");
    vector<thread> threads_with_mutex;
    vector<thread> threads_without_mutex;

    // запускаем потоки с мьютексом
    auto start_with_mutex = high_resolution_clock::now();
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads_with_mutex.emplace_back(increment_with_mutex);
    }

    // ожидаем завершения потоков с мьютексом
    for (auto& thread : threads_with_mutex) {
        thread.join();
    }
    auto end_with_mutex = high_resolution_clock::now();

    // запускаем потоки без мьютекса
    auto start_without_mutex = high_resolution_clock::now();
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads_without_mutex.emplace_back(increment_without_mutex);
    }

    // ожидаем завершения потоков без мьютекса
    for (auto& thread : threads_without_mutex) {
        thread.join();
    }
    auto end_without_mutex = high_resolution_clock::now();

    // выводим результаты
    cout << "Результаты (целевое значение: " << TARGET << ")\n";
    cout << "С мьютексом:\n";
    cout << "  Итоговое значение: " << counter_with_mutex << "\n";
    cout << "  Время выполнения: "
        << duration_cast<milliseconds>(end_with_mutex - start_with_mutex).count()
        << " мс\n\n";

    cout << "Без мьютекса:\n";
    cout << "  Итоговое значение: " << counter_without_mutex << "\n";
    cout << "  Время выполнения: "
        << duration_cast<milliseconds>(end_without_mutex - start_without_mutex).count()
        << " мс\n";

    return 0;
}
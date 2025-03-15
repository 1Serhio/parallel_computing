#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>

using namespace std;
using namespace chrono;

mutex mtx; //мьютекс
unsigned long long factorial = 1; //значение факториала

//функция для вычисления части факториала
void part_fact(unsigned int start, unsigned int end) {
    unsigned long long num = 1;
    for (unsigned int i = start; i <= end; i++) {
        num *= i;
    }

    mtx.lock(); //блокируем мьютекс перед обновлением
    factorial *= num;
    mtx.unlock(); //разблокируем мьютекс после обновления
}

//функция для вычисления факториала многопоточно
unsigned long long multi_threaded_factorial(unsigned int N, int num_threads) {
    vector<thread> threads;
    factorial = 1;

    unsigned int chunk_size = N / num_threads;
    unsigned int remainder = N % num_threads;
    unsigned int start = 1;

    for (int i = 0; i < num_threads; i++) {
        unsigned int end = start + chunk_size - 1;
        if (i == num_threads - 1) end += remainder; //последний поток берет остаток
        threads.emplace_back(part_fact, start, end);
        start = end + 1;
    }

    for (auto& t : threads) {
        t.join();
    }
    return factorial;
}

int main() {
    setlocale(LC_ALL, "");
    unsigned int N;
    cout << "Введите число N: ";
    cin >> N;

    //однопоточное вычисление
    auto start_time = high_resolution_clock::now();
    unsigned long long single_threaded_factorial = 1;
    for (unsigned int i = 2; i <= N; i++) {
        single_threaded_factorial *= i;
    }
    auto end_time = high_resolution_clock::now();
    duration<double> single_thread_duration = end_time - start_time;

    cout << "Факториал (однопоточно) = " << single_threaded_factorial << endl;
    cout << "Время выполнения (однопоточно): " << single_thread_duration.count() << " сек." << endl;

    //многопоточное вычисление
    for (int num_threads = 2; num_threads <= 4; num_threads++) {
        start_time = high_resolution_clock::now();
        unsigned long long multi_threaded_result = multi_threaded_factorial(N, num_threads);
        end_time = high_resolution_clock::now();
        duration<double> multi_thread_duration = end_time - start_time;

        cout << "Факториал (" << num_threads << " потока) = " << multi_threaded_result << endl;
        cout << "Время выполнения (" << num_threads << " потока): " << multi_thread_duration.count() << " сек." << endl;
    }
    return 0;
}

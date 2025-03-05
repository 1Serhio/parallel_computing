#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>

using namespace std;

const int WIDTH = 200;
const int HEIGHT = 50;
const int ITERATIONS = 50;
const char ALIVE = 'o';
const char DEAD = '.';

using Grid = std::vector<std::vector<int>>;

//инициализации игрового поля
void initialize(Grid& grid) {
    srand(time(nullptr)); //инициализация генератора случайных чисел текущим временем
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            grid[i][j] = rand() % 2; //заполняем случайными значениями 0 или 1
        }
    }
}

//вывод игрового поля в консоль
void printGrid(const Grid& grid) {
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            cout << (grid[i][j] ? ALIVE : DEAD);
        }
        cout << "\n";
    }
    cout << "\n";
}

//подсчет живых соседей
int countLiveNeighbors(const Grid& grid, int x, int y) {
    static const int dx[] = { -1, -1, -1, 0, 0, 1, 1, 1 }; //массивы смещений относительно клетки (у каждой клетки, кроме граничных, 8 соседей) 
    static const int dy[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
    int count = 0; //количество живых соседей

    for (int i = 0; i < 8; ++i) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (nx >= 0 && nx < HEIGHT && ny >= 0 && ny < WIDTH) { //проверяем, выходит ли соседняя клетка за пределы поля
            count += grid[nx][ny];
        }
    }
    return count;
}

//обновление состояния поля
void update(Grid& grid, int numThreads) {
    Grid newGrid = grid; //копия для безопасного обновления
    int liveCount = 0; //количество живых клеток на поле

    auto start = std::chrono::high_resolution_clock::now(); //старт таймера

#pragma omp parallel for collapse(2) reduction(+:liveCount) num_threads(numThreads) //параллельное выполнение итераций, объединение двух циклов в один
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            int liveNeighbors = countLiveNeighbors(grid, i, j);//определяем количество живых соседей у каждой клетки
            if (grid[i][j] == 1) {
                newGrid[i][j] = (liveNeighbors == 2 || liveNeighbors == 3) ? 1 : 0;
            }
            else {
                newGrid[i][j] = (liveNeighbors == 3) ? 1 : 0;
            }
            liveCount += newGrid[i][j];
        }
    }

    auto end = std::chrono::high_resolution_clock::now(); //конец таймера
    std::chrono::duration<double> elapsed = end - start; //подсчёт времени

    grid = std::move(newGrid); //обновление игрового поля
    cout << "Потоки: " << numThreads 
         << " | Время: " << elapsed.count() << "s"
         << " | Живые клетки: " << liveCount << "\n";
    
}

int main() {

    setlocale(LC_ALL, "");

    std::vector<int> threadCounts = { 1, 2, 4, 8, 16 }; //разные числа потоков

    for (int threads : threadCounts) {
        Grid grid(HEIGHT, std::vector<int>(WIDTH, 0));
        initialize(grid);
        for (int i = 0; i < ITERATIONS; ++i) {
            cout << "Итерация: " << i + 1 << "\n";
            //printGrid(grid);
            update(grid, threads);
        }
        printGrid(grid);
    }
    return 0;
}

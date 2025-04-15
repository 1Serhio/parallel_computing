#include <opencv2/opencv.hpp>
#include <omp.h>
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;

// Рекурсивная функция для построения ковра Серпинского
void drawSierpinskiCarpet(Mat& image, int x, int y, int size, int depth) {
    if (depth == 0) return;

    int newSize = size / 3;

    // цвет ковра
    Vec3b purple(128, 0, 128); // BGR: blue = 128, green = 0, red = 128

    // Закрашиваем 8 маленьких квадратов
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            // Пропускаем центральную область
            if (i == 1 && j == 1) continue;
            drawSierpinskiCarpet(image, x + i * newSize, y + j * newSize, newSize, depth - 1);
        }
    }
}

int main() {
    setlocale(LC_ALL, "");
    int N, depth, threads;
    cout << "Введите размер изображения (N x N): ";
    cin >> N;
    cout << "Введите глубину рекурсии: ";
    cin >> depth;
    cout << "Введите количество потоков (от 1 до 8): ";
    cin >> threads;

    // Инициализация изображения
    Mat image = Mat::zeros(N, N, CV_8UC3);

    // Установка количества потоков
    omp_set_num_threads(threads);

    // Параллельное построение фрактала
#pragma omp parallel for
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            // Если пиксель находится в "пустой" центральной области на любом уровне рекурсии
            int x = i, y = j;
            int size = N;

            // Поиск принадлежности текущего пикселя к пустой области
            while (size > 1) {
                int mid = size / 3;
                if ((x % size >= mid && x % size < 2 * mid) && (y % size >= mid && y % size < 2 * mid)) {
                    image.at<Vec3b>(i, j) = Vec3b(128, 0, 128); // цвет ковра
                    break;
                }
                size /= 3;
            }
        }
    }

    // Отображение и сохранение результата
    imshow("Sierpinski Carpet", image);
    imwrite("sierpinski_carpet.png", image);

    waitKey(0);
    return 0;
}

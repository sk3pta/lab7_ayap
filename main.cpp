#include <iostream>
#include "my_pack.h"
#include <future>
#include <thread>
#include <random>
#include <ctime>

void array_sort(double *array, size_t from, size_t to, std::string & thread_name) {
    for (size_t i = from; i < to - 1; ++i) {
        for (size_t j = 0; j < i; ++j) {
            if (array[j] > array[j + 1]) {
                auto temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
    array_print(array,from,to);
}

// Функция заполняет массив псевдослучайными числами
double *fill_array(size_t size) {
    auto array = new double[size];
    srandom(static_cast<int>(time(nullptr)));

    for (size_t x = 0; x < size; ++x) {
        array[x] = (double) (random() % 1000) / 10;
    }

    return array;
}


int main(int argc, char * argv[]) {

    // Инициализируем массив и заполним его ГПСЧ
    double *test = fill_array(50);

    std::cout << " Массив до сортировки : " << std::endl;



    //Создаем два потока для одновременной сортировки двух половин массива

    std::future<void> thread1 = std::async(array_sort, test, 0, 25, "thread1");
    std::future<void> thread2 = std::async(array_sort, test, 25, 50);
    //Извлекаем результат выполнения асинхронных функций С++
    thread1.get();
    thread2.get();

    //Асинхронно сортируем весь массив

    std::future<void> thread3 = std::async(array_sort, test, 0, 50);

    thread3.get();
    std::cout << " Массив после третьей сортировки (полностью) " << std::endl;

}

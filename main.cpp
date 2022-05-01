#include <iostream>
#include "my_pack.h"
#include <future>
#include <thread>
#include <random>
#include <ctime>
#include <string>
#include <thread>

// Обрабатываем массив : сортируем методом прямого выбора от индексак from до
// элемента с индексом to. После печатаем элементы в этом пределе вместе с
// именем потока thread_name

void array_proceed(double *array, size_t from, size_t to, const std::string &thread_name) {
    for (size_t i = 0; i < to - 1; ++i) {
        double temp = 0.0;
        size_t min_index = i;
        for (size_t j = i + 1; j < to; ++j) {
            min_index = array[j] < array[min_index] ? j : min_index;
        }
        temp = array[i];
        array[i] = array[min_index];
        array[min_index] = temp;
    }


    for (size_t x = from; x < to; ++x) {
        std::cout << thread_name;
        std::this_thread::sleep_for(std::chrono::milliseconds(0));
        std::cout << "\n" << array[x] << "\n";
    }
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


int main(int argc, char *argv[]) {

    // Инициализируем массив и заполним его ГПСЧ
    double *test_array_async = fill_array(50);
    double *test_array_thread = fill_array(50);

    std::cout << " Массив до сортировки : " << std::endl;
    array_print(test_array_async, 0, 50);

    //Создаем два потока для одновременной сортировки двух половин массива
    std::future<void> thread1 = std::async(array_proceed, test_array_async, 0, 25, "thread1");
    std::future<void> thread2 = std::async(array_proceed, test_array_async, 25, 50, "thread2");
    //Извлекаем результат выполнения асинхронных функций С++
    thread1.get();
    thread2.get();

    //Асинхронно сортируем весь массив
    std::future<void> thread3 = std::async(array_proceed, test_array_async, 0, 50, "thread3");
    thread3.get();
    std::cout << "\n Массив после третьей сортировки (полностью) " << std::endl;
    array_print(test_array_async, 0, 50);


    std::cout << " Массив до сортировки : " << std::endl;
    array_print(test_array_async, 0, 50);

    std::thread thread4(array_proceed, test_array_thread, 0, 25, "thread4");
    std::thread thread5(array_proceed, test_array_thread, 25, 50, "thread5");
    //Это означает, что поток исполнения, который вызвал join,
    // будет ожидать завершения исполнения созданного потока. Блокирует вызывающий поток.
    thread4.join();
    thread5.join();

    std::thread thread6(array_proceed, test_array_thread, 0, 50, "thread6");
    thread6.join();

    std::cout << "\n Массив после третьей сортировки (полностью) " << std::endl;
    array_print(test_array_async, 0, 50);

}

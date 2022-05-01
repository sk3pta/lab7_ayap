#include <iostream>
#include "my_pack.h"
#include <future>
#include <thread>
#include <random>
#include <ctime>
#include <string>
#include <thread>
#include <fstream>

// Функция заполняет массив псевдослучайными числами

double *fill_array(size_t size) {
    auto array = new double[size];
    srandom(static_cast<int>(time(nullptr)));

    for (size_t x = 0; x < size; ++x) {
        array[x] = (double) (random() % 1000) / 10;
    }

    return array;
}

// Функция генерирует файл с числами doubly , созданными ГПСЧ
void generate(size_t size ,std::ostream &out){
    for (size_t x = 0; x < size; ++x) {
        out << ((double) (random() % 1000) / 10) << std::endl;
    }

}



void array_from_file(size_t size,double *array, std::ifstream &inp){
    for (size_t x = 0; x < size; ++x){
        //std::string temp;
        //std::getline(inp,temp);
        //array[x] = std::stod(temp);
        inp >> array[x];
    }
    inp.seekg(0);


}

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




int main(int argc, char *argv[]) {


    // Инициализируем массив и заполним его ГПСЧ
    double *test_array_async = new double[50];
    double *test_array_thread = new double[50];
    std::ifstream inp("CLionProjects/lab2_7/in.txt");
    array_from_file(50,test_array_async,inp);
    array_from_file(50,test_array_thread,inp);
    std::cout << " Массив до сортировки : " << std::endl;
    array_print(test_array_async, 0, 50);

    //Создаем два потока для одновременной сортировки двух половин массива
    std::future<void> thread_async1 = std::async(array_proceed, test_array_async, 0, 25, "thread1_async");
    std::future<void> thread_async2 = std::async(array_proceed, test_array_async, 25, 50, "thread2_async");
    //Извлекаем результат выполнения асинхронных функций С++
    thread_async1.get();
    thread_async2.get();

    //Асинхронно сортируем весь массив
    std::future<void> thread_async_master = std::async(array_proceed, test_array_async, 0, 50, "thread3");
    thread_async_master.get();
    std::cout << "\n Массив после третьей сортировки (полностью) " << std::endl;
    array_print(test_array_async, 0, 50);


    std::cout << " Массив до сортировки : " << std::endl;
    array_print(test_array_async, 0, 50);

    std::thread thread1(array_proceed, test_array_thread, 0, 25, "thread1");
    std::thread thread2(array_proceed, test_array_thread, 25, 50, "thread2");
    //Это означает, что поток исполнения, который вызвал join,
    // будет ожидать завершения исполнения созданного потока. Блокирует вызывающий поток.
    thread1.join();
    thread2.join();

    std::thread thread_master(array_proceed, test_array_thread, 0, 50, "thread_master");
    thread_master.join();

    std::cout << "\n Массив после третьей сортировки (полностью) " << std::endl;
    array_print(test_array_async, 0, 50);

}

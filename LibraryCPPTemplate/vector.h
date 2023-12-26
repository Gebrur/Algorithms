#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef> 
#include <algorithm> // Заголовочный файл для поддержки стандартных алгоритмов

template <typename Data>
class Vector
{
public:
    Vector()  
    {
        vector_size = 0;  
        max_size = 1;  
        data = new Data[max_size]; 
    }

    // Конструктор копирования
    Vector(const Vector &a)
    {
        data = new Data[a.max_size]; // Выделение памяти под новый массив данных
        vector_size = a.vector_size; // Копирование размера вектора
        max_size = a.max_size; // Копирование максимального размера вектора
        for (size_t i = 0; i < vector_size; i++) {
            data[i] = a.data[i]; // Копирование данных из вектора 'a'
        }
    }

    // Оператор присваивания
    Vector &operator=(const Vector &a)
    {
        if (this != &a) { // Проверка на самоприсваивание
            delete[] data; // Освобождение памяти от текущих данных
            vector_size = a.vector_size; // Копирование размера вектора
            max_size = a.max_size; // Копирование максимального размера вектора
            data = new Data[max_size]; // Выделение памяти под новый массив данных

            for (size_t i = 0; i < vector_size; i++) {
                data[i] = a.data[i]; // Копирование данных из вектора 'a'
            }
        }
        return *this; // Возвращение ссылки на текущий объект
    } 

    // Функция обмена содержимым векторов
    void swap(Vector& other)
    {
        // Используется std::swap для обмена указателями на данные, размерами и максимальными размерами
        std::swap(data, other.data);
        std::swap(vector_size, other.vector_size);
        std::swap(max_size, other.max_size);
    }

    // Деструктор
    ~Vector()  
    {
        delete[] data;  // Освобождение памяти от массива данных
    }

    // Получение элемента по индексу
    Data get(size_t index) const {
        if (index < vector_size)
            return data[index];
        else
            throw std::out_of_range("Index out of range"); // Исключение при выходе за пределы массива
    }

    // Установка значения элемента по индексу
    void set(size_t index, Data value)  
    {
        if (index < vector_size)  
            data[index] = value;  // Установка значения элемента
        else
            throw std::out_of_range("Index out of range");  // Исключение при выходе за пределы массива
    }

    // Получение текущего размера вектора
    size_t size() const  
    {
        return vector_size; // Возврат размера вектора
    }

    // Изменение размера вектора
    void resize(size_t size)  
    {
        if (size <= max_size)  
        {
            vector_size = size;  // Установка нового размера вектора
            return;
        }

        size_t _max_size = size * 2;  // Вычисление нового максимального размера
        Data* new_data = new Data[_max_size];  // Выделение памяти под новый массив данных

        for (size_t i = 0; i < vector_size; i++)  
        {
            new_data[i] = data[i]; // Копирование существующих данных
        }

        delete[] data;  
        data = new_data;  // Установка указателя на новый массив данных
        max_size = _max_size;  // Установка нового максимального размера
        vector_size = size;  // Установка нового размера вектора
    }

private:
    Data* data;  // Указатель на массив данных
    size_t vector_size;  // Текущий размер вектора
    size_t max_size;  // Максимальный размер вектора
};

#endif 

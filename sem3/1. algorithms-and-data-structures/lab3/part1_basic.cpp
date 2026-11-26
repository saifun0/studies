#include <iostream>
#include <windows.h>
#include <string>
#include <stdexcept>

using namespace std;

// Шаблонный класс динамического массива (аналог std::vector)
template <typename T>
class Vector {
private:
    T* data;  // указатель на массив в динамической памяти
    int sz;   // текущее количество элементов
    int cap;  // выделенная емкость

public:
    // Конструктор по умолчанию: пустой вектор, сложность O(1)
    Vector() : data(nullptr), sz(0), cap(0) {}

    // Конструктор с параметром размера, сложность O(n)
    explicit Vector(int n) : data(nullptr), sz(0), cap(0) {
        if (n > 0) {
            sz = n;
            cap = n;
            data = new T[cap];
            for (int i = 0; i < sz; i++) {
                data[i] = T();
            }
        }
    }

    // Деструктор освобождает выделенную память, сложность O(1)
    ~Vector() {
        delete[] data;
    }

    // Добавление в конец с удвоением емкости, сложность O(1) амортизированно
    void push_back(const T& value) {
        if (sz >= cap) {
            int newCap = (cap == 0) ? 1 : cap * 2;
            T* newData = new T[newCap];
            for (int i = 0; i < sz; i++) {
                newData[i] = data[i];
            }
            delete[] data;
            data = newData;
            cap = newCap;
        }
        data[sz] = value;
        sz++;
    }

    // Удаление последнего элемента, сложность O(1)
    void pop_back() {
        if (sz == 0) {
            cout << "Вектор пуст, удалять нечего!" << endl;
            return;
        }
        sz--;
    }

    // Текущий размер и емкость, сложность O(1)
    int size() const { return sz; }
    int capacity() const { return cap; }

    // Доступ по индексу с проверкой границ, сложность O(1)
    T& operator[](int index) {
        if (index < 0 || index >= sz) {
            throw out_of_range("Индекс выходит за границы вектора");
        }
        return data[index];
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= sz) {
            throw out_of_range("Индекс выходит за границы вектора");
        }
        return data[index];
    }

    // Вывод элементов вектора, сложность O(n)
    void print() const {
        cout << "[";
        for (int i = 0; i < sz; i++) {
            cout << data[i];
            if (i < sz - 1) cout << ", ";
        }
        cout << "]" << endl;
    }
};

int main() {
    SetConsoleOutputCP(CP_UTF8); // включаем UTF-8 в консоли Windows

    // 1. Вектор через конструктор по умолчанию
    Vector<int> v;
    cout << "Начальный размер: " << v.size() << ", емкость: " << v.capacity() << endl;

    for (int i = 1; i <= 6; i++) {
        v.push_back(i * 10);
        cout << "push_back(" << i * 10 << ") -> размер: " << v.size()
             << ", емкость: " << v.capacity() << endl;
    }

    cout << "Элементы v: ";
    v.print();

    // Удаление элементов с конца
    v.pop_back();
    v.pop_back();
    cout << "После двух pop_back: ";
    v.print();
    cout << "Размер: " << v.size() << ", емкость: " << v.capacity() << endl;

    // 2. Вектор через конструктор с размером
    Vector<int> v2(4);
    for (int i = 0; i < v2.size(); i++) {
        v2[i] = i + 1;
    }
    cout << "\nВектор v2(4): ";
    v2.print();

    // 3. Проверка шаблонности на строках
    Vector<string> words;
    words.push_back("первый");
    words.push_back("второй");
    words.push_back("третий");
    cout << "\nВектор строк: ";
    words.print();

    // Проверка pop_back на пустом векторе
    Vector<int> emptyVec;
    emptyVec.pop_back();

    return 0;
}

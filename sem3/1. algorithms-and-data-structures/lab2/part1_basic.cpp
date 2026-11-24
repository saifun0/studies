#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

// Шаблонная функция поиска максимума из двух значений (сложность O(1))
template <typename T>
T getMax(T a, T b) {
    return (a > b) ? a : b; // если a > b, вернуть a, иначе b
}

int main() {
    SetConsoleOutputCP(CP_UTF8); // включаем UTF-8 в консоли Windows

    // Демонстрация работы шаблона с разными типами данных
    cout << getMax(1, 2) << endl;       // int
    cout << getMax(1.11, 2.22) << endl; // double
    cout << getMax('a', 'z') << endl;   // char

    // Сравнение строк (std::string, а не const char*)
    string s1 = "aa";
    string s2 = "a";

    cout << getMax(s1, s2) << endl;

    return 0;
}
#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

// Шаблонный класс для хранения пары значений произвольных типов T1 и T2
template <typename T1, typename T2>
class Pair {
private:
    T1 first;
    T2 second;

public:
    // Конструктор: инициализация двух полей, сложность O(1)
    Pair(const T1& f, const T2& s) : first(f), second(s) {}

    // Геттеры: чтение полей, сложность O(1)
    T1 getFirst() const { return first; }
    T2 getSecond() const { return second; }

    // Сеттеры: запись новых значений, сложность O(1)
    void setFirst(const T1& f) { first = f; }
    void setSecond(const T2& s) { second = s; }

    // Вывод пары в консоль, сложность O(1)
    void print() const { cout << first << ", " << second << endl; }
};

int main() {
    SetConsoleOutputCP(CP_UTF8); // включаем UTF-8 в консоли Windows

    // Создание пар с разными комбинациями типов
    Pair<string, int> p1("Возраст", 18);
    Pair<int, double> p2(20, 13.2);
    Pair<int, string> p3(13, "книг");

    p1.print();
    p2.print();
    p3.print();

    // Изменение значений через сеттеры
    p1.setFirst("Количество");
    p1.setSecond(20);

    p1.print();

    // Проверка работы геттеров
    cout << p1.getFirst() << " " << p1.getSecond() << endl;

    return 0;
}
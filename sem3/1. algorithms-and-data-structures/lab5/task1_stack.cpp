#include <iostream>
#include <windows.h>
#include <string>
#include <stdexcept>

using namespace std;

// Шаблонный класс стека (LIFO) на динамическом массиве
template <typename T>
class Stack {
private:
    T* data;
    int capacity;
    int topIndex;

    // Удвоение емкости при заполнении, сложность O(n)
    void resize() {
        int newCap = capacity * 2;
        T* newData = new T[newCap];
        for (int i = 0; i <= topIndex; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCap;
    }

public:
    Stack(int initialCap = 10) : data(new T[initialCap]), capacity(initialCap), topIndex(-1) {}

    ~Stack() {
        delete[] data;
    }

    // Добавление на вершину, сложность O(1) амортизированно
    void push(const T& value) {
        if (topIndex >= capacity - 1) {
            resize();
        }
        topIndex++;
        data[topIndex] = value;
    }

    // Извлечение с вершины, сложность O(1)
    T pop() {
        if (isEmpty()) {
            throw underflow_error("Стек пуст!");
        }
        return data[topIndex--];
    }

    // Просмотр вершины без удаления, сложность O(1)
    T top() const {
        if (isEmpty()) {
            throw underflow_error("Стек пуст!");
        }
        return data[topIndex];
    }

    bool isEmpty() const { return topIndex == -1; }
    int size() const { return topIndex + 1; }
};

// Задание 1 (вариант 1): проверка правильности расстановки скобок, сложность O(n)
bool checkBrackets(const string& expr) {
    Stack<char> st;

    for (char ch : expr) {
        if (ch == '(' || ch == '[' || ch == '{') {
            st.push(ch);
        } else if (ch == ')' || ch == ']' || ch == '}') {
            if (st.isEmpty()) {
                return false;
            }
            char open = st.pop();
            if ((ch == ')' && open != '(') ||
                (ch == ']' && open != '[') ||
                (ch == '}' && open != '{')) {
                return false;
            }
        }
    }

    return st.isEmpty();
}

int main() {
    SetConsoleOutputCP(CP_UTF8); // включаем UTF-8 в консоли Windows

    string tests[] = {
        "()",
        "([]{})",
        "{[()]}",
        "((())",
        "([)]",
        ")("
    };

    cout << "Проверка скобок:" << endl;
    for (const string& s : tests) {
        cout << s << " -> " << (checkBrackets(s) ? "ВЕРНО" : "НЕВЕРНО") << endl;
    }

    // Проверка обработки ошибки на пустом стеке
    Stack<int> emptyStack;
    try {
        emptyStack.pop();
    } catch (const exception& e) {
        cout << "\nОшибка при pop из пустого стека: " << e.what() << endl;
    }

    return 0;
}

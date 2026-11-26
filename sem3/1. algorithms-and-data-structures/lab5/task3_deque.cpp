#include <iostream>
#include <windows.h>
#include <string>
#include <stdexcept>

using namespace std;

// Шаблонный класс дека (двусторонняя очередь) на двусвязном списке
template <typename T>
class Deque {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        Node(const T& value) : data(value), prev(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int count;

public:
    Deque() : head(nullptr), tail(nullptr), count(0) {}

    // Деструктор освобождает все узлы, сложность O(n)
    ~Deque() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // Добавление в начало, сложность O(1)
    void push_front(const T& value) {
        Node* newNode = new Node(value);
        if (head == nullptr) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        count++;
    }

    // Добавление в конец, сложность O(1)
    void push_back(const T& value) {
        Node* newNode = new Node(value);
        if (tail == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        count++;
    }

    // Удаление из начала, сложность O(1)
    T pop_front() {
        if (isEmpty()) {
            throw underflow_error("Дек пуст!");
        }
        Node* temp = head;
        T value = temp->data;
        head = head->next;
        if (head != nullptr) {
            head->prev = nullptr;
        } else {
            tail = nullptr;
        }
        delete temp;
        count--;
        return value;
    }

    // Удаление из конца, сложность O(1)
    T pop_back() {
        if (isEmpty()) {
            throw underflow_error("Дек пуст!");
        }
        Node* temp = tail;
        T value = temp->data;
        tail = tail->prev;
        if (tail != nullptr) {
            tail->next = nullptr;
        } else {
            head = nullptr;
        }
        delete temp;
        count--;
        return value;
    }

    // Просмотр первого и последнего элемента, сложность O(1)
    T front() const {
        if (isEmpty()) {
            throw underflow_error("Дек пуст!");
        }
        return head->data;
    }

    T back() const {
        if (isEmpty()) {
            throw underflow_error("Дек пуст!");
        }
        return tail->data;
    }

    bool isEmpty() const { return count == 0; }
    int size() const { return count; }
};

// Задание 3 (вариант 1): проверка строки на палиндром через дек, сложность O(n)
bool isPalindrome(const string& str) {
    Deque<char> dq;
    for (char ch : str) {
        dq.push_back(ch);
    }

    // Сравниваем символы одновременно с начала и с конца
    while (dq.size() > 1) {
        if (dq.pop_front() != dq.pop_back()) {
            return false;
        }
    }
    return true;
}

int main() {
    SetConsoleOutputCP(CP_UTF8); // включаем UTF-8 в консоли Windows

    string words[] = {"radar", "level", "abccba", "hello", "student"};

    cout << "Проверка палиндромов:" << endl;
    for (const string& w : words) {
        cout << w << " -> " << (isPalindrome(w) ? "ПАЛИНДРОМ" : "НЕ палиндром") << endl;
    }

    // Короткая проверка работы обоих концов дека
    Deque<int> dq;
    dq.push_back(20);
    dq.push_front(10);
    dq.push_back(30);
    cout << "\nfront: " << dq.front() << ", back: " << dq.back() << ", size: " << dq.size() << endl;

    // Проверка обработки ошибки на пустом деке
    Deque<int> emptyDq;
    try {
        emptyDq.pop_front();
    } catch (const exception& e) {
        cout << "Ошибка при pop_front из пустого дека: " << e.what() << endl;
    }

    return 0;
}

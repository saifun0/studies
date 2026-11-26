#include <iostream>
#include <windows.h>
#include <stdexcept>

using namespace std;

// Шаблонный класс односвязного списка
template <typename T>
class SinglyLinkedList {
private:
    // Узел списка: хранит значение и указатель на следующий элемент
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* head; // указатель на первый элемент
    Node* tail; // указатель на последний элемент
    int count;  // количество элементов

public:
    // Конструктор пустого списка, сложность O(1)
    SinglyLinkedList() : head(nullptr), tail(nullptr), count(0) {}

    // Деструктор освобождает память всех узлов, сложность O(n)
    ~SinglyLinkedList() {
        clear();
    }

    // Вставка элемента в начало списка, сложность O(1)
    void push_front(const T& value) {
        Node* newNode = new Node(value);
        newNode->next = head;
        head = newNode;
        if (tail == nullptr) {
            tail = newNode;
        }
        count++;
    }

    // Удаление элемента из начала списка, сложность O(1)
    void pop_front() {
        if (head == nullptr) {
            throw underflow_error("Список пуст!");
        }
        Node* temp = head;
        head = head->next;
        delete temp;
        count--;
        if (head == nullptr) {
            tail = nullptr;
        }
    }

    // Вывод всех элементов списка, сложность O(n)
    void print() const {
        Node* curr = head;
        cout << "[";
        while (curr != nullptr) {
            cout << curr->data;
            if (curr->next != nullptr) cout << ", ";
            curr = curr->next;
        }
        cout << "]" << endl;
    }

    // Дополнительное задание (вариант 1): сумма всех элементов, сложность O(n)
    T sum() const {
        T total = T();
        Node* curr = head;
        while (curr != nullptr) {
            total += curr->data;
            curr = curr->next;
        }
        return total;
    }

    // Очистка списка с удалением каждого узла, сложность O(n)
    void clear() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        count = 0;
    }

    int size() const { return count; }
    bool empty() const { return head == nullptr; }
};

int main() {
    SetConsoleOutputCP(CP_UTF8); // включаем UTF-8 в консоли Windows

    SinglyLinkedList<int> list;

    // Добавляем элементы в начало (последний добавленный станет первым)
    list.push_front(10);
    list.push_front(25);
    list.push_front(15);
    list.push_front(5);

    cout << "Список: ";
    list.print();
    cout << "Размер: " << list.size() << endl;
    cout << "Сумма элементов: " << list.sum() << endl;

    // Удаляем первый элемент
    list.pop_front();
    cout << "\nПосле pop_front: ";
    list.print();
    cout << "Новая сумма элементов: " << list.sum() << endl;

    // Проверка работы с дробными числами
    SinglyLinkedList<double> dlist;
    dlist.push_front(1.5);
    dlist.push_front(2.5);
    dlist.push_front(3.2);
    cout << "\nСписок double: ";
    dlist.print();
    cout << "Сумма double: " << dlist.sum() << endl;

    // Проверка обработки пустого списка
    SinglyLinkedList<int> emptyList;
    try {
        emptyList.pop_front();
    } catch (const exception& e) {
        cout << "\nПоймано исключение: " << e.what() << endl;
    }

    return 0;
}

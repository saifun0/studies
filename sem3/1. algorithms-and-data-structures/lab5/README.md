# Лабораторная работа №5: Стек, очередь, дек, приоритетная очередь

[К разделу АСД](../README.md) / [Семестр 3](../../README.md)

Дисциплина: Алгоритмы и структуры данных (семестр 3)  
Студент: Гаджиев Магомед Исламович  
Группа: ПИЖ-б-о-25-2(2)  
Вариант: 1  
Язык и компилятор: C++17, MinGW `g++`  
Репозиторий: [saifun0/studies (lab5)](https://github.com/saifun0/studies/tree/main/sem3/1.%20algorithms-and-data-structures/lab5)

---

## Содержание
1. [Цель работы](#цель-работы)
2. [Теоретическая часть](#теоретическая-часть)
3. [Постановка задачи](#постановка-задачи)
4. [Структура папки и сборка](#структура-папки-и-сборка)
5. [Листинги программ](#листинги-программ)
6. [Результаты работы](#результаты-работы)
7. [Анализ сложности операций](#анализ-сложности-операций)
8. [Ответы на контрольные вопросы](#ответы-на-контрольные-вопросы)
9. [Выводы](#выводы)

---

## Цель работы

Реализовать с нуля (без контейнеров STL) четыре основные структуры данных: стек (`Stack`), очередь (`Queue`), дек (`Deque`) и приоритетную очередь (`PriorityQueue` на двоичной куче), обработать ошибки обращения к пустой структуре и применить каждую из них для решения практической задачи варианта 1.

---

## Теоретическая часть

Стек (`Stack`) работает по принципу LIFO ("последним пришел, первым ушел"). Добавление (`push`), извлечение (`pop`) и чтение (`top`) выполняются с одного конца (вершины) за `O(1)`. Удобно реализуется на динамическом массиве с индексом `topIndex`.

Очередь (`Queue`) работает по принципу FIFO ("первым пришел, первым ушел"). Новые элементы добавляются в конец (`enqueue`), а извлекаются из начала (`dequeue`). При реализации на кольцевом буфере индексы `frontIndex` и `rearIndex` двигаются по модулю емкости (`(index + 1) % capacity`), поэтому обе операции выполняются за `O(1)` без сдвига массива.

Дек (`Deque`, двусторонняя очередь) допускает добавление и удаление элементов как с начала (`push_front`, `pop_front`), так и с конца (`push_back`, `pop_back`). На базе двусвязного списка все операции на концах работают за `O(1)`.

Приоритетная очередь (`PriorityQueue`) всегда отдает элемент с наибольшим приоритетом. На основе двоичной кучи (`max-heap`) в массиве родитель узла `i` находится в `(i - 1) / 2`, а дети в `2 * i + 1` и `2 * i + 2`. Просеивание вверх (`heapifyUp`) и вниз (`heapifyDown`) выполняются за `O(log n)`, а чтение максимума `top()` за `O(1)`.

---

## Постановка задачи

В лабораторной работе №5 критерии едины (4 задания по 25 баллов). Для варианта 1 нужно реализовать все четыре структуры данных и решить по одной задаче на каждую:

1. Стек (`task1_stack.cpp`): реализовать `Stack<T>` на динамическом массиве и проверить правильность расстановки скобок в выражении вида `([]{})`.
2. Очередь (`task2_queue.cpp`): реализовать `Queue<T>` на кольцевом буфере и смоделировать очередь в банке с подсчетом времени ожидания и обслуживания клиентов.
3. Дек (`task3_deque.cpp`): реализовать `Deque<T>` на двусвязном списке и проверить, является ли строка палиндромом.
4. Приоритетная очередь (`task4_priority_queue.cpp`): реализовать `PriorityQueue<T>` на основе `max-heap` и найти `K` наибольших элементов в массиве.

---

## Структура папки и сборка

```text
lab5/
├── task1_stack.cpp           # Стек и проверка скобок
├── task2_queue.cpp           # Очередь и моделирование банка
├── task3_deque.cpp           # Дек и проверка палиндромов
├── task4_priority_queue.cpp  # Приоритетная очередь и K наибольших элементов
└── README.md                 # Отчет по лабораторной работе
```

Команды компиляции и запуска в PowerShell (MinGW `g++`):

```powershell
g++ -Wall -Wextra -std=c++17 task1_stack.cpp -o task1_stack.exe
.\task1_stack.exe

g++ -Wall -Wextra -std=c++17 task2_queue.cpp -o task2_queue.exe
.\task2_queue.exe

g++ -Wall -Wextra -std=c++17 task3_deque.cpp -o task3_deque.exe
.\task3_deque.exe

g++ -Wall -Wextra -std=c++17 task4_priority_queue.cpp -o task4_priority_queue.exe
.\task4_priority_queue.exe
```

Все четыре файла собираются без ошибок и предупреждений.

---

## Листинги программ

### 1. Стек и проверка скобок (`task1_stack.cpp`)

```cpp
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
```

### 2. Очередь и моделирование очереди в банке (`task2_queue.cpp`)

```cpp
#include <iostream>
#include <windows.h>
#include <string>
#include <stdexcept>

using namespace std;

// Шаблонный класс очереди (FIFO) на кольцевом буфере
template <typename T>
class Queue {
private:
    T* data;
    int capacity;
    int frontIndex;
    int rearIndex;
    int count;

    // Расширение кольцевого буфера при заполнении, сложность O(n)
    void resize() {
        int newCap = capacity * 2;
        T* newData = new T[newCap];
        for (int i = 0; i < count; i++) {
            newData[i] = data[(frontIndex + i) % capacity];
        }
        delete[] data;
        data = newData;
        capacity = newCap;
        frontIndex = 0;
        rearIndex = count;
    }

public:
    Queue(int initialCap = 10)
        : data(new T[initialCap]), capacity(initialCap), frontIndex(0), rearIndex(0), count(0) {}

    ~Queue() {
        delete[] data;
    }

    // Добавление в конец очереди, сложность O(1) амортизированно
    void enqueue(const T& value) {
        if (count >= capacity) {
            resize();
        }
        data[rearIndex] = value;
        rearIndex = (rearIndex + 1) % capacity;
        count++;
    }

    // Извлечение из начала очереди, сложность O(1)
    T dequeue() {
        if (isEmpty()) {
            throw underflow_error("Очередь пуста!");
        }
        T value = data[frontIndex];
        frontIndex = (frontIndex + 1) % capacity;
        count--;
        return value;
    }

    // Просмотр первого элемента без удаления, сложность O(1)
    T front() const {
        if (isEmpty()) {
            throw underflow_error("Очередь пуста!");
        }
        return data[frontIndex];
    }

    bool isEmpty() const { return count == 0; }
    int size() const { return count; }
};

// Клиент банка для моделирования очереди
struct Client {
    string name;
    int serviceTime; // время обслуживания в минутах
};

int main() {
    SetConsoleOutputCP(CP_UTF8); // включаем UTF-8 в консоли Windows

    Queue<Client> bankQueue;

    // Клиенты встают в очередь
    bankQueue.enqueue({"Иван", 5});
    bankQueue.enqueue({"Мария", 3});
    bankQueue.enqueue({"Алексей", 7});
    bankQueue.enqueue({"Ольга", 4});

    cout << "Клиентов в очереди: " << bankQueue.size() << endl;
    cout << "Первый на очереди: " << bankQueue.front().name << endl << endl;

    int currentTime = 0;  // текущее время работы кассы
    int totalWait = 0;    // суммарное время ожидания всех клиентов
    int servedCount = 0;

    // Обслуживание клиентов по принципу FIFO
    while (!bankQueue.isEmpty()) {
        Client c = bankQueue.dequeue();
        int waitTime = currentTime;
        currentTime += c.serviceTime;
        totalWait += waitTime;
        servedCount++;

        cout << "Клиент " << c.name
             << " | ждал: " << waitTime << " мин"
             << " | обслуживание: " << c.serviceTime << " мин"
             << " | завершил на " << currentTime << "-й мин" << endl;
    }

    cout << "\nОбщее время работы: " << currentTime << " мин" << endl;
    cout << "Среднее время ожидания: " << (double)totalWait / servedCount << " мин" << endl;

    // Проверка обработки ошибки на пустой очереди
    try {
        bankQueue.dequeue();
    } catch (const exception& e) {
        cout << "\nОшибка при dequeue из пустой очереди: " << e.what() << endl;
    }

    return 0;
}
```

### 3. Дек и проверка палиндромов (`task3_deque.cpp`)

```cpp
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
```

### 4. Приоритетная очередь и K наибольших элементов (`task4_priority_queue.cpp`)

```cpp
#include <iostream>
#include <windows.h>
#include <stdexcept>

using namespace std;

// Шаблонный класс приоритетной очереди на основе двоичной кучи (max-heap)
template <typename T>
class PriorityQueue {
private:
    T* heap;
    int capacity;
    int count;

    // Расширение массива кучи при переполнении, сложность O(n)
    void resize() {
        int newCap = capacity * 2;
        T* newHeap = new T[newCap];
        for (int i = 0; i < count; i++) {
            newHeap[i] = heap[i];
        }
        delete[] heap;
        heap = newHeap;
        capacity = newCap;
    }

    // Просеивание вверх после вставки, сложность O(log n)
    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[index] > heap[parent]) {
                swap(heap[index], heap[parent]);
                index = parent;
            } else {
                break;
            }
        }
    }

    // Просеивание вниз после удаления максимума, сложность O(log n)
    void heapifyDown(int index) {
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int largest = index;

            if (left < count && heap[left] > heap[largest]) largest = left;
            if (right < count && heap[right] > heap[largest]) largest = right;

            if (largest == index) break;

            swap(heap[index], heap[largest]);
            index = largest;
        }
    }

public:
    PriorityQueue(int initialCap = 10) : heap(new T[initialCap]), capacity(initialCap), count(0) {}

    ~PriorityQueue() {
        delete[] heap;
    }

    // Добавление элемента в кучу, сложность O(log n)
    void push(const T& value) {
        if (count >= capacity) {
            resize();
        }
        heap[count] = value;
        heapifyUp(count);
        count++;
    }

    // Извлечение наибольшего элемента, сложность O(log n)
    T pop() {
        if (isEmpty()) {
            throw underflow_error("Приоритетная очередь пуста!");
        }
        T maxVal = heap[0];
        heap[0] = heap[count - 1];
        count--;
        if (count > 0) {
            heapifyDown(0);
        }
        return maxVal;
    }

    // Просмотр наибольшего элемента, сложность O(1)
    T top() const {
        if (isEmpty()) {
            throw underflow_error("Приоритетная очередь пуста!");
        }
        return heap[0];
    }

    bool isEmpty() const { return count == 0; }
    int size() const { return count; }
};

// Задание 4 (вариант 1): найти K наибольших элементов массива, сложность O((n + k) log n)
void printTopK(const int arr[], int n, int k) {
    PriorityQueue<int> pq;
    for (int i = 0; i < n; i++) {
        pq.push(arr[i]);
    }

    if (k > n) k = n;
    cout << k << " наибольших элементов: ";
    for (int i = 0; i < k; i++) {
        cout << pq.pop();
        if (i < k - 1) cout << ", ";
    }
    cout << endl;
}

int main() {
    SetConsoleOutputCP(CP_UTF8); // включаем UTF-8 в консоли Windows

    int arr[] = {12, 5, 38, 1, 90, 24, 55, 7, 42};
    int n = sizeof(arr) / sizeof(arr[0]);

    cout << "Исходный массив: ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    printTopK(arr, n, 3);
    printTopK(arr, n, 5);

    // Проверка обработки ошибки на пустой очереди
    PriorityQueue<int> emptyPq;
    try {
        emptyPq.pop();
    } catch (const exception& e) {
        cout << "\nОшибка при pop из пустой приоритетной очереди: " << e.what() << endl;
    }

    return 0;
}
```

---

## Результаты работы

### Вывод `task1_stack.exe`

```text
Проверка скобок:
() -> ВЕРНО
([]{}) -> ВЕРНО
{[()]} -> ВЕРНО
((()) -> НЕВЕРНО
([)] -> НЕВЕРНО
)( -> НЕВЕРНО

Ошибка при pop из пустого стека: Стек пуст!
```

### Вывод `task2_queue.exe`

```text
Клиентов в очереди: 4
Первый на очереди: Иван

Клиент Иван | ждал: 0 мин | обслуживание: 5 мин | завершил на 5-й мин
Клиент Мария | ждал: 5 мин | обслуживание: 3 мин | завершил на 8-й мин
Клиент Алексей | ждал: 8 мин | обслуживание: 7 мин | завершил на 15-й мин
Клиент Ольга | ждал: 15 мин | обслуживание: 4 мин | завершил на 19-й мин

Общее время работы: 19 мин
Среднее время ожидания: 7 мин

Ошибка при dequeue из пустой очереди: Очередь пуста!
```

### Вывод `task3_deque.exe`

```text
Проверка палиндромов:
radar -> ПАЛИНДРОМ
level -> ПАЛИНДРОМ
abccba -> ПАЛИНДРОМ
hello -> НЕ палиндром
student -> НЕ палиндром

front: 10, back: 30, size: 3
Ошибка при pop_front из пустого дека: Дек пуст!
```

### Вывод `task4_priority_queue.exe`

```text
Исходный массив: 12 5 38 1 90 24 55 7 42 
3 наибольших элементов: 90, 55, 42
5 наибольших элементов: 90, 55, 42, 38, 24

Ошибка при pop из пустой приоритетной очереди: Приоритетная очередь пуста!
```

---

## Анализ сложности операций

| Структура | Операция | Сложность | Пояснение |
| :--- | :--- | :--- | :--- |
| `Stack` | `push` | `O(1)` амортизированно | Запись по `topIndex`, редкое удвоение массива за `O(n)` |
| `Stack` | `pop`, `top`, `isEmpty`, `size` | `O(1)` | Прямое обращение к `data[topIndex]` и счетчику |
| `Stack` | `checkBrackets` | `O(n)` | Один проход по строке длины `n` |
| `Queue` | `enqueue` | `O(1)` амортизированно | Запись в `data[rearIndex]` по модулю, редкое расширение |
| `Queue` | `dequeue`, `front`, `isEmpty`, `size` | `O(1)` | Сдвиг `frontIndex = (frontIndex + 1) % capacity` |
| `Deque` | `push_front`, `push_back`, `pop_front`, `pop_back`, `front`, `back` | `O(1)` | Обновление указателей `head` и `tail` двусвязного списка |
| `Deque` | `isPalindrome` | `O(n)` | Загрузка `n` символов и попарное сравнение с двух концов |
| `PriorityQueue` | `push` | `O(log n)` | Вставка в конец кучи и `heapifyUp` по высоте дерева |
| `PriorityQueue` | `pop` | `O(log n)` | Перенос последнего элемента в корень и `heapifyDown` |
| `PriorityQueue` | `top`, `isEmpty`, `size` | `O(1)` | Максимум всегда лежит в `heap[0]` |
| `PriorityQueue` | `printTopK` | `O((n + k) log n)` | `n` вставок в кучу и `k` извлечений максимума |

---

## Ответы на контрольные вопросы

### 1. В чём заключается принцип работы стека (LIFO)?
LIFO (Last In, First Out) означает, что элемент, добавленный в стек последним, извлекается из него первым. Работа идет только с одной стороной структуры, которую называют вершиной стека.

### 2. В чём заключается принцип работы очереди (FIFO)?
FIFO (First In, First Out) означает, что элемент, попавший в очередь раньше остальных, будет извлечен первым. Новые элементы всегда встают в конец, а удаляются с начала.

### 3. Чем дек отличается от обычной очереди?
В обычной очереди добавлять элементы можно только в конец, а удалять только из начала. Дек (double-ended queue) позволяет добавлять, просматривать и удалять элементы с обоих концов за `O(1)`.

### 4. Что такое приоритетная очередь? Где она применяется?
Это структура данных, где у каждого элемента есть приоритет, и при извлечении всегда выдается элемент с наибольшим (или наименьшим) приоритетом независимо от порядка добавления. Она применяется в планировщиках задач ОС, алгоритмах на графах (Дейкстры, Прима), сжатии Хаффмана и обработке событий по времени.

### 5. Как реализовать стек на основе динамического массива?
Хранят динамический массив `T* data`, емкость `capacity` и индекс вершины `topIndex` (изначально `-1`). При `push` при необходимости расширяют массив в два раза и записывают элемент в `data[++topIndex]`. При `pop` проверяют, что `topIndex >= 0`, и возвращают `data[topIndex--]`. В деструкторе вызывают `delete[] data`.

### 6. Как реализовать очередь на основе кольцевого буфера?
В массиве емкости `capacity` хранят индекс начала `frontIndex`, индекс следующей свободной ячейки `rearIndex` и количество элементов `count`. После каждой вставки или удаления соответствующий индекс сдвигается по формуле `(index + 1) % capacity`, поэтому после конца массива запись продолжается с индекса 0. Если `count == capacity`, выделяется новый массив вдвое больше и элементы переносятся по порядку начиная с `frontIndex`.

### 7. Почему приоритетную очередь обычно реализуют на куче?
Двоичная куча хранится в обычном непрерывном массиве без указателей на детей и дает гарантированное `O(log n)` на вставку и удаление максимума и `O(1)` на просмотр максимума. Если делать приоритетную очередь на неотсортированном массиве, удаление максимума займет `O(n)`, а на отсортированном массиве вставка займет `O(n)`.

### 8. Какова сложность операций push и pop в стеке?
Операция `pop` всегда выполняется за `O(1)`. Операция `push` на динамическом массиве имеет амортизированную сложность `O(1)` (редкое расширение занимает `O(n)`), а на связном списке строго `O(1)`.

### 9. Какова сложность операций enqueue и dequeue в очереди?
На кольцевом буфере `dequeue` работает за `O(1)`, а `enqueue` за `O(1)` амортизированно (при переполнении буфер расширяется за `O(n)`). На связном списке с указателями `head` и `tail` обе операции работают строго за `O(1)`.

### 10. Какова сложность операций в приоритетной очереди? Почему?
`top()` работает за `O(1)`, так как максимальный элемент всегда хранится в корне кучи `heap[0]`. Операции `push` и `pop` работают за `O(log n)`, потому что куча представляет собой полное двоичное дерево высоты `log2(n)`, и при просеивании элемент проходит максимум один путь от листа к корню или обратно.

### 11. Как проверить правильность расстановки скобок с помощью стека?
Строку просматривают посимвольно. Каждую открывающую скобку (`(`, `[`, `{`) кладут в стек. При встрече закрывающей скобки проверяют, что стек не пуст, извлекают верхний элемент и сверяют, образует ли он правильную пару. Если стек оказался пуст или пара не совпала, выражение неверно. После прохода всей строки стек должен остаться пустым.

### 12. Как реализовать очередь на двух стеках? Какова сложность?
Используют два стека: `inStack` (для приема новых элементов) и `outStack` (для выдачи). Операция `enqueue` просто кладет элемент в `inStack` за `O(1)`. При `dequeue`, если `outStack` пуст, все элементы перекладываются по одному из `inStack` в `outStack` (их порядок переворачивается), после чего берется вершина `outStack`. Каждый элемент перекладывается не больше одного раза, поэтому амортизированная сложность обеих операций равна `O(1)`.

### 13. Как реализовать дек на основе двусвязного списка?
Хранят узлы `Node` с полями `data`, `prev`, `next`, а также указатели `head`, `tail` и счетчик `count`. Операции `push_front`/`pop_front` меняют `head` и `head->prev`, а `push_back`/`pop_back` меняют `tail` и `tail->next`. Когда удаляется последний элемент, оба указателя `head` и `tail` обнуляются.

### 14. Как реализовать дек на основе кольцевого буфера?
Так же, как кольцевую очередь, но сдвигать индексы нужно в обе стороны: `push_back` увеличивает `rear` как `(rear + 1) % cap`, `pop_front` увеличивает `front` как `(front + 1) % cap`, `push_front` сначала сдвигает `front` назад по формуле `(front - 1 + cap) % cap`, а `pop_back` сдвигает `rear` назад как `(rear - 1 + cap) % cap`.

### 15. Как реализовать стек с поддержкой получения минимума за O(1)?
Вместе с основным стеком значений ведут второй вспомогательный стек минимумов `minStack`. При каждом `push(x)` в `minStack` кладут `min(x, minStack.top())` (или сам `x`, если стек был пуст). При `pop()` удаляют вершину сразу из обоих стеков. Тогда текущий минимум всегда находится на вершине `minStack` и читается за `O(1)`.

---

## Выводы

В ходе лабораторной работы без использования контейнеров STL реализованы четыре структуры данных: `Stack`, `Queue` (на кольцевом буфере), `Deque` (на двусвязном списке) и `PriorityQueue` (на двоичной куче `max-heap`). Для всех структур реализованы освобождение памяти в деструкторе и перехват ошибок при обращении к пустому контейнеру, а также решены все четыре задачи варианта 1: проверка скобок, моделирование банковской очереди, проверка палиндромов и поиск `K` наибольших элементов массива.

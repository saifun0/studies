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

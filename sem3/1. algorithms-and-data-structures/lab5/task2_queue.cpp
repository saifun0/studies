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

#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <memory>

using namespace std;

class Room {
private:
    string name;
    double area;

public:
    Room(const string& n, double a) : name(n), area(a) {
        cout << "Комната " << name << " создана." << endl;
    }

    ~Room() {
        cout << "Комната " << name << " удалена." << endl;
    }

    void print() const {
        cout << "Комната " << name << ", площадь: " << area << endl;
    }
};

class House {
private:
    string address;
    vector<unique_ptr<Room>> rooms;

public:
    House(const string& n) : address(n) {
        cout << "Дом " << address << " создан." << endl;
    }

    ~House() {
        cout << "Дом " << address << " удален." << endl;
    }

    void addRoom(const string& roomName, double roomArea) {
        rooms.push_back(make_unique<Room>(roomName, roomArea));
    }

    void print() const {
        cout << "Дом " << address << ":" << endl;
        for (const auto& room : rooms) {
            if (room != nullptr) room->print();
        }
    }
};

int main() {
    SetConsoleOutputCP(CP_UTF8);

    {
        House zeleniy("Зеленый");
        zeleniy.addRoom("Спальня", 15.1);
        zeleniy.addRoom("Гостиная", 10.1);
        zeleniy.addRoom("Кухня", 5.1);
        zeleniy.print();
    }
    cout << "--- После блока дом уже снесён ---" << endl;
    return 0;
}
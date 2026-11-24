#include <iostream>
#include <windows.h>
#include <string>
#include <vector>

using namespace std;

// Класс "Часть": книга существует независимо от библиотеки
class Book {
private:
    string title;
    string author;
    int year;
    static int bookCount; // общий счетчик всех созданных книг

public:
    Book(const string& t, const string& a, int y) : title(t), author(a), year(y) {
        bookCount++;
        cout << "Книга " << title << " была создана." << endl;
    }

    ~Book() {
        bookCount--;
        cout << "Книга " << title << " была удалена." << endl;
    }

    // Статический метод доступен без создания объекта Book
    static int getBookCount() {
        return bookCount;
    }

    string getTitle() const {
        return title;
    }

    void print() const {
        cout << "Книга " << title << ". " << author << ", " << year << " г." << endl;
    }
};

// Определение статического поля вне класса
int Book::bookCount = 0;

// Класс "Целое": библиотека хранит указатели на внешние книги (агрегация)
class Library {
private:
    string name;
    vector<Book*> books; // сырые указатели: библиотека не владеет книгами

public:
    Library(const string& n) : name(n) {
        cout << "Библиотека " << name << " создана." << endl;
    }

    // При закрытии библиотеки сами книги не удаляются
    ~Library() {
        cout << "Библиотека " << name << " закрыта." << endl;
    }

    void addBook(Book* book) {
        // Защита от пустого указателя
        if (book != nullptr) {
            books.push_back(book);
            cout << "Книга " << book->getTitle() << " добавлена." << endl;
        } else {
            cout << "Пустой адрес!" << endl;
        }
    }

    void print() const {
        cout << "Библиотека " << name << ": " << endl;
        for (const auto* book : books) {
            if (book != nullptr) {
                book->print();
            }
        }
    }
};

int main() {
    SetConsoleOutputCP(CP_UTF8); // включаем UTF-8 в консоли Windows
    
    cout << "Сейчас " << Book::getBookCount() << " книг." << endl;

    // Книги создаются независимо от библиотеки
    Book b1("Земля", "Автор А. А.", 1999);
    Book b2("Марс", "Иванов И. И.", 1999);
    cout << "Сейчас " << Book::getBookCount() << " книг." << endl;

    // Библиотека живет только внутри этого блока
    {
        Library lib("Городская");

        lib.addBook(&b1);
        lib.addBook(&b2);
        lib.addBook(nullptr); // проверка обработки nullptr
        lib.print();
    }

    // После закрытия библиотеки книги все еще существуют
    cout << "Сейчас " << Book::getBookCount() << " книг." << endl;
    b1.print();

    return 0;
}
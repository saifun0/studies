#include <iostream>
#include <windows.h>
#include <string>
#include <vector>

using namespace std;

class Book {
private:
    string title;
    string author;
    int year;
    static int bookCount;

public:
    Book(const string& t, const string& a, int y) : title(t), author(a), year(y) {
        bookCount++;
        cout << "Книга " << title << " была создана." << endl;
    }
    ~Book() {
        bookCount--;
        cout << "Книга " << title << " была удалена." << endl;
    }
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

int Book::bookCount = 0;

class Library {
private:
    string name;
    vector<Book*> books;

public:
    Library(const string& n) : name(n) {
        cout << "Библиотека " << name << " создана." << endl;
    }

    ~Library() {
        cout << "Библиотека " << name << " закрыта." << endl;
    }

    void addBook(Book* book) {
        if (book != nullptr) {
            books.push_back(book);
            cout << "Книга " << book->getTitle() << " добавлена." << endl;
        } else cout << "Пустой адрес!\n"; return;
    }

    void print() const {
        cout << "Библиотека " << name << ": " << endl;
        for (const auto* book : books) {
            book->print();
        }
    }

};

int main() {
    SetConsoleOutputCP(CP_UTF8);
    
    cout << "Сейчас " << Book::getBookCount() << " книг." << endl;
    Book b1("Земля", "Автор А. А.", 1999);
    Book b2("Марс", "Иванов И. И.", 1999);
    cout << "Сейчас " << Book::getBookCount() << " книг." << endl;
    {
        Library lib("Городская");

        lib.addBook(&b1);
        lib.addBook(&b2);
        lib.addBook(nullptr);
        lib.print();
    }

    cout << "Сейчас " << Book::getBookCount() << " книг." << endl;
    b1.print();

    return 0;
}
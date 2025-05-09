#include <iostream>
#include <exception>
#include "Catalog.h"
#include <locale.h>


using namespace std;
using Katalog::Catalog;
using Katalog::Book;

bool login() {
    string u, p;
    cout << "Kullanıcı: "; cin >> u;
    cout << "Şifre   : "; cin >> p;
    return u == "admin" && p == "admin";
}

int main() {
    setlocale(LC_ALL, "Turkish");
    Catalog catalog;
    try {
        catalog.loadFromFile("data/books.txt");
    }
    catch (const exception& ex) {
        cerr << "Başlatma hatası: " << ex.what() << "\n";
        return 1;
    }

    bool isAdmin = login();
    cout << (isAdmin ? "Admin modunda" : "Misafir modunda") << " devam ediliyor.\n";

    int ch;
    do {
        cout << "\n--- Menü ---\n";
        if (isAdmin) cout << "1. Kitap Ekle\n2. Kitap Sil\n";
        cout << "3. Listele\n4. ID Ara\n5. Yazar Ara\n"
            "6. ISBN Ara\n7. Başlık Ara\n8. Kategori Ara\n0. Çıkış\n"
            "Seçim: ";
        cin >> ch; cin.ignore();

        try {
            if (isAdmin && ch == 1) {
                Book b;
                cout << "ID: "; cin >> b.id; cin.ignore();
                cout << "Başlık: "; getline(cin, b.title);
                cout << "Yazar: ";  getline(cin, b.author);
                cout << "ISBN: ";   getline(cin, b.isbn);
                cout << "Kategori: ";getline(cin, b.category);
                catalog.addBook(b);

            }
            else if (isAdmin && ch == 2) {
                int id; cout << "Silinecek ID: "; cin >> id;
                catalog.deleteBook(id);

            }
            else if (ch == 3) {
                catalog.listBooks();

            }
            else if (ch == 4) {
                int id; cout << "ID: "; cin >> id;
                catalog.searchByID(id);

            }
            else if (ch == 5) {
                string s; cout << "Yazar: "; getline(cin, s);
                catalog.searchByAuthor(s);

            }
            else if (ch == 6) {
                string s; cout << "ISBN: "; getline(cin, s);
                catalog.searchByISBN(s);

            }
            else if (ch == 7) {
                string s; cout << "Başlık: "; getline(cin, s);
                catalog.searchByTitle(s);

            }
            else if (ch == 8) {
                string s; cout << "Kategori: "; getline(cin, s);
                catalog.searchByCategory(s);

            }
            else if (ch != 0) {
                cout << "Geçersiz seçim.\n";
            }
        }
        catch (const exception& ex) {
            cout << "Hata: " << ex.what() << "\n";
        }

    } while (ch != 0);

    try {
        catalog.saveToFile("data/books.txt");
        cout << "Veriler kaydedildi.\n";
    }
    catch (const exception& ex) {
        cerr << "Kaydetme hatası: " << ex.what() << "\n";
        return 1;
    }
    return 0;
}

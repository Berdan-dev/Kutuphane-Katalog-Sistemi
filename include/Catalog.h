#ifndef CATALOG_H
#define CATALOG_H

#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <stack>
#include <stdexcept>

namespace Katalog {

    struct Book {
        int id;
        std::string title;
        std::string author;
        std::string isbn;
        std::string category;
    };

    class Catalog {
    public:
        // Dosya işlemleri
        void loadFromFile(const std::string& filename);
        void saveToFile(const std::string& filename) const;

        // Veri ekleme / silme
        void addBook(const Book& b);
        void deleteBook(int id);

        // Arama fonksiyonları
        void searchByID(int id) const;
        void searchByAuthor(const std::string& author) const;
        void searchByISBN(const std::string& isbn) const;
        void searchByTitle(const std::string& title) const;
        void searchByCategory(const std::string& category) const;

        // Listeleme
        void listBooks() const;

    private:
        std::vector<Book> books_;
        std::unordered_map<std::string, size_t> isbnIndex_;
        std::multimap<std::string, size_t> titleIndex_;
        std::unordered_map<std::string, std::vector<size_t>> categoryIndex_;
        void rebuildIndices();
    };

}

#endif


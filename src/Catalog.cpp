#include "Catalog.h"
#include <fstream>
#include <iostream>
#include <algorithm>

namespace Katalog {

    void Catalog::loadFromFile(const std::string& filename) {
        std::ifstream fin(filename);
        if (!fin.is_open())
            throw std::runtime_error("Dosya açýlamadý: " + filename);

        books_.clear();
        Book b;
        while (fin >> b.id) {
            fin.ignore();
            std::getline(fin, b.title);
            std::getline(fin, b.author);
            std::getline(fin, b.isbn);
            std::getline(fin, b.category);
            books_.push_back(b);
        }
        if (fin.bad())
            throw std::runtime_error("Dosya okuma hatasý: " + filename);
        fin.close();
        rebuildIndices();
    }

    void Catalog::saveToFile(const std::string& filename) const {
        std::ofstream fout(filename);
        if (!fout.is_open())
            throw std::runtime_error("Dosya yazma açýlamadý: " + filename);

        for (auto& b : books_) {
            fout << b.id << "\n"
                << b.title << "\n"
                << b.author << "\n"
                << b.isbn << "\n"
                << b.category << "\n";
            if (fout.fail())
                throw std::runtime_error("Dosya yazma hatasý: " + filename);
        }
    }

    void Catalog::addBook(const Book& b) {
        if (isbnIndex_.count(b.isbn))
            throw std::runtime_error("Bu ISBN zaten mevcut: " + b.isbn);
        books_.push_back(b);
        rebuildIndices();
    }

    void Catalog::deleteBook(int id) {
        auto it = std::remove_if(books_.begin(), books_.end(),
            [&](const Book& x) { return x.id == id; });
        if (it == books_.end())
            throw std::runtime_error("Silinecek ID bulunamadý: " + std::to_string(id));
        books_.erase(it, books_.end());
        rebuildIndices();
    }

    void Catalog::listBooks() const {
        if (books_.empty()) {
            std::cout << "Katalog boþ.\n";
            return;
        }
        for (auto& b : books_) {
            std::cout << "[" << b.id << "] " << b.title
                << " — " << b.author
                << " (ISBN: " << b.isbn
                << ", Kategori: " << b.category << ")\n";
        }
    }

    void Catalog::searchByID(int id) const {
        auto sorted = books_;
        std::sort(sorted.begin(), sorted.end(),
            [](auto& a, auto& b) { return a.id < b.id; });
        auto it = std::lower_bound(sorted.begin(), sorted.end(), id,
            [](auto& a, int v) { return a.id < v; });
        if (it != sorted.end() && it->id == id)
            std::cout << "Bulundu: [" << it->id << "] " << it->title << "\n";
        else
            std::cout << "ID bulunamadý: " << id << "\n";
    }

    void Catalog::searchByAuthor(const std::string& author) const {
        bool found = false;
        for (auto& b : books_) {
            if (b.author.find(author) != std::string::npos) {
                std::cout << "[" << b.id << "] " << b.title << " — " << b.author << "\n";
                found = true;
            }
        }
        if (!found) std::cout << "Yazar bulunamadý: " << author << "\n";
    }

    void Catalog::searchByISBN(const std::string& isbn) const {
        auto it = isbnIndex_.find(isbn);
        if (it == isbnIndex_.end())
            std::cout << "ISBN bulunamadý: " << isbn << "\n";
        else {
            auto& b = books_[it->second];
            std::cout << "Bulundu: [" << b.id << "] " << b.title << " — " << b.author << "\n";
        }
    }

    void Catalog::searchByTitle(const std::string& title) const {
        auto range = titleIndex_.equal_range(title);
        bool found = false;
        for (auto it = range.first; it != range.second; ++it) {
            auto& b = books_[it->second];
            std::cout << "[" << b.id << "] " << b.title << " — " << b.author << "\n";
            found = true;
        }
        if (!found) std::cout << "Baþlýk bulunamadý: " << title << "\n";
    }

    void Catalog::searchByCategory(const std::string& category) const {
        std::stack<std::string> stk;
        stk.push(category);
        bool found = false;
        while (!stk.empty()) {
            auto cat = stk.top(); stk.pop();
            auto it = categoryIndex_.find(cat);
            if (it != categoryIndex_.end()) {
                for (auto idx : it->second) {
                    auto& b = books_[idx];
                    std::cout << "[" << b.id << "] " << b.title << " — " << b.author
                        << " (Kategori: " << b.category << ")\n";
                    found = true;
                }
            }
        }
        if (!found) std::cout << "Kategori bulunamadý: " << category << "\n";
    }

    void Catalog::rebuildIndices() {
        isbnIndex_.clear();
        titleIndex_.clear();
        categoryIndex_.clear();
        for (size_t i = 0; i < books_.size(); ++i) {
            auto& b = books_[i];
            isbnIndex_[b.isbn] = i;
            titleIndex_.insert({ b.title, i });
            categoryIndex_[b.category].push_back(i);
        }
    }

} // namespace Katalog

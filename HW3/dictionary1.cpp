//Author: Thomas Vu and Nihal Abdul Muneer
// takes command line arguements - [path to books folder]
//outputs top 300 most frequent words,dictionary size and time to completion


#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <filesystem>
#include <thread>
#include <chrono>
using namespace std;


struct word_info {
    uint64_t count; // total number of times the word was found
    uint32_t in_books; // in how many books
    int32_t last_book; // number of last book it was found in
};
// in_book = 0 , count = 0,last_book: -1
// found "the"  in_books = 1  count = 1 last_book = 1
// found "the"  in_books: 2  last_book = 1, count = 2 last_book = 2
class Dict {
private:
    unordered_map<string, word_info> dict;

    void merge(vector<pair<string, word_info>>& vec, int left, int mid, int right) const {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        vector<pair<string, word_info>> L(vec.begin() + left, vec.begin() + mid + 1);
        vector<pair<string, word_info>> R(vec.begin() + mid + 1, vec.begin() + right + 1);

        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2) {
            if (L[i].second.count >= R[j].second.count) {
                vec[k++] = L[i++];
            } else {
                vec[k++] = R[j++];
            }
        }

        while (i < n1) {
            vec[k++] = L[i++];
        }

        while (j < n2) {
            vec[k++] = R[j++];
        }
    }

    void merge_sort(vector<pair<string, word_info>>& vec, int left, int right) const {
        if (left < right) {
            int mid = left + (right - left) / 2;

            // Limit depth of threading to avoid excessive thread creation
            if (right - left < 1000) {
                // Perform sequential merge sort for small segments
                merge_sort(vec, left, mid);
                merge_sort(vec, mid + 1, right);
            } else {
                // Create threads for parallel sorting
                std::thread t1(&Dict::merge_sort, this, std::ref(vec), left, mid);
                std::thread t2(&Dict::merge_sort, this, std::ref(vec), mid + 1, right);

                // Wait for threads to complete
                t1.join();
                t2.join();
            }
            merge(vec, left, mid, right);
        }
    }

public:
    Dict() {}

    void add_word(const string& word, int book) {

        // Adds a new entry if the word is not in dictionary
        if (dict.find(word) == dict.end()) {
            dict[word] = { 1, 1, book };
        }
        else {
            // adds word into count and updates last book if word is already in dictionary
            dict[word].count++;
            if (book > dict[word].last_book)
                dict[word].last_book = book;        }
    }

    //prints all entries
    void print() const {
        for (const auto& entry : dict) {
            cout << "Word: " << entry.first
                 << ", Count: " << entry.second.count
                 << ", In Books: " << entry.second.in_books
                 << ", Last Book: " << entry.second.last_book
                 << endl;
        }
    }
    
    //finds a word and prints it
    void print(const string& word) const {
        auto it = dict.find(word);
        if (it != dict.end()) {
            cout << "Word: " << it->first
                 << ", Count: " << it->second.count
                 << ", In Books: " << it->second.in_books
                 << ", Last Book: " << it->second.last_book
                 << endl;
        } else {
            cout << "Word \"" << word << "\" not found in dictionary." << endl;
        }
    }

    //removes entries with one count
    void singleDelete() {
        for (auto it = dict.begin(); it != dict.end(); ) {
            if (it->second.count == 1) {
                it = dict.erase(it);  // Erase and move to next element
            } else {
                ++it;  // Move to next element
            }
        }
    }
    
    //prints the top n words
    void topPrint(int n) const {
        // Create a vector of pairs to sort by count
        vector<pair<string, word_info>> sorted_words(dict.begin(), dict.end());

        // Sort the vector by count in descending order
        merge_sort(sorted_words, 0, sorted_words.size() - 1);

        // Print the top n words
        for (int i = 0; i < n && i < sorted_words.size(); ++i) {
            cout << "Word: " << sorted_words[i].first
                 << ", Count: " << sorted_words[i].second.count
                 << ", In Books: " << sorted_words[i].second.in_books
                 << ", Last Book: " << sorted_words[i].second.last_book
                 << endl;
        }
    }
    
    void outPrint(int n){
        singleDelete();
        topPrint(n);
        cout << "Dictionary size " << dict.size() << endl;
    }
};


namespace fs = std::filesystem;

// open a single dictionary
Dict d;

void openfile(const fs::path& path, int book_num) {

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return;
    }
    // for each word in the file, lower case it and add it to the dictionary

    cout <<"in book : " << book_num << endl;
    string word;
    while (file >> word) {
        // hyph-enated
        // Chrises'
        //  132nd 
        transform(word.begin(), word.end(), word.begin(), [](unsigned char c) { return std::tolower(c); });

        d.add_word(word, book_num);

    }
}


int main(int argc, char* argv[]) {
    string path = argv[1];

    int book_num = 0;
    auto t0 = std::chrono::high_resolution_clock::now();

    try {
        for (const auto& entry : fs::directory_iterator(path)) {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                std::cout << "Found .txt file: " << entry.path().filename() << std::endl;
                openfile(entry.path(), ++book_num);
            }
        }
    } catch (const fs::filesystem_error& err) {
        std::cerr << "Filesystem error: " << err.what() << std::endl;
    }

    d.outPrint(300);
    auto t1 = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0);
        cout << "single dictionary time : " << elapsed.count() << " usec \n" ;
    return 0;
}
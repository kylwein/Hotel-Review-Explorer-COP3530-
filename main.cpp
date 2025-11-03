#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <ostream>
#include <map>
#include <limits>
#include <algorithm>
// #include "HeapSort.h"
#include "Trie.h"
using namespace std;

struct DataForHeap {
    string address;
    float average_rating;
    float lowest_rating;
    float highest_rating;
    string lowest_rating_review;
    string highest_rating_review;
};

int main() {
    ifstream file("../Hotel_Reviews.csv");
    ofstream fout("Concise_Hotel_Reviews.csv", ios::out | ios::app);

    if (!file.is_open()) {
        cout << "File could not be opened" << endl;
        return 1;
    }

    string line;
    map<string, DataForHeap> data;

    while (getline(file, line)) {
        stringstream ss(line);
        vector<string> columns;
        string cell;

        while (getline(ss, cell, ',')) {
            columns.push_back(cell);
        }

        if (columns.size() < 13) continue; // added to avoid index pain

        string hotel_name = columns[4];
        string hotel_address = columns[0];
        float avg_rating;
        float ind_rating;
        string low_rating_review = columns[6];
        string high_rating_review = columns[9];

        // lowercase + trim for Trie consistency
        auto trim = [](string& s) {
            s.erase(0, s.find_first_not_of(" \t\n\r"));
            s.erase(s.find_last_not_of(" \t\n\r") + 1);
        };
        transform(hotel_name.begin(), hotel_name.end(), hotel_name.begin(), ::tolower);
        trim(hotel_name);

        if (hotel_name.empty() || hotel_name == "hotel_name") continue;

        try {
            avg_rating = stof(columns[3]);
            ind_rating = stof(columns[12]);
        }
        catch (invalid_argument&) {
            continue;
        }

        if (data.find(hotel_name) == data.end()) {
            DataForHeap heapData;
            heapData.address = hotel_address;
            heapData.average_rating = avg_rating;
            heapData.lowest_rating = ind_rating;
            heapData.lowest_rating_review = low_rating_review;
            heapData.highest_rating = ind_rating;
            heapData.highest_rating_review = high_rating_review;
            data[hotel_name] = heapData;
        }
        else {
            if (ind_rating < data[hotel_name].lowest_rating) {
                data[hotel_name].lowest_rating = ind_rating;
                data[hotel_name].lowest_rating_review = low_rating_review;
            }
            if (ind_rating > data[hotel_name].highest_rating) {
                data[hotel_name].highest_rating = ind_rating;
                data[hotel_name].highest_rating_review = high_rating_review;
            }
        }
    }

    cout << "Finished loading. Total unique hotels: " << data.size() << endl;

  /* commented out for testing DIDNT TOUCH
    HeapSort sorterAvg;
    int max_heap_size_avg = 60000;
    pair<float, string>* average_Heap = new pair<float, string>[max_heap_size_avg];
    int average_Heap_size = 0;

    for (const auto& entry : data) {
        pair<float, string> temp = {entry.second.average_rating, entry.first};
        sorterAvg.insertNodeMax(average_Heap, average_Heap_size, temp);
    }
    cout << "\nTop 3 Hotels by Average Rating:\n";
    for (int i = 0; i < 3 && average_Heap_size > 0; ++i) {
        pair<float, string> max = sorterAvg.extractMax(average_Heap, average_Heap_size);
        cout << " - " << max.second << ", Average rating: " << max.first << endl;
    }
    delete[] average_Heap;

    HeapSort sorterBest;
    int max_heap_size_best = 60000;
    pair<float, string>* best_Heap = new pair<float, string>[max_heap_size_best];
    int best_Heap_size = 0;

    for (const auto& entry : data) {
        pair<float, string> tempBest = {entry.second.highest_rating, entry.first};
        sorterBest.insertNodeMax(best_Heap, best_Heap_size, tempBest);
    }
    cout << "\nTop 3 Hotels by Highest Rating:\n";
    for (int i = 0; i < 3 && best_Heap_size > 0; ++i) {
        pair<float, string> maxBest = sorterBest.extractMax(best_Heap, best_Heap_size);
        cout << " - " << maxBest.second << ", Best rating: " << maxBest.first << endl;
    }
    delete[] best_Heap;

    HeapSort sorterWorst;
    int min_heap_size_worst = 60000;
    pair<float, string>* worst_Heap = new pair<float, string>[min_heap_size_worst];
    int worst_Heap_size = 0;

    for (const auto& entry : data) {
        pair<float, string> tempWorst = {entry.second.lowest_rating, entry.first};
        sorterWorst.insertNodeMin(worst_Heap, worst_Heap_size, tempWorst);
    }
    cout << "\nBottom 3 Hotels by Lowest Rating:\n";
    for (int i = 0; i < 3 && worst_Heap_size > 0; ++i) {
        pair<float, string> minWorst = sorterWorst.extractMin(worst_Heap, worst_Heap_size);
        cout << " - " << minWorst.second << ", Worst rating: " << minWorst.first << endl;
    }
    delete[] worst_Heap;
*/


  // TRIES
    Trie trie;

    for (const auto& entry : data) {
        trie.insert(entry.first);
    }

    cout << "\nHotel Search (Trie Autocomplete)\n";
    string prefix;
    while (true) {
        cout << "\nType a hotel name prefix (or 'exit' to quit): ";
        getline(cin, prefix);
        if (prefix == "exit") break;

        transform(prefix.begin(), prefix.end(), prefix.begin(), ::tolower);
        vector<string> matches = trie.autocomplete(prefix);

        if (matches.empty()) {
            cout << "No matches found.\n";
        } else {
            sort(matches.begin(), matches.end());
            cout << "Top matches (" << min((int)matches.size(), 5) << "):\n";
            for (int i = 0; i < min((int)matches.size(), 5); i++) {
                cout << " - " << matches[i] << endl;
            }
        }
    }

    file.close();
    return 0;
}

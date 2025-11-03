#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <ostream>
#include <map>
#include <limits>
#include "HeapSort.h"
using namespace std;

struct DataForHeap {
    string address;
    float average_rating;
    float lowest_rating;
    float highest_rating;
    string lowest_rating_review;
    string highest_rating_review;
};

// Use to read the csv file and break into usable
// Concise_Hotel_Review csv created just to confirm it is gathering the data properly
int main() {
    ifstream file("Hotel_Reviews.csv");
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

        string hotel_name = columns[4];
        string hotel_address = columns[0];
        float avg_rating;
        float ind_rating;
        string low_rating_review = columns[6];
        string high_rating_review = columns[9];

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

    HeapSort sorterAvg;
    int max_heap_size_avg = 60000;
    pair<float, string>* average_Heap = new pair<float, string>[max_heap_size_avg];
    int average_Heap_size = 0;

    // Insert average reviews into a max heap
    for (const auto& entry : data) {
        pair<float, string> temp = {entry.second.average_rating, entry.first};
        sorterAvg.insertNodeMax(average_Heap, average_Heap_size, temp);
    }
    for (int i = 0; i < 3 && average_Heap_size > 0; ++i) {
        pair<float, string> max = sorterAvg.extractMax(average_Heap, average_Heap_size);
        cout << max.second << ", Average rating: " << max.first << endl;
    }
    delete[] average_Heap;


    // Insert highest rated reviews for all hotels into a max heap
    HeapSort sorterBest;
    int max_heap_size_best = 60000;
    pair<float, string>* best_Heap = new pair<float, string>[max_heap_size_best];
    int best_Heap_size = 0;

    for (const auto& entry : data) {
        pair<float, string> tempBest = {entry.second.highest_rating, entry.first};
        sorterBest.insertNodeMax(best_Heap, best_Heap_size, tempBest);
    }
    for (int i = 0; i < 3 && best_Heap_size > 0; ++i) {
        pair<float, string> maxBest = sorterBest.extractMax(best_Heap, best_Heap_size);
        cout << maxBest.second << ", Best ratings: " << maxBest.first << endl;
    }
    delete[] best_Heap;


    // Insert lowest rated reviews for all hotels into a min heap
    HeapSort sorterWorst;
    int min_heap_size_worst = 60000;
    pair<float, string>* worst_Heap = new pair<float, string>[min_heap_size_worst];
    int worst_Heap_size = 0;

    for (const auto& entry : data) {
        pair<float, string> tempWorst = {entry.second.lowest_rating, entry.first};
        sorterWorst.insertNodeMin(worst_Heap, worst_Heap_size, tempWorst);
    }
    for (int i = 0; i < 3 && worst_Heap_size > 0; ++i) {
        pair<float, string> maxWorst = sorterWorst.extractMin(worst_Heap, worst_Heap_size);
        cout << maxWorst.second << ", Best ratings: " << maxWorst.first << endl;
    }
    delete[] worst_Heap;


    file.close();

    return 0;
}
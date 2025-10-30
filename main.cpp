#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <ostream>
#include <map>
#include <limits>
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
        catch (...) {
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

    file.close();

    for (const auto& entry : data) {
        fout << entry.first << " " << entry.second.address << ", " << entry.second.average_rating << ", " << entry.second.lowest_rating << ", " << entry.second.lowest_rating_review << ", " << entry.second.highest_rating << ", " << entry.second.highest_rating_review << endl;
    }

    fout.close();
    return 0;
}
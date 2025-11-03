#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <ostream>
#include <map>
#include <limits>
#include <algorithm>
#include "HeapSort.h"
#include "Trie.h"
using namespace std;

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <stdio.h>


struct DataForHeap {
    string address;
    float average_rating;
    float lowest_rating;
    float highest_rating;
    string lowest_rating_review;
    string highest_rating_review;
};

int main() {
    ifstream file("../Hotel_Reviews.csv");  // changed to unify path
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

        // skip malformed rows
        if (columns.size() < 13) continue;

        string hotel_name = columns[4];
        string hotel_address = columns[0];
        float avg_rating;
        float ind_rating;
        string low_rating_review = columns[6];
        string high_rating_review = columns[9];

        // normalize names for Trie consistency
        auto trim = [](string &s) {
            s.erase(0, s.find_first_not_of(" \t\n\r"));
            s.erase(s.find_last_not_of(" \t\n\r") + 1);
        };
        transform(hotel_name.begin(), hotel_name.end(), hotel_name.begin(), ::tolower);
        trim(hotel_name);

        if (hotel_name.empty() || hotel_name == "hotel_name") continue;

        try {
            avg_rating = stof(columns[3]);
            ind_rating = stof(columns[12]);
        } catch (invalid_argument &) {
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
        } else {
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

    cout << "Loading Complete! Total unique hotels: " << data.size() << endl;

    // HEAPSORT SECTION
    HeapSort sorterAvg;
    int max_heap_size_avg = 60000;
    pair<float, string> *average_Heap = new pair<float, string>[max_heap_size_avg];
    int average_Heap_size = 0;

    for (const auto &entry : data) {
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
    pair<float, string> *best_Heap = new pair<float, string>[max_heap_size_best];
    int best_Heap_size = 0;

    for (const auto &entry : data) {
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
    pair<float, string> *worst_Heap = new pair<float, string>[min_heap_size_worst];
    int worst_Heap_size = 0;

    for (const auto &entry : data) {
        pair<float, string> tempWorst = {entry.second.lowest_rating, entry.first};
        sorterWorst.insertNodeMin(worst_Heap, worst_Heap_size, tempWorst);
    }
    cout << "\nBottom 3 Hotels by Lowest Rating:\n";
    for (int i = 0; i < 3 && worst_Heap_size > 0; ++i) {
        pair<float, string> minWorst = sorterWorst.extractMin(worst_Heap, worst_Heap_size);
        cout << " - " << minWorst.second << ", Worst rating: " << minWorst.first << endl;
    }
    delete[] worst_Heap;

    // TRIE SECTION
    Trie trie;
    for (const auto &entry : data) {
        trie.insert(entry.first);
    }
    /* COMMENTED FOR GUI TESTING
    cout << "\nHotel Search\n";
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
     */

    file.close();

    // GUI STUFF

    if (!glfwInit()) {
        cerr << "Failed to initialize GLFW" << endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(900, 700, "Hotel Review Explorer", nullptr, nullptr);
    if (window == nullptr) {
        cerr << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

// Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

// GUI variables
    char searchBuf[128] = "";
    vector<string> guiResults;
    string selectedHotel = "";

// Prepare top 10 from heaps
    vector<pair<float, string>> topAvg;
    vector<pair<float, string>> topHigh;
    vector<pair<float, string>> topLow;

    {
        // rebuild heaps quickly
        HeapSort sorterAvg;
        int size = 0;
        pair<float, string>* heap = new pair<float, string>[data.size() + 1];
        for (auto& e : data) sorterAvg.insertNodeMax(heap, size, {e.second.average_rating, e.first});
        for (int i = 0; i < 10 && size > 0; i++) topAvg.push_back(sorterAvg.extractMax(heap, size));
        delete[] heap;

        HeapSort sorterBest;
        size = 0;
        heap = new pair<float, string>[data.size() + 1];
        for (auto& e : data) sorterBest.insertNodeMax(heap, size, {e.second.highest_rating, e.first});
        for (int i = 0; i < 10 && size > 0; i++) topHigh.push_back(sorterBest.extractMax(heap, size));
        delete[] heap;

        HeapSort sorterWorst;
        size = 0;
        heap = new pair<float, string>[data.size() + 1];
        for (auto& e : data) sorterWorst.insertNodeMin(heap, size, {e.second.lowest_rating, e.first});
        for (int i = 0; i < 10 && size > 0; i++) topLow.push_back(sorterWorst.extractMin(heap, size));
        delete[] heap;
    }

// === MAIN GUI LOOP ===
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Hotel Review Explorer");
        ImGui::SetWindowSize(ImVec2(850, 650), ImGuiCond_Once);
        ImGui::Text("Welcome! Search for a hotel or browse rankings.");
        ImGui::Separator();

        // Tabs: Search | Top Avg | Top Highest | Top Lowest
        if (ImGui::BeginTabBar("HotelTabs")) {

            // SEARCH TAB
            if (ImGui::BeginTabItem("Search")) {
                ImGui::InputText("##searchBar", searchBuf, IM_ARRAYSIZE(searchBuf));
                ImGui::SameLine();
                if (ImGui::Button("Search")) {
                    string query(searchBuf);
                    transform(query.begin(), query.end(), query.begin(), ::tolower);
                    guiResults = trie.autocomplete(query);
                }

                ImGui::Separator();
                if (!guiResults.empty()) {
                    ImGui::Text("Top matches:");
                    for (int i = 0; i < std::min((int)guiResults.size(), 10); ++i) {
                        if (ImGui::Selectable(guiResults[i].c_str(), selectedHotel == guiResults[i])) {
                            selectedHotel = guiResults[i];
                        }
                    }
                } else {
                    ImGui::TextDisabled("No matches yet. Type something and click Search.");
                }
                ImGui::EndTabItem();
            }

            // TOP AVG TAB
            if (ImGui::BeginTabItem("Top Avg Rated")) {
                ImGui::Text("Top 10 Hotels by Average Rating:");
                for (auto& p : topAvg) {
                    string label = p.second + " (" + to_string(p.first).substr(0, 4) + ")";
                    if (ImGui::Selectable(label.c_str(), selectedHotel == p.second))
                        selectedHotel = p.second;
                }
                ImGui::EndTabItem();
            }

            // TOP HIGHEST TAB
            if (ImGui::BeginTabItem("Top Highest Rated")) {
                ImGui::Text("Top 10 Highest Ratings:");
                for (auto& p : topHigh) {
                    string label = p.second + " (" + to_string(p.first).substr(0, 4) + ")";
                    if (ImGui::Selectable(label.c_str(), selectedHotel == p.second))
                        selectedHotel = p.second;
                }
                ImGui::EndTabItem();
            }

            // TOP LOWEST TAB
            if (ImGui::BeginTabItem("Top Lowest Rated")) {
                ImGui::Text("Bottom 10 Hotels (Lowest Ratings):");
                for (auto& p : topLow) {
                    string label = p.second + " (" + to_string(p.first).substr(0, 4) + ")";
                    if (ImGui::Selectable(label.c_str(), selectedHotel == p.second))
                        selectedHotel = p.second;
                }
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::Separator();

        // DETAILS PANEL
        if (!selectedHotel.empty()) {
            auto it = data.find(selectedHotel);
            if (it != data.end()) {
                ImGui::Text("Hotel Details");
                ImGui::Separator();
                ImGui::Text("Name: %s", selectedHotel.c_str());
                ImGui::TextWrapped("Address: %s", it->second.address.c_str());
                ImGui::Text("Average Rating: %.2f", it->second.average_rating);
                ImGui::Text("Highest Rating: %.2f", it->second.highest_rating);
                ImGui::Text("Lowest Rating: %.2f", it->second.lowest_rating);
                ImGui::Separator();
                ImGui::TextWrapped("Top Review:\n%s", it->second.highest_rating_review.c_str());
                ImGui::Separator();
                ImGui::TextWrapped("Lowest Review:\n%s", it->second.lowest_rating_review.c_str());
            } else {
                ImGui::TextDisabled("No details found for this hotel.");
            }
        }

        ImGui::End();

        // Render
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

// === CLEANUP ===
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();




    return 0;
}


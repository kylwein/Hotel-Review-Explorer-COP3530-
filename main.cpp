#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

#include "HeapSort.h"
#include "Trie.h"

// GUI stuff
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

// struct for storing information for each hotel
struct DataForHeap {
    string address;
    float average_rating;
    float lowest_rating;
    float highest_rating;
    string lowest_rating_review;
    string highest_rating_review;
};

// color helper for GUI
static ImVec4 scoreColor(float score) {
    if (score >= 9.0f) return ImVec4(0.35f,0.85f,0.30f,1.0f);
    if (score >= 8.0f) return ImVec4(0.95f,0.82f,0.25f,1.0f);
    if (score >= 6.0f) return ImVec4(0.95f,0.60f,0.25f,1.0f);
    return ImVec4(0.90f,0.32f,0.30f,1.0f);
}


int main() {

    // CSV Loading
    ifstream file("../Hotel_Reviews.csv");
    if (!file.is_open()) {
        cout << "CSV Won't Open :(.\n";
        return 1;
    }

    map<string, DataForHeap> data;
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        vector<string> c;
        string cell;
        while (getline(ss, cell, ',')) c.push_back(cell);
        if (c.size() < 13) continue; // bad row

        string name = c[4];
        string address = c[0];

        // normalize hotel name for trie
        transform(name.begin(), name.end(), name.begin(), ::tolower);
        name.erase(0, name.find_first_not_of(" \t\n\r"));
        name.erase(name.find_last_not_of(" \t\n\r") + 1);

        if (name.empty() || name == "hotel_name") continue;

        float avg, indiv;
        try {
            avg = stof(c[3]);
            indiv = stof(c[12]);
        } catch(...) { continue; }

        // reviews pulled directly from CSV
        string low_r = c[6];
        string high_r = c[9];

        // if first time seeing this hotel, initalize it
        if (!data.count(name)) {
            data[name] = {address, avg, indiv, indiv, low_r, high_r};
        }
            //  update min/max ratings
        else {
            auto &h = data[name];
            if (indiv < h.lowest_rating) {
                h.lowest_rating = indiv;
                h.lowest_rating_review = low_r;
            }
            if (indiv > h.highest_rating) {
                h.highest_rating = indiv;
                h.highest_rating_review = high_r;
            }
        }
    }
    file.close();

    cout << "Loaded! " << data.size() << " unique hotels\n"; // see how many hotels in dataset

    // Heaps - Jacob
    vector<pair<float,string>> topAvg, topHigh, topLow;

    {   // top avg
        HeapSort s;
        int n=0;
        auto *heap = new pair<float,string>[data.size()+1];
        for (auto &e: data) s.insertNodeMax(heap,n,{e.second.average_rating,e.first});
        for (int i=0;i<10 && n>0;i++) topAvg.push_back(s.extractMax(heap,n));
        delete[] heap;
    }
    {   // top highest rating
        HeapSort s;
        int n=0;
        auto *heap = new pair<float,string>[data.size()+1];
        for (auto &e: data) s.insertNodeMax(heap,n,{e.second.highest_rating,e.first});
        for (int i=0;i<10 && n>0;i++) topHigh.push_back(s.extractMax(heap,n));
        delete[] heap;
    }
    {   // lowest rating
        HeapSort s;
        int n=0;
        auto *heap = new pair<float,string>[data.size()+1];
        for (auto &e: data) s.insertNodeMin(heap,n,{e.second.lowest_rating,e.first});
        for (int i=0;i<10 && n>0;i++) topLow.push_back(s.extractMin(heap,n));
        delete[] heap;
    }

    // Tries - Kyle
    Trie trie;
    for (auto &e: data) trie.insert(e.first);


    // GUI setup - Pierce
    if (!glfwInit()) return -1;
    GLFWwindow *window = glfwCreateWindow(900,700,"Hotel Review Explorer",NULL,NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window,true);
    ImGui_ImplOpenGL3_Init("#version 130");
    ImGui::StyleColorsDark();


    bool darkMode = true;
    char searchBuf[128] = "";
    vector<string> guiResults;
    string selectedHotel = "";

    enum class Mode { AVG,HIGH,LOW };
    Mode mode = Mode::AVG;

    // GUI
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Hotel Review Explorer");
        ImGui::Text("Hotel Review Explorer");

        // theme switcher
        ImGui::SameLine(ImGui::GetWindowWidth() - 140);
        if (ImGui::Button(darkMode ? "Light Mode" : "Dark Mode",ImVec2(120,25))) {
            darkMode=!darkMode;
            if(darkMode) ImGui::StyleColorsDark();
            else ImGui::StyleColorsLight();
        }

        ImGui::Separator();
        ImGui::InputTextWithHint("##search","Search hotels...",searchBuf,sizeof(searchBuf));
        ImGui::SameLine();
        if (ImGui::Button("Search",ImVec2(100,28))) {
            string q = searchBuf;
            transform(q.begin(),q.end(),q.begin(),::tolower);
            guiResults = trie.autocomplete(q);
            mode = Mode::AVG;
        }

        // Tab Bar
        if (ImGui::BeginTabBar("tabs")) {

            if (ImGui::BeginTabItem("Search")) {
                mode = Mode::AVG;
                ImGui::EndTabItem();
            }


            if (ImGui::BeginTabItem("Best Overall")) {
                guiResults.clear();
                for (auto &p: topAvg) guiResults.push_back(p.second);
                mode = Mode::AVG;
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Best Single Reviews")) {
                guiResults.clear();
                for (auto &p: topHigh) guiResults.push_back(p.second);
                mode = Mode::HIGH;
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Worst Single Reviews")) {
                guiResults.clear();
                for (auto &p: topLow) guiResults.push_back(p.second);
                mode = Mode::LOW;
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        // results list
        ImGui::Spacing();
        ImGui::BeginChild("results",ImVec2(0,330),true);

        for (auto &name : guiResults) {
            auto &h = data[name];
            float score = (mode==Mode::AVG)?h.average_rating : (mode==Mode::HIGH)?h.highest_rating : h.lowest_rating;


            // card container
            ImVec2 card(ImGui::GetContentRegionAvail().x,80);
            ImVec2 pos = ImGui::GetCursorScreenPos();
            ImVec2 pos2(pos.x+card.x,pos.y+card.y);

            bool hovered = ImGui::IsMouseHoveringRect(pos,pos2);
            bool clicked = hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left);

            if(clicked) selectedHotel = name;

            ImVec4 bg = hovered ? ImVec4(0.25f,0.35f,0.45f,1.0f) : (darkMode?ImVec4(0.13f,0.14f,0.18f,1.0f):ImVec4(0.9f,0.93f,0.96f,1.0f));

            ImGui::PushStyleColor(ImGuiCol_ChildBg,bg);
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding,10);
            ImGui::BeginChild(("card_"+name).c_str(),card,true);

            ImGui::Text("%s",name.c_str());
            ImGui::SameLine(ImGui::GetWindowWidth()-180);
            ImGui::TextColored(scoreColor(score),"%.1f / 10",score);

            ImGui::EndChild();
            ImGui::PopStyleVar();
            ImGui::PopStyleColor();
            ImGui::Spacing();
        }

        ImGui::EndChild();
        ImGui::Separator();

        // detail panel
        if (!selectedHotel.empty()) {
            auto &h = data[selectedHotel];
            ImGui::Text("Hotel Details:");
            ImGui::Text("Address: %s",h.address.c_str());
            ImGui::Text("Avg: %.2f | High: %.2f | Low: %.2f",h.average_rating,h.highest_rating,h.lowest_rating);
            ImGui::Separator();
            ImGui::TextWrapped("Highest Review:\n%s",h.highest_rating_review.c_str());
            ImGui::Separator();
            ImGui::TextWrapped("Lowest Review:\n%s",h.lowest_rating_review.c_str());
        }

        ImGui::End();
        ImGui::Render();

        int w,h;
        glfwGetFramebufferSize(window,&w,&h);
        glViewport(0,0,w,h);
        glClearColor(0.1f,0.1f,0.1f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // close everything
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

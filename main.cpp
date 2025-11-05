#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>
#include <random>
#include "Trie.h"
#include "HeapSort.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

using namespace std;

struct HotelData {
    string display_name;
    string address;
    int review_count = 0;
    float average_rating = 0.0f;
    float highest_rating = 0.0f;
    float lowest_rating = 10.0f;
    string highest_rating_review;
    string lowest_rating_review;
};

static vector<string> splitCSVLine(const string& line) {
    vector<string> out;
    string cur;
    bool in_quotes = false;
    for (size_t i = 0; i < line.size(); ++i) {
        char ch = line[i];
        if (ch == '"') {
            if (in_quotes && i + 1 < line.size() && line[i + 1] == '"') { cur.push_back('"'); ++i; }
            else { in_quotes = !in_quotes; }
        } else if (ch == ',' && !in_quotes) {
            out.push_back(cur);
            cur.clear();
        } else {
            cur.push_back(ch);
        }
    }
    out.push_back(cur);
    return out;
}

static inline string to_lower_copy(string s) {
    transform(s.begin(), s.end(), s.begin(),
              [](unsigned char c){ return static_cast<char>(tolower(c)); });
    return s;
}

inline void ApplyDarkAesthetic() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    colors[ImGuiCol_WindowBg]      = ImVec4(0.10f, 0.12f, 0.16f, 1.00f);
    colors[ImGuiCol_Text]          = ImVec4(0.90f, 0.92f, 0.95f, 1.00f);
    colors[ImGuiCol_Button]        = ImVec4(0.20f, 0.55f, 0.70f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.65f, 0.80f, 1.00f);
    colors[ImGuiCol_ButtonActive]  = ImVec4(0.25f, 0.50f, 0.65f, 1.00f);
    colors[ImGuiCol_Header]        = ImVec4(0.22f, 0.45f, 0.55f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.28f, 0.55f, 0.65f, 1.00f);
    colors[ImGuiCol_ChildBg]       = ImVec4(0.12f, 0.14f, 0.18f, 1.00f);

    style.FrameRounding = 10.0f;
    style.WindowRounding = 14.0f;
    style.ItemSpacing = ImVec2(10, 8);
    style.WindowPadding = ImVec2(16, 12);
}

inline void ApplyLightAesthetic() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    colors[ImGuiCol_WindowBg]      = ImVec4(0.97f, 0.97f, 0.96f, 1.00f);
    colors[ImGuiCol_ChildBg]       = ImVec4(0.99f, 0.99f, 0.98f, 1.00f);

    colors[ImGuiCol_Text]          = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TextDisabled]  = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);

    colors[ImGuiCol_Button]        = ImVec4(0.24f, 0.50f, 0.78f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.32f, 0.60f, 0.85f, 1.00f);
    colors[ImGuiCol_ButtonActive]  = ImVec4(0.20f, 0.45f, 0.70f, 1.00f);

    colors[ImGuiCol_Header]        = ImVec4(0.30f, 0.60f, 0.80f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.55f, 0.75f, 1.00f);
    colors[ImGuiCol_HeaderActive]  = ImVec4(0.20f, 0.45f, 0.70f, 1.00f);

    colors[ImGuiCol_FrameBg]       = ImVec4(0.95f, 0.95f, 0.94f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]= ImVec4(0.90f, 0.92f, 0.94f, 1.00f);
    colors[ImGuiCol_Border]        = ImVec4(0.70f, 0.73f, 0.77f, 1.00f);

    style.FrameRounding = 10.0f;
    style.WindowRounding = 14.0f;
    style.ItemSpacing = ImVec2(10, 8);
    style.WindowPadding = ImVec2(18, 12);
    style.ChildRounding = 10.0f;
}

static ImVec4 scoreColor(float score) {
    if (score >= 9.0f) return ImVec4(0.35f, 0.85f, 0.30f, 1.0f);
    if (score >= 8.0f) return ImVec4(0.95f, 0.82f, 0.25f, 1.0f);
    if (score >= 6.0f) return ImVec4(0.95f, 0.60f, 0.25f, 1.0f);
    return ImVec4(0.90f, 0.32f, 0.30f, 1.0f);
}

int main() {
    ifstream file("../Hotel_Reviews.csv");//change to your system
    if (!file.is_open()) {
        cerr << " Error: Could not open ../Hotel_Reviews.csv" << endl;
        return -1;
    }

    ofstream fout("Concise_Hotel_Reviews.csv", ios::out | ios::app);
    if (!fout.is_open()) {
        cerr << " Error: Could not create Concise_Hotel_Reviews.csv" << endl;
        return -1;
    }

    unsigned char bom[3] = {0};
    file.read(reinterpret_cast<char*>(bom), 3);
    if (!(bom[0] == 0xEF && bom[1] == 0xBB && bom[2] == 0xBF)) file.seekg(0);

    string header;
    getline(file, header);
    cout << "Header columns: " << header << endl;

    map<string, HotelData> hotels;
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        if (!line.empty() && line.back() == '\r') line.pop_back();
        vector<string> cols = splitCSVLine(line);
        if (cols.size() < 13) continue;

        string hotel_name = cols[4];
        if (hotel_name.empty()) continue;

        string key = to_lower_copy(hotel_name);
        string avg_str = cols[3];
        string reviewer_score_str = cols[12];

        float avg = 0.0f, reviewer_score = 0.0f;
        try { if (!avg_str.empty()) avg = stof(avg_str); } catch (...) {}
        try { if (!reviewer_score_str.empty()) reviewer_score = stof(reviewer_score_str); } catch (...) {}

        HotelData& h = hotels[key];
        if (h.display_name.empty()) h.display_name = hotel_name;
        h.review_count++;
        if (reviewer_score > h.highest_rating) h.highest_rating = reviewer_score;
        if (reviewer_score < h.lowest_rating) h.lowest_rating = reviewer_score;
        if (h.average_rating <= 0.f && avg > 0.f) h.average_rating = avg;

        fout << hotel_name << "," << h.average_rating << "," << h.review_count << "\n";
    }
    file.close();
    fout.close();
    cout << " Loaded " << hotels.size() << " unique hotels.\n";

    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(900, 700, "Hotel Review Explorer", nullptr, nullptr);
    if (!window) { glfwTerminate(); return -1; }

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if (mode) glfwSetWindowPos(window, (mode->width - 900) / 2, (mode->height - 700) / 2);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    bool darkMode = true;
    ApplyDarkAesthetic();

    static vector<string> searchResults;
    char searchBuf[128] = "";

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowSize(ImVec2(850, 650), ImGuiCond_Once);
        ImGui::Begin("Hotel Review Explorer", nullptr, ImGuiWindowFlags_NoCollapse);

        ImVec4 titleColor = darkMode ? ImVec4(0.60f, 0.80f, 1.00f, 1.00f)
                                     : ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        ImGui::TextColored(titleColor, "Hotel Review Explorer");

        ImGui::SameLine(ImGui::GetWindowWidth() - 180);
        if (ImGui::Button(darkMode ? "Switch to Light Mode" : "Switch to Dark Mode", ImVec2(160, 28))) {
            darkMode = !darkMode;
            if (darkMode) ApplyDarkAesthetic(); else ApplyLightAesthetic();
        }

        ImGui::Separator();
        ImGui::TextDisabled("Loaded: %d hotels", (int)hotels.size());
        ImGui::Separator();

        ImGui::InputTextWithHint("##search", "Enter hotel name...", searchBuf, IM_ARRAYSIZE(searchBuf));
        ImGui::SameLine();
        if (ImGui::Button("Search", ImVec2(100, 28))) {
            string q = to_lower_copy(string(searchBuf));
            searchResults.clear();
            for (auto &kv : hotels)
                if (kv.first.find(q) != string::npos)
                    searchResults.push_back(kv.first);
        }

        ImGui::SameLine();
        if (ImGui::Button("Top Rated", ImVec2(100, 28))) {
            searchResults.clear();
            vector<pair<string, float>> sorted;
            for (auto &kv : hotels) sorted.push_back({kv.first, kv.second.average_rating});
            sort(sorted.begin(), sorted.end(), [](auto &a, auto &b){ return a.second > b.second; });
            for (int i = 0; i < 10 && i < (int)sorted.size(); ++i)
                searchResults.push_back(sorted[i].first);
        }

        ImGui::SameLine();
        if (ImGui::Button("Lowest Rated", ImVec2(120, 28))) {
            searchResults.clear();
            vector<pair<string, float>> sorted;
            for (auto &kv : hotels) sorted.push_back({kv.first, kv.second.average_rating});
            sort(sorted.begin(), sorted.end(), [](auto &a, auto &b){ return a.second < b.second; });
            for (int i = 0; i < 10 && i < (int)sorted.size(); ++i)
                searchResults.push_back(sorted[i].first);
        }

        if (ImGui::Button("Show Example Hotels", ImVec2(180, 28))) {
            searchResults.clear();
            vector<string> keys;
            for (auto &kv : hotels) keys.push_back(kv.first);
            if (!keys.empty()) {
                shuffle(keys.begin(), keys.end(), std::default_random_engine((unsigned)time(nullptr)));
                for (int i = 0; i < 10 && i < (int)keys.size(); ++i)
                    searchResults.push_back(keys[i]);
            }
        }

        ImGui::Spacing();
        if (!searchResults.empty()) {
            ImGui::Text("Showing %d hotels", (int)searchResults.size());
            ImGui::BeginChild("ResultsPanel", ImVec2(0, 360), true);

            for (auto &key : searchResults) {
                const HotelData& h = hotels[key];
                ImVec4 sc = scoreColor(h.average_rating);
                ImVec4 cardBg = darkMode ? ImVec4(0.13f, 0.14f, 0.18f, 1.00f)
                                         : ImVec4(0.90f, 0.93f, 0.96f, 1.00f);
                ImVec4 textColor = darkMode ? ImVec4(1.00f, 1.00f, 1.00f, 1.00f)
                                            : ImVec4(0.00f, 0.00f, 0.00f, 1.00f);

                ImGui::PushStyleColor(ImGuiCol_ChildBg, cardBg);
                ImGui::BeginChild(("card_" + key).c_str(), ImVec2(0, 84), true);

                ImGui::PushStyleColor(ImGuiCol_Text, textColor);
                ImGui::Text("%s", h.display_name.c_str());
                ImGui::PopStyleColor();

                ImGui::SameLine(ImGui::GetWindowWidth() - 200);
                ImGui::TextColored(sc, "%.1f / 10", h.average_rating);
                ImGui::TextDisabled("Reviews: %d", h.review_count);

                ImGui::EndChild();
                ImGui::PopStyleColor();
                ImGui::Spacing();
            }

            ImGui::EndChild();
        }

        ImGui::End();
        ImGui::Render();

        int w, h; glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);
        ImVec4 bg = darkMode ? ImVec4(0.08f, 0.10f, 0.13f, 1.0f) : ImVec4(0.95f, 0.94f, 0.92f, 1.0f);
        glClearColor(bg.x, bg.y, bg.z, bg.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

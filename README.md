# Hotel Review Explorer (COP3530)

**Team Name:** The Triple Dippers  
**Team Members:** Pierce Lopez, Kyle Weiner, Jacob DeWese  

---

## 📌 Project Overview

**Hotel Review Explorer** is an interactive GUI for browsing and comparing hotels using real review data.  
We focus on efficient searching and ranking using two data structures:

- **Trie** → enables fast hotel name search with autocomplete
- **Heaps** → used to quickly retrieve top-rated and lowest-rated hotels


### What You Can Do in the GUI
- Search for hotels by name
- View each hotel's:
  - **Average Rating**
  - **Highest Rating** + corresponding review
  - **Lowest Rating** + corresponding review
- Browse rankings:
  - **Top Overall Rated Hotels**
  - **Top Highest Individual Review Scores**
  - **Top Lowest Individual Review Scores**

### Goal
- The purpose is to make hotel review data **easy to explore and compare**, providing a clean UI, quick searching, and interesting rankings.
---

## 🛠️ SETUP

This project uses **Dear ImGui** and **GLFW** for our GUI. 
Both dependencies are already included locally in the repository so you don't need to install them.

### Step 1: Clone the project
```bash
git clone https://github.com/kylwein/Hotel-Review-Explorer-COP3530-.git
cd Hotel-Review-Explorer-COP3530-
```

### Step 2: Make sure Git LFS is installed (IMPORTANT)

The hotel dataset is stored using Git LFS.  
If you skip this step, no hotels will load.

Run this once:

(Windows)
```powershell
winget install git-lfs
git lfs install
git lfs pull
```

(Mac)
```
brew install git-lfs 
git lfs install
git lfs pull
```

### Step 3: Verify Folders
Make sure your project root has these folders:
``` 
Hotel-Review-Explorer-COP3530-
├── imgui/
├── glfw/
├── main.cpp
├── Trie.cpp
├── HeapSort.cpp
└── CMakeLists.txt
```

### Step 4: Build & Run
```
1) Open the folder in IDE
2) Reload CMake Project
3) Build
4) Run (the GUI will open automatically)
```

---

## 📊 Dataset
**Source:** [Kaggle – 515k Hotel Reviews Data in Europe](https://www.kaggle.com/datasets/jiashenliu/515k-hotel-reviews-data-in-europe/data)

**Columns Include:**  
| Column | Type | Description |
|--------|------|-------------|
| Hotel_name | string | Name of the hotel |
| Hotel_address | string | Address of the hotel |
| Reviewer_nationality | string | Country of reviewer |
| Pos_review / Neg_review | string | Review text |
| Avg_score | float | Average hotel score |
| Reviewer_score | float | User’s review score |
| Total_num_of_reviews | int | Total reviews |
| Review_date | date | Date of review |
| Lat / Lng | float | Coordinates |

---

## 🧰 Tools & Frameworks
- **C++** – Core logic, data structures, GUI  
- **GitHub** – Version control and collaboration  

---

## ⚙️ Implementation
**Data Structures**
- **Trie:**  
  - Stores hotel names, tags, and keywords  
  - Enables fast prefix-based autocomplete — O(n) time, where *n* = string length  
- **Heap (Min/Max):**  
  - Ranks hotels by ratings
  - Maintains top-N efficiently in O(log n) time  

**Workflow**
1. Load and preprocess dataset  
2. Insert hotel/review data into Trie for lookup  
3. Build Heaps for ranking/filtering  
4. Integrate GUI — user searches → Trie finds matches → Heap ranks → display results  

---

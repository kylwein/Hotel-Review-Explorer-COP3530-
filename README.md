# Hotel Review Explorer (COP3530)

**Team Name:** The Triple Dippers  
**Team Members:** Pierce Lopez, Kyle Weiner, Jacob DeWese  

---

## 📌 Project Overview
The **Hotel Review Explorer** predicts and analyzes hotel review scores using textual reviews and metadata.  
Our model will:
- Predict the **numerical review score** a user might give, or  
- Classify reviews as **positive/negative** or **above/below average**  

It will also identify the most influential words, reviewer traits, and hotel attributes that correlate with high or low ratings.

---

## 💡 Motivation
Accurate review prediction gives hotels insight before feedback is even submitted.  
Platforms like **Airbnb** and **Vrbo** have changed the landscape — hotels need to adapt to shifting customer sentiment and competition.  
Our system helps them pinpoint what drives satisfaction and where to improve.

---

## 🎯 Success Criteria
We’ll consider the project successful when:

- **Regression Goal:** Low *Mean Absolute Error (MAE)* and *Root Mean Squared Error (RMSE)* compared to a baseline model.  
- **Classification Goal:** At least **80% accuracy** for positive vs. negative review prediction.  
- **Performance Goal:** Efficient ranking and visualization using Tries and Heaps.  
- **Interpretability Goal:** Clear identification of top predictive words and metadata features.

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
- **Python (optional)** – Data cleaning and visualization  
- **GitHub** – Version control and collaboration  

---

## 🧩 Planned Visuals
**Interface Features:**
- **Search Bar (Trie-based):** Autocomplete for hotel names and keywords  
- **Ranking Panel (Heap-based):**
  - Top 10 highest-rated hotels  
  - Top 10 most-reviewed hotels  
  - Top 10 positive/negative mentions  
- **Optional:** Map view with hotel locations using lat/lng data  

---

## ⚙️ Implementation Strategy
**Data Structures**
- **Trie:**  
  - Stores hotel names, tags, and keywords  
  - Enables fast prefix-based autocomplete — O(n) time, where *n* = string length  
- **Heap (Min/Max):**  
  - Ranks hotels by score or number of reviews  
  - Maintains top-N efficiently in O(log n) time  

**Workflow**
1. Load and preprocess dataset  
2. Insert hotel/review data into Trie for lookup  
3. Build Heaps for ranking/filtering  
4. Integrate GUI — user searches → Trie finds matches → Heap ranks → display results  

---

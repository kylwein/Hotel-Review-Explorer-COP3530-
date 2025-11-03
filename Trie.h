//
// Created by 19546 on 11/3/2025.
//

#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

// each node in the trie (letter)
struct TrieNode {
    bool isEnd;  // marks if it’s the end of a word
    unordered_map<char, TrieNode*> children; // map of next letters
    TrieNode() : isEnd(false) {}
};

class Trie {
private:
    TrieNode* root;

    // helper to get all words starting with a prefix
    void dfs(TrieNode* node, string prefix, vector<string>& results) {
        if (node->isEnd) results.push_back(prefix); // word complete
        for (auto& pair : node->children) {
            dfs(pair.second, prefix + pair.first, results); // go deeper
        }
    }

public:
    Trie() {
        root = new TrieNode(); // make the root node
    }

    // adds a word into the trie
    void insert(const string& word) {
        TrieNode* node = root;
        for (char c : word) {
            if (!node->children[c]) { // if that letter not seen yet
                node->children[c] = new TrieNode();
            }
            node = node->children[c]; // move to next node
        }
        node->isEnd = true; // mark end of word
    }

    // gets list of all words that start with a prefix

    vector<string> autocomplete(const string& prefix) {
        TrieNode* node = root;
        for (char c : prefix) {
            if (!node->children.count(c)) {
                return {}; // if prefix not found
            }
            node = node->children[c];
        }
        vector<string> results;
        dfs(node, prefix, results); // find all completions
        return results;
    }
};


#endif //COP3530_HOTEL_REVIEWS_PROJECT_TRIE_H

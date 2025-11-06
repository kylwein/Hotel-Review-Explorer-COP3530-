//
// Created by 19546 on 11/2/2025.
//

#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

// each node in the trie = one branch
struct TrieNode {
    bool isEnd;  // true if the path so far is an actual full word
    unordered_map<char, TrieNode*> children; // edges to next letters

    TrieNode() : isEnd(false) {} // default
};

class Trie {
private:
    TrieNode* root;

    // DFS helper: once we find the prefix node, just keep walking
    // down and collect all possible words.
    void dfs(TrieNode* node, string prefix, vector<string>& results) {
        if (node->isEnd) {
            results.push_back(prefix);
        }

        // go through every child letter
        for (auto& nxt : node->children) {
            dfs(nxt.second, prefix + nxt.first, results);
        }
    }

public:
    Trie() {
        root = new TrieNode();
    }

    // Insert a full word
    void insert(const string& word) {
        TrieNode* cur = root;

        for (char c : word) {
            if (!cur->children[c]) {
                cur->children[c] = new TrieNode();
            }
            cur = cur->children[c];
        }

        cur->isEnd = true; //  reached the end of the word
    }

    // Return all words starting with a given prefix
    vector<string> autocomplete(const string& prefix) {
        TrieNode* cur = root;

        for (char c : prefix) {
            if (!cur->children.count(c)) {
                return {};
            }
            cur = cur->children[c];
        }

        // reached prefix node
        vector<string> results;
        dfs(cur, prefix, results);

        return results;
    }
};

#endif // TRIE_H

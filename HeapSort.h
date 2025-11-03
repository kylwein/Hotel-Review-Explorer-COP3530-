//
// Created by thear on 11/1/2025.
//

#include <iostream>
using namespace std;
#pragma once

class HeapSort {
   private:
      struct Heap {
        int *array;
        int index;
        int size;
        Heap(int *arr, int ind, int s) : array(arr), index(ind), size(s) {}
      };

  public:
    void heapifyDownMax(pair<float, string> arr[], int index, int size);
    void heapifyDownMin(pair<float, string> arr[], int index, int size);
    void heapifyUpMax(pair<float, string> arr[], int size, int index);
    void heapifyUpMin(pair<float, string> arr[], int size, int index);
    pair<float, string> extractMax(pair<float, string> arr[], int &size);
    pair<float, string> extractMin(pair<float, string> arr[], int &size);
    void insertNodeMin(pair<float, string> arr[], int& size, pair<float, string> Key);
    void insertNodeMax(pair<float, string> arr[], int& size, pair<float, string> Key);
};
//
// Created by thear on 10/28/2025.
//
#include <iostream>
#include <algorithm>
#include "HeapSort.h"
using namespace std;

// Heapify function (max heap)
void HeapSort::heapifyDownMax(pair<float, string> arr[], int index, int size) {
    int largest_child_index = index;
    int leftInd = 2 * index + 1;
    int rightInd = 2 * index + 2;

    if (leftInd >= size) {
        return;
    }

    if (leftInd < size && arr[leftInd].first > arr[largest_child_index].first) {
      largest_child_index = leftInd;
    }
    if (rightInd < size && arr[rightInd].first > arr[largest_child_index].first) {
      largest_child_index = rightInd;
    }

    if (largest_child_index != index) {
        swap(arr[index], arr[largest_child_index]);
        heapifyDownMax(arr, largest_child_index, size);
    }
}

void HeapSort::heapifyDownMin(pair<float, string> arr[], int index, int size) {
    int smallest_child_index = index;
    int leftInd = 2 * index + 1;
    int rightInd = 2 * index + 2;

    if (leftInd >= size) {
        return;
    }

    if (leftInd < size && arr[leftInd].first < arr[smallest_child_index].first) {
        smallest_child_index = leftInd;
    }
    if (rightInd < size && arr[rightInd].first < arr[smallest_child_index].first) {
        smallest_child_index = rightInd;
    }

    if (smallest_child_index != index) {
        swap(arr[index], arr[smallest_child_index]);
        heapifyDownMin(arr, smallest_child_index, size);
    }
}

void HeapSort::heapifyUpMax(pair<float, string> arr[], int size, int index) {
    if (index == 0) {
        return;
    }

    int parent = (index - 1) / 2;
    if (parent >= 0) {
        if (arr[index].first > arr[parent].first) {
            swap(arr[index], arr[parent]);
            heapifyUpMax(arr, size, parent);
        }
    }
}

void HeapSort::heapifyUpMin(pair<float, string> arr[], int size, int index) {
    if (index == 0) {
        return;
    }

    int parent = (index - 1) / 2;
    if (parent >= 0) {
        if (arr[index].first < arr[parent].first) {
            swap(arr[index], arr[parent]);
            heapifyUpMin(arr, size, parent);
        }
    }
}

// General extractMax function
pair<float, string> HeapSort::extractMax(pair<float, string> arr[], int &size) {
    pair<float, string> max = arr[0];
    arr[0] = arr[--size];
    heapifyDownMax(arr, 0, size);

    return max;
}

// General extractMin function
pair<float, string> HeapSort::extractMin(pair<float, string> arr[], int &size) {
    pair<float, string> min = arr[0];
    arr[0] = arr[--size];
    heapifyDownMin(arr, 0, size);

    return min;
}

void HeapSort::insertNodeMin(pair<float, string> arr[], int& size, pair<float, string> Key) {
    size = size + 1;
    arr[size - 1] = Key;
    heapifyUpMin(arr, size, size - 1);
}

void HeapSort::insertNodeMax(pair<float, string> arr[], int& size, pair<float, string> Key) {
    size = size + 1;
    arr[size - 1] = Key;
    heapifyUpMax(arr, size, size - 1);
}
//
// Created by thear on 10/28/2025.
//
#include <iostream>
using namespace std;

// Min heap to get the lowest rated hotels (average - only one review for each hotel needed)



// Min heap to get the lowest rated review for a specific hotel (parse through all reviews for a specific hotel name)



// Max heap to get the highest rated hotels (average - only one review for each hotel needed)



// Max heap to get the highest rated review for a specific hotel (parse through all reviews for a specific hotel name)



// Heapify function
void heapifyDown(int arr[], int index, int size) {
    int largest_child_index = index;
    int leftInd = 2 * index + 1;
    int rightInd = 2 * index + 2;

    if (leftInd >= size) {
        return;
    }

    if (leftInd < size && arr[leftInd] > largest_child_index) {
      largest_child_index = leftInd;
    }
    if (rightInd < size && arr[rightInd] > largest_child_index) {
      largest_child_index = rightInd;
    }

    swap(arr[index], arr[largest_child_index]);
    heapifyDown(arr, largest_child_index, size);
}

// General extractMax function
int* extractMax(int arr[], int size) {
    arr[0] = arr[--size];
    heapifyDown(arr, 0, size);

    return arr;
}
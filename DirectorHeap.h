#pragma once
#include <vector>
#include <string>
#include <iostream>

struct DirectorEntry {
    std::string id;
    size_t movieCount;
    DirectorEntry(std::string id, size_t movieCount) : id(id), movieCount(movieCount) {}

    // Operator for max-heap property
    bool operator<(const DirectorEntry& other) const {
        return movieCount < other.movieCount;
    }

    // Operator for max-heap property using operator>
    bool operator>(const DirectorEntry& other) const {
        return movieCount > other.movieCount;
    }
};

class DirectorHeap {
private:
    std::vector<DirectorEntry> heap;

    void heapifyUp(int index) {
        while (index > 0 && heap[(index - 1) / 2] < heap[index]) {
            std::swap(heap[(index - 1) / 2], heap[index]);
            index = (index - 1) / 2;
        }
    }

    void heapifyDown(int index) {
        int size = heap.size();
        int largest = index;
        int leftChildIdx = 2 * index + 1;
        int rightChildIdx = 2 * index + 2;

        if (leftChildIdx < size && heap[leftChildIdx] > heap[largest]) {
            largest = leftChildIdx;
        }
        if (rightChildIdx < size && heap[rightChildIdx] > heap[largest]) {
            largest = rightChildIdx;
        }

        if (largest != index) {
            std::swap(heap[index], heap[largest]);
            heapifyDown(largest);
        }
    }


public:
    void insert(const std::string& directorID, size_t movieCount) {
        DirectorEntry entry(directorID, movieCount);
        heap.push_back(entry);
        heapifyUp(heap.size() - 1);
    }

    DirectorEntry extractMax() {
        if (heap.empty()) {
            throw std::range_error("Heap is empty");
        }
        DirectorEntry max = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        heapifyDown(0);
        return max;
    }

    bool isEmpty() const {
        return heap.empty();
    }
};
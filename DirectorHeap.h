#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <stdexcept>

class DirectorHeap {
private:
    struct DirectorEntry {
        std::string id;
        size_t movieCount;
        DirectorEntry() : id(""), movieCount(0) {} //Default constructor
        DirectorEntry(std::string id, size_t movieCount) : id(id), movieCount(movieCount) {}

        bool operator<(const DirectorEntry& other) const {
            return movieCount < other.movieCount;
        }

        bool operator>(const DirectorEntry& other) const {
            return movieCount > other.movieCount;
        }
    };

    std::vector<DirectorEntry> heap;
    std::unordered_map<std::string, size_t> indexMap; 

    int parent(int i) { return (i - 1) / 2; }

    int left(int i) { return 2 * i + 1; }

    int right(int i) { return 2 * i + 2; }

    void heapifyUp(int idx) {
        while (idx != 0 && heap[parent(idx)] < heap[idx]) {
            std::swap(heap[parent(idx)], heap[idx]);
            indexMap[heap[idx].id] = idx;
            indexMap[heap[parent(idx)].id] = parent(idx);
            idx = parent(idx);
        }
    }

    void heapifyDown(int idx) {
        int largest = idx;
        int l = left(idx);
        int r = right(idx);

        if (l < static_cast<int>(heap.size()) && heap[l] > heap[idx])
            largest = l;
        if (r < static_cast<int>(heap.size()) && heap[r] > heap[largest])
            largest = r;

        if (largest != idx) {
            std::swap(heap[idx], heap[largest]);
            indexMap[heap[idx].id] = idx;
            indexMap[heap[largest].id] = largest;
            heapifyDown(largest);
        }
    }

public:
    //Either inserts a director into the heap or updates their movie count
    void insertOrUpdate(const std::string& directorID, size_t count) {
        if (indexMap.find(directorID) != indexMap.end()) {
            size_t idx = indexMap[directorID];
            heap[idx].movieCount += count;
            heapifyDown(idx);
            heapifyUp(idx);
        } else {
            DirectorEntry newEntry(directorID, count);
            heap.push_back(newEntry);
            size_t idx = heap.size() - 1;
            indexMap[directorID] = idx;
            heapifyUp(idx);
        }
    }

    //Function to extract the director with the most movies
    DirectorEntry extractMax() {
        if (heap.empty()) {
            throw std::range_error("Heap is empty");
        }
        DirectorEntry max = heap.front();
        std::swap(heap[0], heap.back());
        indexMap.erase(max.id);
        heap.pop_back();
        if (!heap.empty()) {
            indexMap[heap[0].id] = 0;
            heapifyDown(0);
        }
        return max;
    }

    // the function retrieves the Nth highest director based on the number of movies directed
    DirectorEntry getNthHighestDirector(size_t n) {
        if (n == 0 || n > heap.size()) {
            throw std::invalid_argument("Invalid value of n.");
        }
        std::vector<DirectorEntry> tempHeap = heap;
        std::unordered_map<std::string, size_t> tempIndexMap = indexMap;

        DirectorEntry nthDirector;
        for (size_t i = 0; i < n; ++i) {
            nthDirector = extractMax();
        }

        heap = tempHeap;
        indexMap = tempIndexMap;
        for (size_t i = heap.size() / 2; i-- > 0; ) {
            heapifyDown(i);
        }

        return nthDirector;
    }

    bool isEmpty() const {
        return heap.empty();
    }
};

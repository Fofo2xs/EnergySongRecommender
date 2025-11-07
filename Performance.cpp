#include <iostream>
#include <chrono>
#include "Performance.h"
#include "Heap.h"
#include "RB.h"

// helper function to print prettier
void printTime(const std::string& operation, long long duration_ms) {
    std::cout << "  " << operation << ": " << duration_ms << " ms" << std::endl;
}

void Performance::testInsertion(const std::vector<Song>& songs) {
    std::cout << "\nTesting Insertion Speed (" << songs.size() << " songs)" << std::endl;
    
    // test heap insertion
    auto start = std::chrono::high_resolution_clock::now();
    
    Heap heap;
    for (const auto& song : songs) {
        heap.insert(song);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto heapTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    printTime("Heap Insert Time", heapTime);

    // testing RB tree insertion
    start = std::chrono::high_resolution_clock::now();
    
    RB tree;
    for (const auto& song : songs) {
        tree.insert(song);
    }
    
    end = std::chrono::high_resolution_clock::now();
    auto rbTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    printTime("RB Tree Insert Time", rbTime);
}

void Performance::testTopNRetrieval(const std::vector<Song>& songs, int n) {
    if (n > songs.size()) {
        n = songs.size();
    }
    std::cout << "\nTesting Top " << n << " Retrieval Speed" << std::endl;

    // test heap retrieval
    Heap heap;
    for (const auto& song : songs) {
        heap.insert(song);
    }
    
    // timing n extractions
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<Song> heapTopN = heap.recommendTopNHeap(n);

    auto end = std::chrono::high_resolution_clock::now();
    auto heapTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    printTime("Heap Top N (N extracts)", heapTime);

    // test RB tree retrieval
    RB tree;
    for (const auto& song : songs) {
        tree.insert(song);
    }

    // time topNenergy call
    start = std::chrono::high_resolution_clock::now();
    std::vector<Song> rbResults = tree.topNenergy(n, 0.0f, 1.0f);

    end = std::chrono::high_resolution_clock::now();
    auto rbTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    printTime("RB Tree Top N (Traversal)", rbTime);
}

void Performance::testRangeQuery(const std::vector<Song>& songs, float energyLow, float energyHigh) {
    std::cout << "\nTesting Range Query [" << energyLow << ", " << energyHigh << "]" << std::endl;

    // test teap range query
    Heap heap;
    for (const auto& song : songs) {
        heap.insert(song);
    }

    // time the query O(N)
    auto start = std::chrono::high_resolution_clock::now();
    
    std::vector<Song> heapResults = heap.findSongInRange(energyLow, energyHigh);

    auto end = std::chrono::high_resolution_clock::now();
    auto heapTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    printTime("Heap Range Query (N scan)", heapTime); 
    std::cout << "   Found " << heapResults.size() << " songs." << std::endl;


    // test RB tree range query
    RB tree;
    for (const auto& song : songs) {
        tree.insert(song);
    }

    // time the query O(K + log N)
    start = std::chrono::high_resolution_clock::now();
    std::vector<Song> rbResults = tree.topNenergy(songs.size(), energyLow, energyHigh);

    end = std::chrono::high_resolution_clock::now();
    auto rbTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    printTime("RB Tree Range Query (Traversal)", rbTime);
    
    std::cout << "   Found " << rbResults.size() << " songs." << std::endl;
}


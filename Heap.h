#include <vector>
#include "Song.h"
using namespace std;

//Max Heap
class Heap {
private:
    vector<Song> heap;

    // Parent, left, and right child indexies
    int parent(int i);
    int left(int i);
    int right(int i);

    //Heapify up and down
    void heapifyUp(int index);
    void heapifyDown(int index);

    //Sort heap alphebeitcally as well
    bool greaterPriority(int i, int j) const;



public:
    Heap();

    //Properties in Heaps
    void insert(const Song& song);
    Song extractMax();
    Song getMax() const;
    bool isEmpty() const;
    int size() const;
    vector<Song> search(const string &songName) const;
    void printAllSongs() const;

    //For Application purposes
    vector<Song> recommendTopNHeap(int n) const;
    vector<Song> findSongInRange(double low, double high) const;
    Song getRandomSong(int i) const;
    double calculateAverageHeap() const;
    Song findMinEnergyHeap() const;;



};
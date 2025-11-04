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

    public:
        Heap();

        //Properties in Heaps
        void insert(const Song& song);
        Song extractMax();
        Song getMax() const;
        bool isEmpty() const;
        int size() const;
        void printAllSongs() const;
        vector<Song> recommendTopNHeap(int n) const;
        double findEnergyHeap(const string &songName, const string &artist) const;
        vector<Song> findSongInRange(double low, double high) const;



 };
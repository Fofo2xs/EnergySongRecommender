#include "Heap.h"
#include <iostream>
#include <cstdlib>

using namespace std;


Heap::Heap(){}


int Heap::parent(int i){
 return (i-1)/2;
}


int Heap::left(int i){
 return 2*i+1;
}


int Heap::right(int i){
 return 2*i+2;
}


//Check if heap is empty
bool Heap::isEmpty() const{
 return heap.empty();
}


//Heap size
int Heap::size() const{
 return heap.size();
}


//Insert into heap
void Heap::insert(const Song& song){
 heap.push_back(song);
 heapifyUp(heap.size()-1);


}

bool Heap::greaterPriority(int i, int j) const {
    if (heap[i].getEnergy() > heap[j].getEnergy()) {
        return true;
    }

    if (heap[i].getEnergy() == heap[j].getEnergy() && heap[i].getSongName() > heap[j].getSongName()) {
        return true;
    }
    return false;
}

void Heap::heapifyUp(int index) {
    while(index > 0 ) {
        int parentIndex = parent(index);
        const Song& currentSong = heap[index];
        const Song& parentSong = heap[parentIndex];
        if (currentSong.getEnergy() > parentSong.getEnergy()){
            swap(heap[parentIndex],heap[index]);
            index = parent(index);
        }

        //Heap sorted with song names
        else if (currentSong.getEnergy() == parentSong.getEnergy() && (currentSong.getSongName() > parentSong.getSongName())) {
            swap(heap[parentIndex],heap[index]);
            index = parentIndex;
        }
        else {
            break;
        }
    }
}


void Heap::heapifyDown(int index){
 int largest = index;
 int leftChild = left(index);
 int rightChild = right(index);


 if (leftChild < heap.size() && greaterPriority(leftChild, largest)) {
   largest = leftChild;
 }
 if (rightChild < heap.size() && greaterPriority(rightChild, largest)) {
   largest = rightChild;
 }
 if(largest != index){
   swap(heap[index],heap[largest]);
   heapifyDown(largest);
 }
}


//Getting max energy
Song Heap::extractMax(){
 if(heap.empty()){
   return Song();
 }


 Song max = heap[0];
 heap[0] = heap[heap.size()-1];
 heap.pop_back();
 heapifyDown(0);
 return max;
}


Song Heap::getMax() const{
 if(heap.empty()){
   return Song();
 }
 return heap[0];
}


//Print Songs
void Heap::printAllSongs() const {
 for(const auto& song : heap){
   cout << song.getSongName() << " by " << song.getArtist()
   << " | Energy: " << song.getEnergy()
   << " | Genre: " << song.getGenre() << endl;
 }
}



vector<Song> Heap::recommendTopNHeap(int n) const {
   vector<Song> topSongs;
   if (heap.empty() || n <= 0) {
     return topSongs;
   }


   //Made a copy so the heap isn't altered
   Heap tempHeap = *this;


   for (int i = 0; i < n && !tempHeap.isEmpty(); ++i) {
       topSongs.push_back(tempHeap.extractMax());
   }


   return topSongs;
}

vector<Song> Heap::search(const string &songName) const {
    vector<Song> matchingSongs;

    for (const auto& song : heap) {
        if (song.getSongName() == songName) {
            matchingSongs.push_back(song);
        }
    }
    return matchingSongs;
}

vector<Song> Heap::findSongInRange(double low, double high) const {
     vector<Song> result;
     if (low < 0.0 || high > 1.0) {
       return result;
     }
     for (const auto& song : heap) {
       double energy = song.getEnergy();
       if (energy >= low && energy <= high) {
         result.push_back(song);
       }
     }
     return result;
}


Song Heap::getRandomSong(int i) const {
    if (i >= 0 && i < heap.size()) {
        return heap[i];
    }
    return Song();
}

double Heap::calculateAverageHeap() const {
    if (heap.empty()) {
        return 0.0;
    }
    double totalEnergy = 0.0;
    for (const auto& song : heap) {
        totalEnergy += song.getEnergy();
    }
    return totalEnergy / heap.size();
}

Song Heap::findMinEnergyHeap() const{
    if (heap.empty()) {
        return Song();
    }
    Song minEnergy = heap[0];
    for (  int i = 1; i < heap.size(); ++i) {
        if (heap[i].getEnergy() < minEnergy.getEnergy()) {
            minEnergy = heap[i];
        }

        else if (heap[i].getEnergy() == minEnergy.getEnergy()) {
            if (heap[i].getSongName() < minEnergy.getSongName()) {
                minEnergy = heap[i];
            }
        }
    }
    return minEnergy;
    
}
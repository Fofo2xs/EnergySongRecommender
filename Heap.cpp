#include "Heap.h"
#include <iostream>
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

void Heap::heapifyUp(int index){
  while(index > 0 && heap[parent(index)].getEnergy() < heap[index].getEnergy()){
    swap(heap[parent(index)],heap[index]);
    index = parent(index);
  }
}

void Heap::heapifyDown(int index){
  int largest = index;
  int leftChild = left(index);
  int rightChild = right(index);

  if (leftChild < heap.size() && heap[leftChild].getEnergy() > heap[largest].getEnergy()){
    largest = leftChild;
  }
  if (rightChild < heap.size() && heap[rightChild].getEnergy() > heap[largest].getEnergy()){
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
void Heap::printAllSongs() const{
  for(const auto& song : heap){
    cout << song.getSongName() << " by " << song.getArtist()
    << " | Energy: " << song.getEnergy()
    << " | Genre: " << song.getGenre() << endl;
  }
}



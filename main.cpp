#include <iostream>
#include "Song_loader.h"
#include "Heap.h"
using namespace std;


int main() {

   //Load Songs
   vector<Song> songs = loadSongs("dataset.csv");
   cout << "Loaded " << songs.size() << " songs" << endl;

   Heap songHeap;

   for (const Song& song : songs) {
      songHeap.insert(song);
   }
   songHeap.printAllSongs();




   
}




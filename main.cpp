#include <iostream>
#include "Song_loader.h"
#include "Heap.h"
using namespace std;


int main() {

   //Load Songs
    vector<Song> songs = loadSongs("dataset.csv");
    cout << "Loaded " << songs.size() << " songs" << endl;
    Heap songHeap;

    //Insert Songs into heap
    for (auto&  song: songs) {
        songHeap.insert(song);
    }

    cout << "\n----------- Amplify!!!! -------" << endl;
    cout << "Song Recommender Based on Energy " << endl;
    cout << "\n -------------Menu--------------" << endl;
    cout << "1. View All Songs" << endl;
    cout << "2. Add New Song" << endl;
    cout << "3. Delete Song" << endl;
    cout << "4. Recommend Top N Songs by Energy Level" << endl;
    cout << "5. Find Energy Level of a Song" <<  endl;
    cout << "6. Find Songs in Range Query" << endl;
    cout << "7. Compare Heap vs Red-Black Tree Performance" << endl;
    cout << "8. Exit" << endl;
    cout << "--------------------------------" << endl;
    
    

}




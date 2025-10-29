#include <iostream>
#include "Song_loader.cpp"

using namespace std;

int main() {
    vector<Song> songs = loadSongs("dataset.csv");

   cout << "Loaded " << songs.size() << " songs.\n";

    for (int i = 0; i < 5 && i < songs.size(); i++) {
        std::cout << songs[i].getTrackName() << " by " << songs[i].getArtist()
                  << " | Energy: " << songs[i].getEnergy()
                  << "| Genre: " << songs[i].getGenre() << endl;
    }

}
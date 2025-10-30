
#include "Song.h"

// Default constructor
Song::Song() {
    songName = "";
    artist = "";
    energy = 0.0;
    genre = "";
}

// Parameterized constructor
Song::Song(const std::string& songName, const std::string& artist, double energy,const std::string& genre) {
    this->songName = songName;
    this->artist = artist;
    this->energy = energy;
    this->genre = genre;
}

// Getters
string Song::getSongName() const {
    return songName;
}

string Song::getArtist() const {
    return artist;
}

double Song::getEnergy() const {
    return energy;
}

string Song::getGenre() const {
    return genre;
}


// Print song details; Here for checking/Testing
void Song::printSong() const {
    cout << songName << " by " << artist << " | Energy: " << energy
         << " | Genre: " << genre << endl;
}

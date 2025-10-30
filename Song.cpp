
#include "Song.h"

// Default constructor
Song::Song() {
    trackName = "";
    artist = "";
    energy = 0.0;
    genre = "";
}

// Parameterized constructor
Song::Song(const std::string& trackName, const std::string& artist, double energy,const std::string& genre) {
    this->trackName = trackName;
    this->artist = artist;
    this->energy = energy;
    this->genre = genre;
}

// Getters
string Song::getTrackName() const {
    return trackName;
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

// Print song details
void Song::printSong() const {
    cout << "Track: " << trackName << " | Artist: " << artist << " | Energy: " << energy
         << " | Genre: " << genre << endl;
}

#pragma once
#include <string>
#include <iostream>

using namespace std;

class Song{
    private:
        string songName;
        string artist;
        double energy = 0.0;
        string genre;

    public:
        // Default constructor
        Song();

        // Parameterized constructor
        Song(const string& songName, const string& artist, double energy, const string& genre);


        // Getters
        string getSongName() const;
        string getArtist() const;
        double getEnergy() const;
        string getGenre() const;


        void printSong() const;



};






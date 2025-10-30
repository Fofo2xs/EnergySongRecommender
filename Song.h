#include <string>
#include <istream>
#include <iostream>

using namespace std;

class Song{
    private:
        string trackName;
        string artist;
        double energy;
        string genre;

    public:
        // Default constructor
        Song();

        // Parameterized constructor
        Song(const string& trackName, const string& artist, double energy, const string& genre);


        // Getters
        string getTrackName() const;
        string getArtist() const;
        double getEnergy() const;
        string getGenre() const;


        void printSong() const;



};






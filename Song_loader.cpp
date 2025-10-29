#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;
#include "Song.h"

vector<Song> loadSongs(const string& filename) {
 
    vector<Song> songs;

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return songs;
    }
    string line;

    // Skip header
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string temp; // skip columns
        string track_name, artist_name, energy_str, track_genre;

        // Skip first 2 columns: index, track_id,
        getline(ss, temp, ','); // index
        getline(ss, temp, ','); // track_id

        // Artist name
        getline(ss, artist_name, ',');

        getline(ss, temp, ',');

        getline(ss, track_name, ',');

        // Skip columns until energy
        for (int i = 0; i < 4; i++) {
            getline(ss, temp, ',');
        }

        getline(ss, energy_str, ',');

        // Skip columns until track_genre
        for (int i = 0; i < 10; i++) {
            getline(ss, temp, ',');
        }

        //Track genre
        getline(ss, track_genre, ',');

        double energy = 0.0;
        try {
            energy = stod(energy_str);
        }
        catch (const std::invalid_argument& e) {
            // Skip row if energy is not a number
            continue;
        }
        songs.emplace_back(track_name, artist_name, energy, track_genre);
    }

    return songs;
}

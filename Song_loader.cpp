#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;
#include "Song_loader.h"

vector<Song> loadSongs(const string& filename) {

    vector<Song> songs;

    //Open file
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return songs;
    }
    string line;
    //Skip header
    getline(file, line);

    while (getline(file, line)) {
        vector<string> columns;
        string columnInfo;
        bool inQuotes = false;

        // Parse line character by character as the dataset has commas in song names
        for (int i = 0; i < line.size(); ++i) {
            char currentChar = line[i];

            //Check if information is in quotes
            if (currentChar == '"') {
                inQuotes = !inQuotes;
            }
            else if (currentChar == ',' && !inQuotes) {
                columns.push_back(columnInfo);
                columnInfo = "";
            }
            else {
                columnInfo += currentChar;
            }
        }
        //Push last column
        columns.push_back(columnInfo);

        //Accessing information that's relevant
        string songName = columns[4];
        string artist = columns[2];
        string genre = columns[20];
        string energy_str = columns[9];

        //Convert energy value into a double
        double energy = 0.0;
        energy = stod(energy_str);

        //Create and add Song
        songs.emplace_back(songName, artist,energy, genre);
    }

    return songs;
}
#include <iostream>
#include "Song_loader.h"
#include "Heap.h"
#include <fstream>
#include <sstream>
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

    cout << "\n------------ Amplify!!!! -------------------" << endl;
    cout << "Song Recommender Based on Energy " << endl;
    cout << "\n ------------- Menu ------------------------" << endl;
    cout << "1. View All Songs" << endl;
    cout << "2. Add New Song" << endl;
    cout << "3. Recommend Top N Energetic Songs" << endl;
    cout << "4. Find Energy Level of a Song" <<  endl;  //should enter name of song and artist
    cout << "5. Find Songs in Range Query" << endl;
    cout << "6. Compare Heap vs Red-Black Tree Performance" << endl;
    cout << "7. Exit" << endl;
    cout << "---------------------------------------------" << endl;

    //songHeap.printAllSongs();

    while (true) {
        cout << "\nSelect an option:" << endl;
        string option;
        cin >> option;


        if (option == "1") {
            songHeap.printAllSongs();
        }

        else if (option == "2") {
           cout << "Song can be added by this format: " << endl;
            cout << "[Song Name] [Artist] [Energy Level] [Genre]" << endl;
            while (true) {
                cin.ignore();
                string input;
                cout << "Enter Song Details: ";
                getline(cin, input);
                stringstream ss(input);
                string songName, artist, energy_str, genre;

                
                if (!( ss >> songName >> artist >> energy_str >> genre)) {
                    cout << "Invalid format. Please use: [Song Name] [Artist] [Energy Level] [Genre]";
                    continue;
                }

                double energy = stod(energy_str);

                if (energy < 0.0|| energy > 1.0) {
                    cout << "Song unsuccessfully added, invalid energy value: " << energy << endl;
                    cout << "Energy values are between 0.0 - 1.0" << endl;
                    continue;
                }

                bool duplicate = false;
                for (auto&  song: songs) {
                    if (song.getSongName() == songName && song.getArtist() == artist) {
                        cout << "Song already exists in Amplify!!!!";
                        duplicate = true;
                        break;;
                    }
                }
                if (duplicate) {
                    continue;
                }

                //Create song object
                Song newSong (songName, artist, energy, genre);

                songs.push_back(newSong);

                //Add to Heap structure
                songHeap.insert(newSong);

                cout << "Song Added" << endl;
                break;
            }
            


        }
        else if (option == "3") {
            int n;
            while (true) {
                cout << "Enter how many top energetic songs you want: " << endl;
                string input;
                cin >> input;

                stringstream ss(input);
                if (ss >> n && n > 0) {
                    break;
                }
                else {
                    cout << "Invalid input. Please enter a positive integer." << endl;
                    cin.clear();
                }
            }
            cout << "Top " << n << " Energetic Songs" << endl;
            songHeap.recommendTopNHeap(n);

        }
        else if (option == "4") {

        }
        else if (option == "5") {

        }
        else if (option == "6") {

        }
        else if (option == "7") {
            break;

        }
        else {
            cout <<  "Invalid Option" << endl;
        }

    }
    
    
    

}




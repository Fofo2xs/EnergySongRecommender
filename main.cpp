#include <iostream>
#include "Song_loader.h"
#include "Heap.h"
#include <fstream>
#include <sstream>
#include "RB.h"
#include <limits>
#include <chrono>
using namespace std;

double timeRBinsertion = 0;
double timeRBdeletion = 0;
double timeRBsearch = 0;
double timeHeapPush = 0;
double timeHeapPop = 0;
double timeHeapSearch = 0;
int SEARCH = 0;
int INSERT = 0;

int main() {

   //Load Songs
    vector<Song> songs = loadSongs("dataset.csv");
    cout << "Loaded " << songs.size() << " songs" << endl;
    Heap songHeap;
    RB redblacktree;


    //Insert Songs into heap
    for (auto&  song: songs) {
        songHeap.insert(song);
        redblacktree.insert(song);
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
          int n;
          std::cout<<"Enter Number of Songs: "<<std::endl;
          std::cin>>n;
          float l, h;
          std::cout<<"Enter Desired Energy Range Level (two numbers between 0 and 1): "<<std::endl;
          std::cin>>l;
          std::cin>>h;
           auto start = std::chrono::high_resolution_clock::now();
           std::vector<Song> Topsongs = redblacktree.topNenergy(n, l, h);
           auto end = std::chrono::high_resolution_clock::now();
            timeRBsearch += chrono::duration<double>(end - start).count() * 1000;
            SEARCH++;

        }
        else if (option == "5") {

          std::cout << "Type the name of the song or what you remember..."<<std::endl;
          std::string input;
          std::cin.ignore();

          std::getline(std::cin, input);
          std::cout << "Searching for " << input<<std::endl;
///////////////////////////////////////////******Red Black Tree search*******//////////////////////////////////////////////
            auto start = std::chrono::high_resolution_clock::now();// measuring time performance of searching in a search operation
          std::vector<Song> matches = redblacktree.search(input);
            auto end = chrono::high_resolution_clock::now();

            timeRBsearch += chrono::duration<double>(end - start).count() * 1000;
            SEARCH++;
          if (matches.empty()) {
            std::cout << "No song found" << std::endl;
            continue;
          }
          if (matches.size() == 1) {
            std::cout << "Found 1 match!"<<std::endl;
            std::cout<<std::endl;
            std::cout << matches[0].getSongName() <<" by: "<< matches[0].getArtist()<< std::endl;
            std::cout << "Energy Level: "<< matches[0].getEnergy() << std::endl;

          }
          else if (matches.size() > 1) {
              std::cout << "Matches found: "<<std::endl;
              for (int i = 0; i < matches.size(); ++i) {
                  std::cout<<std::endl;
                  std::cout<< i+1 <<". "<< matches[i].getSongName() << " by: " << matches[i].getArtist()<<std::endl;
                  std::cout<< "Energy Level: " << matches[i].getEnergy() << std::endl;
              }
              std::cout << std::endl;
              std::cout<<"Select the song you were looking for."<<std::endl;

              int choice;
              if (!(std::cin >> choice))
                {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout<<"Please enter a number."<<std::endl;
                }
              //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                else {
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    if (choice>0 && choice<=matches.size()) {
                        Song s = matches[choice-1];
                        std::cout << "You have selected " <<s.getSongName()<<" by " <<s.getArtist()<< std::endl;
                        std::cout << "Energy Level: " << s.getEnergy() << std::endl;
                    }
                    else {
                        std::cout << "Invalid selection." << std::endl;
                       // continue;
                    }
                }
/////////////////////////////////////////******End of Red Black Tree search*******////////////////////////////////////////////////////
          }
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




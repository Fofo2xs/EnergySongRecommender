#include <iostream>
#include "Song_loader.h"
#include "Heap.h"
#include <fstream>
#include <sstream>
#include "RB.h"
#include <limits>
#include <chrono>
#include <stdexcept>
using namespace std;

double timeRBinsertion = 0;
double timeRBdeletion = 0;
double timeRBsearch = 0;
double timeHeapPush = 0;
double timeHeapPop = 0;
double timeHeapSearch = 0;
int SEARCH = 0;
int INSERT = 0;
void menu(){
    cout << "\n------------ Amplify!!!! -------------------" << endl;
    cout << "Song Recommender Based on Energy " << endl;
    cout << "\n ------------- Menu ------------------------" << endl;
    cout << "1. View All Songs" << endl;
    cout << "2. Add New Song" << endl;
    cout << "3. Recommend Top N Songs by Energy Level" << endl;
    cout << "4. Find Energy Level of a Song" <<  endl;
    cout << "5. Find Songs in Range Query" << endl;
    cout << "6. Compare Heap vs Red-Black Tree Performance" << endl;
    cout << "7. Exit" << endl;
    cout << "---------------------------------------------" << endl;
    }

int main() {

   //Load Songs
    vector<Song> songs = loadSongs("dataset.csv");
    cout << "Loaded " << songs.size() << " songs" << endl;
    Heap songHeap;
    RB redblacktree;


    //Insert Songs into heap and red black tree...
    for (auto&  song: songs) {
        songHeap.insert(song);
        redblacktree.insert(song);
    }
    //songHeap.printAllSongs();
    menu();
    while (true) {
        string option;
        cin >> option;

        if (option == "1") {
            songHeap.printAllSongs();
        }

        else if (option == "2") {
           cout << "Song can be added by this format: " << endl;
            cout << "[Song Name], [Artist], [Energy Level], [Genre]" << endl;
            cin.ignore();
            while (true) {
//                cin.ignore();
                string input;
                cout << "Enter Song Details (or enter cancel to go back): " << endl;
                getline(cin, input);
                if (input == "cancel" || input == "Cancel"|| input.empty()) {
                  std::cout<< "Cancelled..." << endl;
                  std::cout<<std::endl;
                  break;
                }
                stringstream ss(input);
                string songName, artist, energy_str, genre;

                
                if (!getline(ss, songName,',') || !getline(ss, artist,',') || !getline(ss, energy_str,',') ||!getline(ss, genre,',')) {
                    cout << "Invalid format. Please use: [Song Name], [Artist], [Energy Level], [Genre]"<<std::endl;
                    continue;
                }
                auto lambda = [](std::string& s){
                  s.erase(0,s.find_first_not_of('\t'));
                  s.erase(s.find_last_not_of('\t')+1);
                };
                lambda(songName);
                lambda(artist);
                lambda(energy_str);
                lambda(genre);
                //double energy = stod(energy_str);
                if (songName.empty() || artist.empty() || energy_str.empty() || genre.empty()) {
                  std::cout<< "Please fill all the required fileds." << std::endl;
                  continue;
                }
                double energy;
                try{
                energy = stod(energy_str);
                }catch(std::invalid_argument& e){
                  std::cout << "Energy values are between 0.0 - 1.0" << std::endl;
                  continue;
                }

                if (energy < 0.0|| energy > 1.0) {
                    cout << "Song unsuccessfully added, invalid energy value: " << energy << endl;
                    cout << "Energy values are between 0.0 - 1.0" << endl;
                    continue;
                }
                auto lambda2 = [](std::string s){//dont alter the original
                  transform(s.begin(), s.end(), s.begin(), ::tolower);
                  return s;
                };
                std::string lowerName = lambda2(songName);
                std::string lowerArtist = lambda2(artist);


                bool duplicate = false;
                for (auto&  song: songs) {
                  std::string lowerName1 = lambda2(song.getSongName());
                  std::string lowerArtist1 = lambda2(song.getArtist());
                    if (lowerName1 == lowerName && lowerArtist1 == lowerArtist) {
                        cout << "Song already exists in Amplify!!!!"<<std::endl;;
                        duplicate = true;
                        break;
                    }
                }
                if (duplicate) {
                    continue;
                }

                //Create song object

                Song newSong (songName, artist, energy, genre);

                songs.push_back(newSong);

                //Add to Heap structure
                //ADD PERFORMANCE TIME
                songHeap.insert(newSong);

                //Add to Red Black Tree Data Structure
                auto start = std::chrono::high_resolution_clock::now();
                redblacktree.insert(newSong);
                auto end = std::chrono::high_resolution_clock::now();
                timeRBinsertion += chrono::duration<double>(end - start).count() * 1000; /// MEASURING TIME
                INSERT++;
                cout << "Song Added" << endl;
                break;
            }
            menu();
        }

        else if (option == "3") {
          int n;
          std::cout<<"Enter Number of Songs: "<<std::endl;
            if (!(std::cin >> n) || n <= 0) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
                std::cout<<"Invalid input. Please enter a positive number."<< std::endl;
                menu();
                continue;
            }
          float l;
          float h;
          std::cout<<"Enter Desired Energy Range Level (two numbers between 0.0 and 1.0, both inclusive): "<<std::endl;
          if (!(std::cin >> l) || !(std::cin >> h) || l < 0 || h < 0 || l > h) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            std::cout<<"Invalid Range. please enter a positive value between 0.0 and 1.0";
            menu();
            continue;
          }
            ///////////////////////////////////////////*********Red Black Tree search**********//////////////////////////////////////////////
           auto start = std::chrono::high_resolution_clock::now();
           std::vector<Song> Topsongs = redblacktree.topNenergy(n, l, h);
           auto end = std::chrono::high_resolution_clock::now();
            timeRBsearch += chrono::duration<double>(end - start).count() * 1000; /// MEASURING TIME
            SEARCH++;

            if (Topsongs.empty())
              {
              std::cout<<"No songs found with the specified range."<<endl;
              }
              else {
                if (Topsongs.size()<n)
                  {
                  std::cout<<"Found only "<<Topsongs.size()<<" songs."<<endl;
                  }
                  else{
                    std::cout<<"Top "<<n<<" songs."<<endl;
                    std::cout<<std::endl;
                    }
                  for (auto i = 0;i<Topsongs.size();i++) {
                      std::cout<< i+1<<". ";
                      std::cout<< Topsongs[i].getSongName()<<" by: "<< Topsongs[i].getArtist()<<std::endl;
                      std::cout<< "Energy level: "<<Topsongs[i].getEnergy()<<endl;
                      std::cout<<std::endl;
              }
              ///////////////////////////////////////////******Red Black Tree search*******//////////////////////////////////////////////
            }
            //////////////////////////////////////////    /Heap /////////////////////
            vector<Song> sonsInRange = songHeap.findSongInRange(l,h);

            cout << "-------------- Heap Results ----------------" <<endl;
            // ADD IN PERFORMANCE TIME
            Heap rangeHeap;
            for (auto& song: sonsInRange) {
                rangeHeap.insert(song);
            }

            vector <Song> topSongs = rangeHeap.recommendTopNHeap(n);
            if (topSongs.empty()) {
                cout << "No songs found with the specified range."<<endl;
            }
            else {
                if (topSongs.size()<n) {
                    cout << "Found only "<< topSongs.size()<<" songs."<<endl;
                }
                else {
                    cout << "Top "<<n<<" songs."<<endl;
                    cout << endl;
                }

                for (auto i = 0; i<topSongs.size(); i++) {
                    cout << i+1<<". ";
                    cout << topSongs[i].getSongName()<<" by: "<< topSongs[i].getArtist()<<endl;
                    cout << "Energy level: "<<topSongs[i].getEnergy()<<endl;
                    cout << endl;
                }
            }



        }
        else if (option == "4") {

          std::cout << "Type the name of the song or what you remember..."<<std::endl;
          std::string input;
          std::cin.ignore();

          std::getline(std::cin, input);
          std::cout << "Searching for " << input<<std::endl;
///////////////////////////////////////////******Red Black Tree search*******//////////////////////////////////////////////
            auto start = std::chrono::high_resolution_clock::now();// measuring time performance of searching in a search operation
          std::vector<Song> matches = redblacktree.search(input);
            auto end = chrono::high_resolution_clock::now();

            timeRBsearch += chrono::duration<double>(end - start).count() * 1000; /// MEASURING TIME
            SEARCH++;
          if (matches.empty()) {
            std::cout << "No song found" << std::endl;
            menu();
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




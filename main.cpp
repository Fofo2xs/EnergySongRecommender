#include <iostream>
#include "Song_loader.h"
#include "Heap.h"
#include <fstream>
#include <sstream>
#include "RB.h"
#include <limits>
#include <chrono>
#include <stdexcept>
#include <iomanip>
#include <cmath>
#include "Performance.h"
#include <set>

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
    cout << "\n------------- Menu ------------------------" << endl;
    cout << "1. View 20 Random songs." << endl;
    cout << "2. View Playlist Statistics." << endl;
    cout << "3. Add New Song" << endl;
    cout << "4. Recommend Top N Songs by Energy Level" << endl;
    cout << "5. Find Energy Level of a Song" <<  endl;
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

    cout << "\n------------ Amplify!!!! -------------------" << endl;
    cout << "Song Recommender Based on Energy " << endl;
    menu();
    while (true) {
        string option;
        cin >> option;

        if (option == "1") {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
          char choice = 'y';
          std::string input;
          while ( choice == 'y' || choice == 'Y') {
              std::cout << "Displaying 2O random songs"<< std::endl;
              ///////////////---------Red Black Tree-------------////////////////
              std::vector<Song*> randSongs = redblacktree.getRandomSongs(20);
              cout << "\n---------------- Red Black Tree Results ----------" << endl;
              if(randSongs.empty() && songHeap.isEmpty()){
                  std::cout << "No songs found" << std::endl;
                  break;
              }
              for (int i = 0; i < randSongs.size(); i++) {
                  std::string mood;
                  double energy = std::round(randSongs[i]->getEnergy()*100.0)/100.0;
                  if (energy>=0 && energy <= 0.29) {
                      mood = "Chillout";
                  }
                  else if(energy > 0.29 && energy <= 0.49)
                  {
                      mood = "Pleasant";
                  }
                  else if (energy > 0.49 && energy <= 0.69)
                      mood = "Moderate";
                  else if (energy > 0.69 && energy <= 0.89)
                  { mood = "Energetic";}
                  else
                  {mood = "Pure Hype";}
                  std::cout<< i +1<<". ";
                  std::cout << randSongs[i]->getSongName() << " by: "<<randSongs[i]->getArtist();
                  std::cout<<" ["<<randSongs[i]->getGenre()<<"] "<< "(Energy: "<<std::fixed<<setprecision(2)<<randSongs[i]->getEnergy()<<") "<< mood<<std::endl;
              }

        //////////////////// HEAP //////////////////////////////////

        //Make sure difference indices to ensure uniqueness.
        set<int> nums;
        vector <Song> randSongsHeap;

        while (nums.size() < 20 && nums.size() < songHeap.size()) {

            // Generate a random index
            int i = rand() % songHeap.size();

            //Check for uniqueness
            if (nums.find(i) == nums.end()) {

                nums.insert(i);

                // Get Song
                Song randomSong = songHeap.getRandomSong(i);
                randSongsHeap.push_back(randomSong);
            }
        }


        cout << "\n\n -------------------------- Heap Results -------------------- " << endl;
        for (int i = 0; i < randSongsHeap.size(); i++) {
            string mood;
            double energy = round(randSongsHeap[i].getEnergy() * 100.0) / 100.0;

            if (energy >= 0 && energy <= 0.29) {
                mood = "Chillout";
            } else if (energy > 0.29 && energy <= 0.49) {
                mood = "Pleasant";
            } else if (energy > 0.49 && energy <= 0.69) {
                mood = "Moderate";
            } else if (energy > 0.69 && energy <= 0.89) {
                mood = "Energetic";
            } else {
                mood = "Pure Hype";
            }

            cout << i + 1 << ". ";
            cout << randSongsHeap[i].getSongName() << " by: " << randSongsHeap[i].getArtist();
            cout << " [" << randSongsHeap[i].getGenre() << "] "
                      << "(Energy: " << std::fixed << std::setprecision(2) << randSongsHeap[i].getEnergy() << ") "
                      << mood << std::endl;
        }
              //////////////////// HEAP //////////////////////////////////

              ///// User Interaction
              while(true){
                  std::cout << std::endl;
                  std::cout << "Would you like to view 20 other Random songs? (y/n): "<<std::endl;
                  std::getline(std::cin,input);
                  auto lowerSTR = [](std::string& str){
                      std::transform(str.begin(), str.end(), str.begin(), ::tolower);
                  };
                  if (input.empty()) {
                     // lowerSTR(input);
                     continue;
                  }
                  lowerSTR(input);
                  if (input == "y"|| input == "yes") {
                      choice = 'y';
                      break;
                  }
                  else if (input == "n"|| input == "no") {
                      choice = 'n';
                      menu();
                      break;

                  }
                  else {
                      std::cout<< "Invalid argument. Please input your answer as y/n."<<std::endl;
                  }
              }
          }
        }
        else if (option == "2") {
            //songHeap.printAllSongs();
            cout << "\n---------------- Red Black Tree Results ----------" << endl;
            std::cout<< "Average Energy level in the playlist: ";
            double averageEnergy = redblacktree.calculateAverage();//1
            cout << averageEnergy << endl;
            std::cout << endl;

            /////////////////////// Heap ////////////////
            /////*******ADD PERFORMANCE***********
            cout << "\n -------------------------- Heap Results -------------------- " << endl;
            double averageEnergyHeap = songHeap.calculateAverageHeap();
            cout << "Average Energy Level: " << averageEnergyHeap << endl;
            cout << endl;

            cout << "\n---------------- Red Black Tree Results ----------" << endl;
            std::cout << "Song with the HIGHEST energy level: ";
            Song max = redblacktree.findMaxEnergy();//3
            std::cout << max.getSongName() <<" by: "<<max.getArtist()<< endl;
            std::cout <<"Energy level: "<< max.getEnergy() << std::endl;
            std::cout<<std::endl;

            /////////////////////// Heap ////////////////
            /////*******ADD PERFORMANCE***********
            cout << "\n -------------------------- Heap Results -------------------- " << endl;
            Song highestEnergy = songHeap.getMax();
            cout << highestEnergy.getSongName() << " by: "<< highestEnergy.getArtist() << endl;
            cout <<"Energy level: "<< highestEnergy.getEnergy() << endl;
            cout << endl;


            cout << "\n---------------- Red Black Tree Results ----------" << endl;
            std::cout << "Song with the LOWEST energy level: ";
            Song min = redblacktree.findMinEnergy(); //2
            std::cout << min.getSongName() <<" by: "<<min.getArtist()<< endl;
            std::cout <<"Energy level: "<< min.getEnergy() << std::endl;
            std::cout<<std::endl;

            /////////////////////// Heap ////////////////
            /////*******ADD PERFORMANCE***********
            cout << "\n\n -------------------------- Heap Results -------------------- " << endl;
            Song lowestEnergy = songHeap.findMinEnergyHeap();
            cout << lowestEnergy.getSongName() << " by: "<< lowestEnergy.getArtist() << endl;
            cout <<"Energy level: "<< lowestEnergy.getEnergy() << endl;
            cout << endl;

            cout << "\n---------------- Red Black Tree Results ----------" << endl;
            std::cout<<"Top 5 Genres: "<<std::endl;
            std::map<std::string, int> genres = redblacktree.getGenreCount();
            std::vector<std::pair<std::string, int> > genresVector(genres.begin(), genres.end());
            auto sort1 = [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b){
              return a.second>b.second;

              };
            std::sort(genresVector.begin(), genresVector.end(), sort1);
            size_t minNum= std::min(genresVector.size(),static_cast<size_t>(5));
            for (size_t i = 0;i<minNum;i++){
              std::cout<<i+1<<". "<<genresVector[i].first<<std::endl;
              std::cout<<"Songs: "<<genresVector[i].second;
              std::cout<<" ("<<std::fixed<<setprecision(1)<<genresVector[i].second*100/songs.size()<<"%)"<<std::endl;
              //std::cout<<std::endl;
            }
            std::cout << "Out of " << genres.size() << " genres." << std::endl;
            std::cout<<std::endl;

            std::cout<<"Top 5 Artists: "<<std::endl;
            std::map<std::string, int> artists = redblacktree.getArtistCount();
            std::vector<std::pair<std::string, int> > artistsVector(artists.begin(), artists.end());

            std::sort(artistsVector.begin(), artistsVector.end(), sort1);
            size_t minNum2= std::min(artistsVector.size(),static_cast<size_t>(5));
            for (size_t i = 0;i<minNum2;i++){
                std::cout<<i+1<<". "<<artistsVector[i].first<<std::endl;
                std::cout<<"Songs: "<<artistsVector[i].second;
                std::cout<<" ("<<std::fixed<<setprecision(1)<<artistsVector[i].second*100/songs.size()<<"%)"<<std::endl;
                std::cout<<std::endl;
            }
            std::cout << "Out of " << artists.size() << " artists." << std::endl;
            std::cout<<std::endl;

            std::cout<< "Most Energetic Genre: "<<std::endl;
            std::pair<std::string, double> genre = redblacktree.getMostEnergeticGenre();//5
            if (!genre.first.empty()){
                std::cout<< genre.first<<std::endl;
                std::cout<<"Average Energy Level: "<<std::fixed<<setprecision(2)<< genre.second<<std::endl;
                std::cout << std::endl;
            }
            else{
              std::cout<<"No data available."<<std::endl;
              }

            std::cout<< "Most Energetic Artist: "<<std::endl;
            std::pair<std::string, double> artist = redblacktree.getMostEnergeticArtist();//4
            if (!artist.first.empty()){
                std::cout<< artist.first<<std::endl;
                std::cout<<"Average Energy Level: "<< std::fixed<<setprecision(2)<<artist.second<<std::endl;
                std::cout << std::endl;
            }
            else{
                std::cout<<"No data available."<<std::endl;
            }
            menu();
        }

        else if (option == "3") {
           cout << "Song can be added by this format: " << endl;
            cout << "[Song Name], [Artist], [Energy Level], [Genre]" << endl;
            cin.ignore();
            while (true) {
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

                //*******ADD PERFORMANCE***********
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

        else if (option == "4") {
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

            cout << "\n---------------- Red Black Tree Results ----------" << endl;
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
            
            //////////////////////////////////////////    /Heap ////////////////////////////////
            vector<Song> sonsInRange = songHeap.findSongInRange(l,h);


            //*******ADD PERFORMANCE***********
            cout << "\n-------------------------- Heap Results -------------------- " << endl;
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

            menu();

        }
        else if (option == "5") {

          std::cout << "Type the name of the song or what you remember..."<<std::endl;
          std::string input;
          std::cin.ignore();

          std::getline(std::cin, input);
            cout << "\n---------------- Red Black Tree Results ----------" << endl;
          std::cout << "Searching for " << input<<std::endl;


///////////////////////////////////////////******Red Black Tree search*******//////////////////////////////////////////////
            auto start = std::chrono::high_resolution_clock::now();// measuring time performance of searching in a search operation
          std::vector<Song> matches = redblacktree.search(input);
            auto end = chrono::high_resolution_clock::now();

            ///////////////  Heap   ////////////////
            //*******ADD PERFORMANCE***********
            vector <Song> matchesHeap = songHeap.search(input);

            timeRBsearch += chrono::duration<double>(end - start).count() * 1000; /// MEASURING TIME
            SEARCH++;
          if (matches.empty() && matchesHeap.empty()) {
            std::cout << "No song found" << std::endl;
            menu();
            continue;
          }
          if (matches.size() == 1) {
            std::cout << "Found 1 match!"<<std::endl;
            std::cout<<std::endl;
            std::cout << matches[0].getSongName() <<" by: "<< matches[0].getArtist()<< std::endl;
            std::cout << "Energy Level: "<< matches[0].getEnergy() << std::endl;

          
            cout << "\n -------------- Heap Results ---------------" <<endl;
            for (int i = 0; i < matchesHeap.size(); ++i) {
                cout<< endl;
                cout<< i+1 <<". "<< matches[i].getSongName() << " by: " << matches[i].getArtist()<< endl;
                cout<< "Energy Level: " << matches[i].getEnergy() << endl;
            }



          }
          else if (matches.size() > 1) {
              std::cout << "Matches found: "<<std::endl;
              for (int i = 0; i < matches.size(); ++i) {
                  std::cout<<std::endl;
                  std::cout<< i+1 <<". "<< matches[i].getSongName() << " by: " << matches[i].getArtist()<<std::endl;
                  std::cout<< "Energy Level: " << matches[i].getEnergy() << std::endl;

                  cout << "\n-------------- Heap Results ---------------" <<endl;
                  for (int i = 0; i < matchesHeap.size(); ++i) {
                      cout<< endl;
                      cout<< i+1 <<". "<< matches[i].getSongName() << " by: " << matches[i].getArtist()<< endl;
                      cout<< "Energy Level: " << matches[i].getEnergy() << endl;
                  }
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
            menu();

        }
       
        else if (option == "6") {
            cout << "--- Compare Heap vs Red-Black Tree Performance ---" << endl;
            
            Performance::testInsertion(songs); 
            Performance::testTopNRetrieval(songs, 10); 
            Performance::testRangeQuery(songs, 0.4f, 0.6f);

            menu();
        }
        else if (option == "7") {
            break;
        }
        else {
            cout <<  "Invalid Option" << endl;
        }

    }
    
    
    

}


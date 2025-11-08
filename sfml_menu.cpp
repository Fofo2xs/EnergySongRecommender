#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <limits>
#include <chrono>
#include <optional>
#include <iomanip>
#include <cmath>
#include <map>

#include "Song.h"
#include "Song_loader.h"
#include "Heap.h"
#include "RB.h"
#include "Performance.h"

using namespace std;

vector<Song> songs;
Heap songHeap;
RB redblacktree;
sf::Font font;

double timeRBinsertion = 0;
double timeRBsearch = 0;
int SEARCH = 0;
int INSERT = 0;

enum class AppState {
    MainMenu,
    AddSong,
    RecommendTopN,
    FindSong
};
AppState currentState = AppState::MainMenu;

// button class
struct Button {
    sf::RectangleShape shape;
    sf::Text text;
    std::function<void()> onClick;

    Button(const string& label, sf::Vector2f position, sf::Vector2f size, const sf::Font& font) 
        : text(font, label, 20) 
    {
        shape.setPosition(position);
        shape.setSize(size);
        shape.setFillColor(sf::Color(34, 37, 43)); // button color
        shape.setOutlineColor(sf::Color(255, 255, 255));
        shape.setOutlineThickness(1);

        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin({textBounds.position.x + textBounds.size.x / 2.0f, textBounds.position.y + textBounds.size.y / 2.0f});
        text.setPosition({position.x + size.x / 2.0f, position.y + size.y / 2.0f - 5});
    }

    bool isMouseOver(sf::Vector2f mousePos) const {
        return shape.getGlobalBounds().contains(mousePos);
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(shape);
        window.draw(text);
    }

    void handleHover(sf::Vector2f mousePos) {
        if (isMouseOver(mousePos)) {
            shape.setFillColor(sf::Color(45, 49, 58)); // hover color
        } else {
            shape.setFillColor(sf::Color(34, 37, 43));
        }
    }
};

// textbox class
struct Textbox {
    sf::RectangleShape shape;
    sf::Text text;
    sf::Text label;
    string inputString;
    bool isSelected = false;

    Textbox(const string& labelText, sf::Vector2f position, sf::Vector2f size, const sf::Font& font) 
        : text(font, "", 18), label(font, labelText, 18)
    {
        label.setFillColor(sf::Color::White);
        label.setPosition({position.x, position.y - 30}); 

        shape.setPosition(position);
        shape.setSize(size);
        shape.setFillColor(sf::Color(52, 55, 62)); 
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(1);

        text.setFillColor(sf::Color::White);
        text.setPosition({position.x + 10, position.y + (size.y / 2.0f) - 12});
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(label);
        window.draw(shape);
        window.draw(text);
    }

    void handleEvent(const sf::Event& event) {
        if (const auto* mouseButton = event.getIf<sf::Event::MouseButtonPressed>()) {
            sf::Vector2f mousePos = { (float)mouseButton->position.x, (float)mouseButton->position.y };
            isSelected = shape.getGlobalBounds().contains(mousePos);
        }

        if (isSelected) {
            shape.setFillColor(sf::Color(29, 31, 35)); 
            if (const auto* textEntered = event.getIf<sf::Event::TextEntered>()) {
                if (textEntered->unicode == '\b') { 
                    if (!inputString.empty()) {
                        inputString.pop_back();
                    }
                } else if (textEntered->unicode < 128) { 
                    inputString += static_cast<char>(textEntered->unicode);
                }
                text.setString(inputString);
            }
        } else
        {
            shape.setFillColor(sf::Color(52, 55, 62)); 
        }
    }

    void clear() {
        inputString.clear();
        text.setString("");
    }
};


void drawTitle(sf::RenderWindow& window, const string& titleText) {
    sf::Text title(font, titleText, 48);
    title.setFillColor(sf::Color(225, 193, 126)); // title color
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setOrigin({titleBounds.position.x + titleBounds.size.x / 2.0f, titleBounds.position.y + titleBounds.size.y / 2.0f});
    title.setPosition({(float)window.getSize().x / 2.0f, 60.f});
    window.draw(title);
}

int main() {
    cout << "Loading song data..." << endl;
    songs = loadSongs("dataset.csv");

    if (songs.empty()) {
        cerr << "Error: 'dataset.csv' not found" << endl;
        return 1;
    }
    cout << "Loaded " << songs.size() << " songs." << endl;

    cout << "Building Heap and Red-Black Tree..." << endl;
    for (auto&  song: songs) {
        songHeap.insert(song);
        redblacktree.insert(song);
    }
    cout << "Data structures built. Launching menu." << endl;
    cout << "Output will appear here." << endl;

    sf::RenderWindow window(sf::VideoMode({1000, 750}), "EnergySongRecommender");
    window.setFramerateLimit(60);

    if (!font.openFromFile("../Inter-Regular.ttf")) {
        cerr << "Error: Could not load font" << endl;
        return 1;
    }

    // main menu ui
    vector<Button> mainButtons;
    float btnWidth = 450;
    float btnHeight = 60;
    float gap = 25;
    float x1 = (1000 - (btnWidth * 2 + gap)) / 2;
    float x2 = x1 + btnWidth + gap;
    float y1 = 150;
    float y2 = y1 + btnHeight + gap;
    float y3 = y2 + btnHeight + gap;
    float y4 = y3 + btnHeight + gap;

    mainButtons.push_back(Button("1. View 20 Random Songs", {x1, y1}, {btnWidth, btnHeight}, font));
    mainButtons.push_back(Button("2. View Playlist Statistics", {x2, y1}, {btnWidth, btnHeight}, font));
    mainButtons.push_back(Button("3. Add New Song", {x1, y2}, {btnWidth, btnHeight}, font));
    mainButtons.push_back(Button("4. Recommend Top N", {x2, y2}, {btnWidth, btnHeight}, font));
    mainButtons.push_back(Button("5. Find Song by Name", {x1, y3}, {btnWidth, btnHeight}, font));
    mainButtons.push_back(Button("6. Compare Performance", {x2, y3}, {btnWidth, btnHeight}, font));

    float exitWidth = 450;
    float x_exit = (1000 - exitWidth) / 2;
    mainButtons.push_back(Button("7. Exit", {x_exit, y4}, {exitWidth, btnHeight}, font));


    // add song ui
    vector<Textbox> addSongInputs;
    addSongInputs.push_back(Textbox("Song Name", {100, 200}, {800, 40}, font));
    addSongInputs.push_back(Textbox("Artist", {100, 300}, {800, 40}, font));
    addSongInputs.push_back(Textbox("Energy (0.0 - 1.0)", {100, 400}, {800, 40}, font));
    addSongInputs.push_back(Textbox("Genre", {100, 500}, {800, 40}, font));
    Button addSongSubmit("Add Song", {350, 600}, {300, 50}, font);
    Button addSongBack("Back to Menu", {350, 670}, {300, 50}, font);

    // recommend top n ui
    vector<Textbox> recommendInputs;
    recommendInputs.push_back(Textbox("Number of Songs (N)", {100, 200}, {800, 40}, font));
    recommendInputs.push_back(Textbox("Energy Low (0.0 - 1.0)", {100, 300}, {800, 40}, font));
    recommendInputs.push_back(Textbox("Energy High (0.0 - 1.0)", {100, 400}, {800, 40}, font));
    Button recommendSubmit("Recommend", {350, 600}, {300, 50}, font);
    Button recommendBack("Back to Menu", {350, 670}, {300, 50}, font);

    // find song ui
    vector<Textbox> findSongInputs;
    findSongInputs.push_back(Textbox("Song Name (or part of a name)", {100, 200}, {800, 40}, font));
    Button findSongSubmit("Find Song", {350, 600}, {300, 50}, font);
    Button findSongBack("Back to Menu", {350, 670}, {300, 50}, font);
    
    // main sfml loop
    while (window.isOpen()) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        std::optional<sf::Event> event;
        while (event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (currentState == AppState::AddSong) {
                for(auto& box : addSongInputs) box.handleEvent(*event);
            } else if (currentState == AppState::RecommendTopN) {
                for(auto& box : recommendInputs) box.handleEvent(*event);
            } else if (currentState == AppState::FindSong) {
                for(auto& box : findSongInputs) box.handleEvent(*event);
            }
            
            if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButton->button == sf::Mouse::Button::Left) {
                    
                    if (currentState == AppState::MainMenu) {
                        // view 20 random songs
                        if (mainButtons[0].isMouseOver(mousePos)) { 
                            cout << "\nDisplaying 2O random songs" << endl;
                            std::vector<Song*> randSongs = redblacktree.getRandomSongs(20);
                            if(randSongs.empty()){
                                std::cout << "No songs found" << std::endl;
                            }
                            for (int i = 0; i < randSongs.size(); i++) {
                                std::string mood;
                                double energy = std::round(randSongs[i]->getEnergy()*100.0)/100.0;
                                if (energy>=0 && energy <= 0.29) { mood = "Chillout"; }
                                else if(energy > 0.29 && energy <= 0.49) { mood = "Pleasant"; }
                                else if (energy > 0.49 && energy <= 0.69) { mood = "Moderate"; }
                                else if (energy > 0.69 && energy <= 0.89) { mood = "Energetic";}
                                else {mood = "Pure Hype";}
                                
                                std::cout<< i + 1<<". ";
                                std::cout << randSongs[i]->getSongName() << " by: "<<randSongs[i]->getArtist();
                                std::cout<<" ["<<randSongs[i]->getGenre()<<"] "<< "(Energy: "<<std::fixed<<setprecision(2)<<randSongs[i]->getEnergy()<<") "<< mood<<std::endl;
                            }
                            cout << "Click again for 20 more" << endl;
                        }
                        // view playlist statistics
                        if (mainButtons[1].isMouseOver(mousePos)) { 
                            cout << "\nView Playlist Statistics" << endl;
                            std::cout<< "Average Energy level in the playlist: ";
                            double averageEnergy = redblacktree.calculateAverage();
                            cout << averageEnergy << endl;
                            std::cout << endl;

                            std::cout << "Song with the HIGHEST energy level: ";
                            Song max = redblacktree.findMaxEnergy();
                            std::cout << max.getSongName() <<" by: "<<max.getArtist()<< endl;
                            std::cout <<"Energy level: "<< max.getEnergy() << std::endl;
                            std::cout<<std::endl;

                            std::cout << "Song with the LOWEST energy level: ";
                            Song min = redblacktree.findMinEnergy(); 
                            std::cout << min.getSongName() <<" by: "<<min.getArtist()<< endl;
                            std::cout <<"Energy level: "<< min.getEnergy() << std::endl;
                            std::cout<<std::endl;

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
                                std::cout<<" ("<<std::fixed<<setprecision(1)<<(double)genresVector[i].second*100.0/songs.size()<<"%)"<<std::endl;
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
                                std::cout<<" ("<<std::fixed<<setprecision(1)<<(double)artistsVector[i].second*100.0/songs.size()<<"%)"<<std::endl;
                                std::cout<<std::endl;
                            }
                            std::cout << "Out of " << artists.size() << " artists." << std::endl;
                            std::cout<<std::endl;

                            std::cout<< "Most Energetic Genre: "<<std::endl;
                            std::pair<std::string, double> genre = redblacktree.getMostEnergeticGenre();
                            if (!genre.first.empty()){
                                std::cout<< genre.first<<std::endl;
                                std::cout<<"Average Energy Level: "<<std::fixed<<setprecision(2)<< genre.second<<std::endl;
                                std::cout << std::endl;
                            }
                            
                            std::cout<< "Most Energetic Artist: "<<std::endl;
                            std::pair<std::string, double> artist = redblacktree.getMostEnergeticArtist();
                            if (!artist.first.empty()){
                                std::cout<< artist.first<<std::endl;
                                std::cout<<"Average Energy Level: "<< std::fixed<<setprecision(2)<<artist.second<<std::endl;
                                std::cout << std::endl;
                            }
                        }
                        // add new song
                        if (mainButtons[2].isMouseOver(mousePos)) { 
                            currentState = AppState::AddSong;
                        }
                        // recommend top n
                        if (mainButtons[3].isMouseOver(mousePos)) { 
                            currentState = AppState::RecommendTopN;
                        }
                        // find song by name
                        if (mainButtons[4].isMouseOver(mousePos)) { 
                            currentState = AppState::FindSong;
                        }
                        // compare performance
                        if (mainButtons[5].isMouseOver(mousePos)) { 
                            cout << "\nCompare Performance" << endl;
                            Performance::testInsertion(songs);
                            Performance::testTopNRetrieval(songs, 10);
                            Performance::testRangeQuery(songs, 0.4f, 0.6f);
                        }
                        // exit
                        if (mainButtons[6].isMouseOver(mousePos)) { 
                            window.close();
                        }
                    } 
                    
                    // add song clicks
                    else if (currentState == AppState::AddSong) {
                        if (addSongBack.isMouseOver(mousePos)) {
                            currentState = AppState::MainMenu;
                        }
                        if (addSongSubmit.isMouseOver(mousePos)) {
                            cout << "\nAdd New Song" << endl;
                            try {
                                string name = addSongInputs[0].inputString;
                                string artist = addSongInputs[1].inputString;
                                double energy = stod(addSongInputs[2].inputString);
                                string genre = addSongInputs[3].inputString;

                                if (name.empty() || artist.empty() || addSongInputs[2].inputString.empty() || genre.empty()) {
                                    cout << "Error: Please fill all required fields." << endl;
                                } else if (energy < 0.0 || energy > 1.0) {
                                    cout << "Error: Invalid energy value. Must be 0.0 - 1.0" << endl;
                                } else {
                                    Song newSong(name, artist, energy, genre);
                                    songs.push_back(newSong);
                                    songHeap.insert(newSong);
                                    redblacktree.insert(newSong);

                                    cout << "Successfully added: " << name << endl;
                                    for(auto& box : addSongInputs) box.clear();
                                    currentState = AppState::MainMenu;
                                }
                            } catch (const std::exception& e) {
                                cout << "Error: Invalid input. Energy must be a number." << endl;
                            }
                        }
                    }

                    // recommend top n click
                    else if (currentState == AppState::RecommendTopN) {
                        if (recommendBack.isMouseOver(mousePos)) {
                            currentState = AppState::MainMenu;
                        }
                        if (recommendSubmit.isMouseOver(mousePos)) {
                            cout << "\nRecommend Top N Songs" << endl;
                            try {
                                int n = stoi(recommendInputs[0].inputString);
                                float l = stof(recommendInputs[1].inputString);
                                float h = stof(recommendInputs[2].inputString);

                                if (l < 0 || h < 0 || l > h) {
                                     cout << "Error: Invalid Range. please enter positive values between 0.0 and 1.0" << endl;
                                } else {
                                    auto start = std::chrono::high_resolution_clock::now();
                                    std::vector<Song> Topsongs = redblacktree.topNenergy(n, l, h);
                                    auto end = std::chrono::high_resolution_clock::now();
                                    timeRBsearch += chrono::duration<double>(end - start).count() * 1000;
                                    SEARCH++;

                                    for (auto i = 0; i < Topsongs.size(); i++) {
                                        cout << i + 1 << ". " << Topsongs[i].getSongName() << " by: " << Topsongs[i].getArtist() << " | Energy: " << Topsongs[i].getEnergy() << endl;
                                    }
                                    for(auto& box : recommendInputs) box.clear();
                                    currentState = AppState::MainMenu;
                                }
                            } catch (const std::exception& e) {
                                cout << "Error: Invalid input. Please enter numbers." << endl;
                            }
                        }
                    }

                    // find song clicks
                    else if (currentState == AppState::FindSong) {
                        if (findSongBack.isMouseOver(mousePos)) {
                            currentState = AppState::MainMenu;
                        }
                        if (findSongSubmit.isMouseOver(mousePos)) {
                            string input = findSongInputs[0].inputString;
                            cout << "\nFind Song by Name" << endl;
                            cout << "Searching for: " << input << endl;

                            auto start = std::chrono::high_resolution_clock::now();
                            std::vector<Song> matches = redblacktree.search(input);
                            auto end = chrono::high_resolution_clock::now();
                            timeRBsearch += chrono::duration<double>(end - start).count() * 1000;
                            SEARCH++;

                            if (matches.empty()) {
                                cout << "No song found." << endl;
                            } else {
                                for (int i = 0; i < matches.size(); ++i) {
                                    cout << i + 1 << ". " << matches[i].getSongName() << " by: " << matches[i].getArtist() << " | Energy: " << matches[i].getEnergy() << endl;
                                }
                            }
                            for(auto& box : findSongInputs) box.clear();
                            currentState = AppState::MainMenu;
                        }
                    }
                }
            }
        }

        // hover effects
        if (currentState == AppState::MainMenu) {
            for (auto& button : mainButtons) button.handleHover(mousePos);
        } else if (currentState == AppState::AddSong) {
            addSongSubmit.handleHover(mousePos);
            addSongBack.handleHover(mousePos);
        } else if (currentState == AppState::RecommendTopN) {
            recommendSubmit.handleHover(mousePos);
            recommendBack.handleHover(mousePos);
        } else if (currentState == AppState::FindSong) {
            findSongSubmit.handleHover(mousePos);
            findSongBack.handleHover(mousePos);
        }

        window.clear(sf::Color(41, 44, 52)); // background

        // draw state
        if (currentState == AppState::MainMenu) {
            drawTitle(window, "Main Menu");
            for (const auto& button : mainButtons) button.draw(window);
        } 
        else if (currentState == AppState::AddSong) {
            drawTitle(window, "Add a New Song");
            for (const auto& box : addSongInputs) box.draw(window);
            addSongSubmit.draw(window);
            addSongBack.draw(window);
        }
        else if (currentState == AppState::RecommendTopN) {
            drawTitle(window, "Recommend Top N Songs");
            for (const auto& box : recommendInputs) box.draw(window);
            recommendSubmit.draw(window);
            recommendBack.draw(window);
        }
        else if (currentState == AppState::FindSong) {
            drawTitle(window, "Find Song by Name");
            for (const auto& box : findSongInputs) box.draw(window);
            findSongSubmit.draw(window);
            findSongBack.draw(window);
        }

        window.display();
    }

    return 0;
}
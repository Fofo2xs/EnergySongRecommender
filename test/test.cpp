
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <vector>
#include <string>
#include "../Song.h"
#include "../Song_loader.h"
#include "../Heap.h"

using namespace std;


// Test Case 1: Song Loader
TEST_CASE("1. Test Song Loader", "[loader]") {

    SECTION("Loading the real dataset") {
        vector<Song> songs = loadSongs("../dataset.csv");
        
        REQUIRE(!songs.empty());
        REQUIRE(songs.size() > 0);
        
        Song firstSong = songs[0];
        REQUIRE(!firstSong.getSongName().empty());
        REQUIRE(!firstSong.getArtist().empty());
        REQUIRE(firstSong.getEnergy() >= 0.0);
        REQUIRE(firstSong.getEnergy() <= 1.0);
    }

    SECTION("Attempting to load a non-existent file") {
        vector<Song> songs = loadSongs("12345.csv");
        REQUIRE(songs.empty());
        REQUIRE(songs.size() == 0);
    }
}

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


// Test Case 2: Heap Data Structure
TEST_CASE("2. Test Heap Operations", "[heap]") {

    Song low("Low Song", "Artist", 0.2, "Genre");
    Song mid("Mid Song", "Artist", 0.6, "Genre");
    Song high("High Song", "Artist", 0.9, "Genre");

    SECTION("Insert and getMax (Max-Heap)") {
        Heap songHeap;
        REQUIRE(songHeap.isEmpty());
        
        // Insert mid
        songHeap.insert(mid);
        REQUIRE(!songHeap.isEmpty());
        REQUIRE(songHeap.size() == 1);
        REQUIRE(songHeap.getMax().getEnergy() == 0.6); // new max

        // Insert high
        songHeap.insert(high);
        REQUIRE(songHeap.size() == 2);
        REQUIRE(songHeap.getMax().getEnergy() == 0.9); // new max

        // Insert low
        songHeap.insert(low);
        REQUIRE(songHeap.size() == 3);
        REQUIRE(songHeap.getMax().getEnergy() == 0.9); // max is still high
    }

    SECTION("extractMax and Heapify-Down") {
        Heap songHeap;
        songHeap.insert(low);
        songHeap.insert(high);
        songHeap.insert(mid);
        
        REQUIRE(songHeap.size() == 3);
        
        // 1. Extract "High" (0.9)
        Song extracted1 = songHeap.extractMax();
        REQUIRE(extracted1.getEnergy() == 0.9);
        REQUIRE(songHeap.size() == 2);
        
        // 2. New max should be "Mid" (0.6)
        REQUIRE(songHeap.getMax().getEnergy() == 0.6);

        // 3. Extract "Mid" (0.6)
        Song extracted2 = songHeap.extractMax();
        REQUIRE(extracted2.getEnergy() == 0.6);
        REQUIRE(songHeap.size() == 1);

        // 4. New max should be "Low" (0.2)
        REQUIRE(songHeap.getMax().getEnergy() == 0.2);
        
        // 5. Extract "Low" (0.2)
        Song extracted3 = songHeap.extractMax();
        REQUIRE(extracted3.getEnergy() == 0.2);
        REQUIRE(songHeap.size() == 0);
        REQUIRE(songHeap.isEmpty());
    }

    SECTION("Heap Edge Cases") {
        Heap emptyHeap;
        
        // Test operations on an empty heap
        REQUIRE(emptyHeap.isEmpty());
        REQUIRE(emptyHeap.size() == 0);
        
        // Check what getMax() returns on empty
        Song defaultSong = emptyHeap.getMax();
        REQUIRE(defaultSong.getEnergy() == 0.0); 
        
        // Check what extractMax() returns on empty
        Song extractedDefault = emptyHeap.extractMax();
        REQUIRE(extractedDefault.getEnergy() == 0.0); 
        
        REQUIRE(emptyHeap.isEmpty());
    }
}
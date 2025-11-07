
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <vector>
#include <string>
#include "../Song.h"
#include "../Song_loader.h"
#include "../Heap.h"
#include "../RB.h"

using namespace std;


// song loader
TEST_CASE("1. Test Song Loader", "[loader]") {

    SECTION("Loading the real dataset") {
        vector<Song> songs = loadSongs("dataset.csv");
        
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


// heap data structure
TEST_CASE("2. Test Heap Operations", "[heap]") {

    Song low("Low Song", "Artist", 0.2, "Genre");
    Song mid("Mid Song", "Artist", 0.6, "Genre");
    Song high("High Song", "Artist", 0.9, "Genre");

    SECTION("Insert and getMax (Max-Heap)") {
        Heap songHeap;
        REQUIRE(songHeap.isEmpty());
        
        // insert mid
        songHeap.insert(mid);
        REQUIRE(!songHeap.isEmpty());
        REQUIRE(songHeap.size() == 1);
        REQUIRE(songHeap.getMax().getEnergy() == 0.6); // new max

        // insert high
        songHeap.insert(high);
        REQUIRE(songHeap.size() == 2);
        REQUIRE(songHeap.getMax().getEnergy() == 0.9); // new max

        // insert low
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
        
        // extract "high" (0.9)
        Song extracted1 = songHeap.extractMax();
        REQUIRE(extracted1.getEnergy() == 0.9);
        REQUIRE(songHeap.size() == 2);
        
        // new max should be "mid" (0.6)
        REQUIRE(songHeap.getMax().getEnergy() == 0.6);

        // extract "mid" (0.6)
        Song extracted2 = songHeap.extractMax();
        REQUIRE(extracted2.getEnergy() == 0.6);
        REQUIRE(songHeap.size() == 1);

        // new max should be "low" (0.2)
        REQUIRE(songHeap.getMax().getEnergy() == 0.2);
        
        // extract "low" (0.2)
        Song extracted3 = songHeap.extractMax();
        REQUIRE(extracted3.getEnergy() == 0.2);
        REQUIRE(songHeap.size() == 0);
        REQUIRE(songHeap.isEmpty());
    }

    SECTION("Heap Edge Cases") {
        Heap emptyHeap;
        
        // test operations on empty heap
        REQUIRE(emptyHeap.isEmpty());
        REQUIRE(emptyHeap.size() == 0);
        
        // check what getMax() returns on empty
        Song defaultSong = emptyHeap.getMax();
        REQUIRE(defaultSong.getEnergy() == 0.0); 
        
        // check what extractMax() returns on empty
        Song extractedDefault = emptyHeap.extractMax();
        REQUIRE(extractedDefault.getEnergy() == 0.0); 
        
        REQUIRE(emptyHeap.isEmpty());
    }
}

TEST_CASE("3. Test Red-Black Tree Operations", "[rb]") {

    Song s_a_05("Song A", "Artist 1", 0.5, "Genre");
    Song s_b_03("Song B", "Artist 2", 0.3, "Genre");
    Song s_c_07("Song C", "Artist 3", 0.7, "Genre");
    Song s_d_05("Song D", "Artist 4", 0.5, "Genre"); // duplicate energy
    Song s_a_09("Song A", "Artist 5", 0.9, "Genre"); // duplicate name

    SECTION("Empty Tree Search") {
        RB tree;
        std::vector<Song> results = tree.search("non-existent");
        REQUIRE(results.empty());
    }

    SECTION("Insert Multiple and Search for Duplicate Names") {
        RB tree;
        tree.insert(s_a_05);
        tree.insert(s_b_03);
        tree.insert(s_a_09);

        std::vector<Song> results = tree.search("Song A");

        REQUIRE(results.size() == 2);

        bool found05 = false;
        bool found09 = false;
        for (const auto& song : results) {
            if (song.getEnergy() == 0.5) found05 = true;
            if (song.getEnergy() == 0.9) found09 = true;
        }
        REQUIRE(found05);
        REQUIRE(found09);
    }

    SECTION("Search for Non-Existent Song in Populated Tree") {
        RB tree;
        tree.insert(s_a_05);
        tree.insert(s_b_03);
        tree.insert(s_c_07);
        std::vector<Song> results = tree.search("Fake Song");
        REQUIRE(results.empty());
    }

    SECTION("Test Balancing with Sorted Insert") {
        RB tree;
        Song s1("s1", "a", 0.1, "g");
        Song s2("s2", "a", 0.2, "g");
        Song s3("s3", "a", 0.3, "g");
        Song s4("s4", "a", 0.4, "g");
        Song s5("s5", "a", 0.5, "g");
        
        tree.insert(s1); 
        tree.insert(s2); 
        tree.insert(s3); 
        tree.insert(s4); 
        tree.insert(s5);
        
        std::vector<Song> r1 = tree.search("s1");
        std::vector<Song> r3 = tree.search("s3");
        std::vector<Song> r5 = tree.search("s5");

        REQUIRE(r1.size() == 1);
        if (r1.size() == 1) REQUIRE(r1[0].getEnergy() == 0.1);
        
        REQUIRE(r3.size() == 1);
        if (r3.size() == 1) REQUIRE(r3[0].getEnergy() == 0.3);

        REQUIRE(r5.size() == 1);
        if (r5.size() == 1) REQUIRE(r5[0].getEnergy() == 0.5);
    }
}
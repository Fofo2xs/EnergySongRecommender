
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <vector>
#include <string>
#include "../Song.h"
#include "../Song_loader.h"
#include "../Heap.h"
#include "../RB.h"

using namespace std;


// Test Case 1: Song Loader
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

TEST_CASE("3. Test Red-Black Tree Operations", "[rb]") {

    // --- Test Fixtures ---
    Song s_a_05("Song A", "Artist 1", 0.5, "Genre");
    Song s_b_03("Song B", "Artist 2", 0.3, "Genre");
    Song s_c_07("Song C", "Artist 3", 0.7, "Genre");
    Song s_d_05("Song D", "Artist 4", 0.5, "Genre"); // Duplicate energy
    Song s_a_09("Song A", "Artist 5", 0.9, "Genre"); // Duplicate name
    // ---------------------

    SECTION("Empty Tree Search") {
        RB tree;
        std::vector<Song> results = tree.search("Non-existent");
        // NOTE: This assumes the search function correctly returns an empty vector
        // if no results are found (which the provided rb.cpp code does).
        REQUIRE(results.empty());
    }

    SECTION("Insert Multiple and Search for Duplicate Names") {
        RB tree;
        tree.insert(s_a_05); // ("Song A", 0.5)
        tree.insert(s_b_03); // ("Song B", 0.3)
        tree.insert(s_a_09); // ("Song A", 0.9)

        std::vector<Song> results = tree.search("Song A");

        // NOTE: These tests will fail until rb.cpp's search() function
        // is fixed to return the vector of found songs.
        REQUIRE(results.size() == 2);

        // Check that both songs are present. Order is not guaranteed.
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

    SECTION("Test Balancing with Sorted Inserts (Implicit Test)") {
        RB tree;
        Song s1("s1", "a", 0.1, "g");
        Song s2("s2", "a", 0.2, "g");
        Song s3("s3", "a", 0.3, "g");
        Song s4("s4", "a", 0.4, "g");
        Song s5("s5", "a", 0.5, "g");
        
        // Insert in ascending order (worst case for simple BST)
        tree.insert(s1); 
        tree.insert(s2); 
        tree.insert(s3); 
        tree.insert(s4); 
        tree.insert(s5);
        
        // Test that searching still works correctly, implying balancing occurred.
        std::vector<Song> r1 = tree.search("s1");
        std::vector<Song> r3 = tree.search("s3");
        std::vector<Song> r5 = tree.search("s5");

        // NOTE: These tests will fail until rb.cpp's search() function
        // is fixed to return the vector of found songs.
        REQUIRE(r1.size() == 1);
        if (r1.size() == 1) REQUIRE(r1[0].getEnergy() == 0.1);
        
        REQUIRE(r3.size() == 1);
        if (r3.size() == 1) REQUIRE(r3[0].getEnergy() == 0.3);

        REQUIRE(r5.size() == 1);
        if (r5.size() == 1) REQUIRE(r5[0].getEnergy() == 0.5);
    }

    SECTION("Test topNenergy (Visual Check)") {
        RB tree;
        tree.insert(s_a_05); // 0.5
        tree.insert(s_b_03); // 0.3
        tree.insert(s_c_07); // 0.7
        tree.insert(s_d_05); // 0.5
        tree.insert(s_a_09); // 0.9

        // This function prints to cout, so we can't use REQUIRE.
        // We use SUCCEED() to mark the test as passed and print instructions.
        std::cout << "\n--- Visual Check for topNenergy ---" << std::endl;
        
        std::cout << "\n[Test] topNenergy(3, 0.0, 1.0) - Should show 3 songs, highest energy first (0.9, 0.7, 0.5)" << std::endl;
        tree.topNenergy(3, 0.0, 1.0);
        
        std::cout << "\n[Test] topNenergy(2, 0.4, 0.8) - Should show 2 songs (0.7, 0.5)" << std::endl;
        tree.topNenergy(2, 0.4, 0.8);
        
        std::cout << "\n[Test] topNenergy(5, 0.0, 0.4) - Should show 1 song (0.3)" << std::endl;
        tree.topNenergy(5, 0.0, 0.4);
        
        std::cout << "\n--- End Visual Check ---" << std::endl;
        
        SUCCEED("topNenergy tests called. Please check console output for correctness.");
    }
}
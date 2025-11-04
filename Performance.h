#pragma once
#include <vector>
#include "Song.h"

/*
Performance class for timing data structures. All the methods are static and print results to cout.
*/
class Performance {
public:
    static void testInsertion(const std::vector<Song>& songs);
    
    static void testTopNRetrieval(const std::vector<Song>& songs, int n);
    
    static void testRangeQuery(const std::vector<Song>& songs, float energyLow, float energyHigh);
};
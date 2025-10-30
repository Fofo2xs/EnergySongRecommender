cmake_minimum_required(VERSION 3.30)
project(EnergySongRecommender)

set(CMAKE_CXX_STANDARD 20)

add_executable(EnergySongRecommender main.cpp  Song.cpp Heap.cpp Song_loader.cpp)


cmake_minimum_required(VERSION 3.30)
project(EnergySongRecommender)

set(CMAKE_CXX_STANDARD 20)

Include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.5.2
)
FetchContent_MakeAvailable(Catch2)

set(CATCH_BUILD_STATIC_LIB ON CACHE BOOL "")

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(SFML)

add_executable(EnergySongRecommender main.cpp Song.cpp Heap.cpp Song_loader.cpp RB.cpp Performance.cpp)

add_executable(Tests test/test.cpp Song.cpp Heap.cpp Song_loader.cpp RB.cpp)
target_link_libraries(Tests PRIVATE Catch2::Catch2WithMain)

add_executable(SongMenu sfml_menu.cpp Song.cpp Heap.cpp Song_loader.cpp RB.cpp Performance.cpp)
target_link_libraries(SongMenu PRIVATE sfml-graphics sfml-window sfml-system)

include(CTest)
include(Catch)
catch_discover_tests(Tests)
//
// Created by danie on 10/31/2025.
//
#ifndef RB_H
#define RB_H
#include <string>
#include <iostream>
#include <sstream>
#include <regex>
#include "Song.h"

enum Color{
    RED,
    BLACK
};

class RB {
    private:
    class Node {
        public:

        int height;
        Song song;
        Color color;
        //int balance;
        Node* parent;
        Node* left;
        Node* right;
        Node (const Song& s) : song(s), color(RED), parent(nullptr), left(nullptr), right(nullptr) {};
        Node(): color(BLACK), parent(nullptr), left(nullptr), right(nullptr) {};

    };

    Node* root;
    Node* nil;

    void balanceTree(Node* node); //DONE
    void leftRotate(Node* node);//DONE
    void rightRotate(Node* node);//DONE

    void ReverseInOrderHelper(Node* node, float low, float high, int& counter, int n, std::vector<Song>& res) const; //DONE
    void searchHelper(Node* node, const std::string& name,std::vector<Song>& result) const; //DONE


public:

    RB();

    void insert(const Song& song); //DONE
    std::vector<Song> search(const string& name);//DONE
    std::vector<Song> topNenergy(int n, float l, float h) const; //DONE

    RB(const RB& obj);
    //RB& operator=(const RB& obj);

    ~RB();
    void destructorHelper(Node* node);

    void printTrace(Node* node, int depth);
   // Node* helperCopy(Node* rhs);
};


#endif //RB_H

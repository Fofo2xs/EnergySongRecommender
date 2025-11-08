//
// Created by danie on 10/31/2025.
//
#include "RB.h"
#include "Song.h"
#include <ctime>
#include <cstdlib>
#include <set>

RB::RB() {
    nil = new Node();
    nil->color = BLACK;
    nil->left = nil;
    nil->right = nil;
    nil->parent = nil;
    root = nil;
    //std::cout << "nil address: " << nil << std::endl;  // ADD THIS
}

 void RB::insert(const Song &song) {
  Node *newNode = new Node(song);
     Node* parent = nil;
    // newNode->parent = parent;
     newNode->color = RED;
    newNode->left = nil;
    newNode->right = nil;
    if (root == nil) {//newNode is the root
        root = newNode;
        root->left = nil;
        root->right = nil;
        root->parent=nil;
        root->color = BLACK;
        return;

    }
    Node* current = root;
    while (current != nil) {
        parent = current;
        if (song.getEnergy()<current->song.getEnergy()) {
            current = current->left;
        }
        else if (song.getEnergy()>current->song.getEnergy()) {
            current = current->right;
        }
        else {
            if (song.getSongName()<current->song.getSongName()) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }
    }
    newNode->parent = parent; // make the new node parent point to its parent
    if (song.getEnergy() < parent->song.getEnergy()) {// attactch new node
        parent->left = newNode;
    }
    else if (song.getEnergy() > parent->song.getEnergy()) {
        parent->right = newNode;
    }
    else {
        if (song.getSongName()<parent->song.getSongName()) {
            parent->left = newNode;
        }
        else {
            parent->right = newNode;
        }
        //parent->right = newNode;
    }
    balanceTree(newNode);

}
void RB::leftRotate(Node *node) {
   // std::cout << "leftRotate called with node: " << node << std::endl;
    //std::cout << "node->parent: " << node->parent << std::endl;
    //std::cout << "node->right: " << node->right << std::endl;
    //std::cout << "nil: " << nil << std::endl;

    Node* newroot = nullptr;
    if (node == nil || node->right == nil) {
       // std::cout << "Early return from leftRotate" << std::endl;
        return;
    }
    //std::cout << "Setting newroot" << std::endl;
    newroot = node->right;

   // std::cout << "Setting node->right" << std::endl;
    node->right = newroot->left;

    //std::cout << "Checking newroot->left" << std::endl;
    if (newroot->left != nil) {
        newroot->left->parent = node;
    }
   // std::cout << "Setting newroot->parent" << std::endl;
    newroot->parent = node->parent;//attach the new root to the parent of the rotated node

   // std::cout << "Checking node->parent" << std::endl;
    if (node->parent == nil) {//node was the root
        root = newroot;
    }
    else if (node == node->parent->right) {
        //std::cout << "node is right child" << std::endl;
        node->parent->right = newroot;
    }
    else {
       // std::cout << "node is left child" << std::endl;
        node->parent->left = newroot;
    }
    //std::cout << "Final assignments" << std::endl;
    newroot->left = node;
    node->parent = newroot; // now the parent of the rotated node points to the created new root of the subtree
   // std::cout << "leftRotate completed" << std::endl;

}


void RB::rightRotate(Node *node) {
    Node* newroot = nullptr;
    if (node == nil || node->left == nil) {

        return;
    }
    newroot = node->left;
    node->left = newroot->right;
    if (newroot->right != nil) {
        newroot->right->parent = node;
    }
    newroot->parent = node->parent;//attach the new root to the parent of the rotated node
    if (node->parent == nil) {//node was the root
        root = newroot;
    }
    else if (node == node->parent->right) {
        node->parent->right = newroot;
    }
    else {
        node->parent->left = newroot;
    }
    newroot->right = node;
    node->parent = newroot; // now the parent of the rotated node points to the created new root of the subtree

}


void RB::balanceTree(Node *node) {
    // Node* pare= node->parent;
    // Node* gparent = node->parent->parent;
    while (node != root && node->parent->color == RED) {
        Node* pare= node->parent;
        Node* gparent = node->parent->parent;
        if (pare == gparent->right) {
            Node* uncle = gparent->left;
            if (uncle->color == RED) {
                gparent->color= RED;
                pare->color = BLACK;
                uncle->color = BLACK;
                node = gparent;
            }
            else {
                if (node ==pare->left) {
                    //std::cout << "before rightRotate in RL case" << std::endl;
                   // std::cout << "node: " << node << ", parent: " << node->parent << std::endl;
                    node= pare;
                    rightRotate(node);
                    //std::cout << "After rightRotate" << std::endl;
                    //std::cout << "node: " << node << ", parent: " << node->parent << std::endl;
                   // pare = node->parent;
                   // gparent = pare->parent;
                }

                // node->color = BLACK;
              //  std::cout << "Before leftRotate in RR case" << std::endl;
              //  std::cout << "gparent: " << gparent << ", pare: " << pare << std::endl;
                gparent->color =RED;
                pare->color = BLACK;
                leftRotate(gparent);
               // std::cout << "After leftRotate" << std::endl;
              //  break;
            }
        }
        else {
            Node* uncle = gparent->right;
            if (uncle->color == RED) {
                gparent->color = RED;
                pare->color = BLACK;
                uncle->color = BLACK;
                node = gparent;
            }
            else {
                if (node == pare->right) {
                    node = pare;
                    leftRotate(node);
                    //pare = node->parent;
                    //gparent = pare->parent;

                }

                // node->color = BLACK;
                gparent->color = RED;
                pare->color = BLACK;
                rightRotate(gparent);
                //break;
            }
        }
    }
    root->color = BLACK;
}


std::vector<Song> RB::search(const string &name) {
    std::string lowerName = name;
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
   // std::vector<Node*> results;
    std::vector<Song> matches;
    searchHelper(root, lowerName, matches);

    return matches;
}

void RB::searchHelper(Node *node, const std::string& name, std::vector<Song> &result) const {
    if (node == nil) {
        return;
    }
    // std::string copySTR = name;
    // std::transform(copySTR.begin(), copySTR.end(), copySTR.begin(), ::tolower);
    std::string lowerSong = node->song.getSongName();
    std::transform(lowerSong.begin(), lowerSong.end(), lowerSong.begin(), ::tolower);
    if (lowerSong.find(name) != std::string::npos) {
        result.push_back(node->song);
    }
    searchHelper(node->left, name, result);
    searchHelper(node->right, name, result);
}

std::vector<Song> RB::topNenergy(int n, float l, float h) const {
    int count = 0;
    std::vector<Song> result;
    ReverseInOrderHelper(root, l, h, count, n, result);

    return result;
}

void RB::ReverseInOrderHelper(Node* node, float l, float h, int& count, int n, std::vector<Song>& res) const{
    if (node ==nil|| count >= n) {
        return;
    }
    ReverseInOrderHelper(node->right, l, h, count, n, res);
    if (count<n && node->song.getEnergy()>= l && node->song.getEnergy()<= h) {
        res.push_back(node->song);
        count++;
    }
   ReverseInOrderHelper(node->left, l, h, count, n, res);
}
/////////////-----------------Main: option 2-----------/////////////////
Song RB::findMinEnergy() {
    Node* node = findMinEnergyHelper(root);
    return node->song;
}
RB::Node* RB::findMinEnergyHelper(Node* node) {

    if (node == nil) {
        throw std::runtime_error("Tree is empty");
    }
    if (node->left== nil) {
        return node;
    }
    return findMinEnergyHelper(node->left);
}
RB::Node* RB::findMaxEnergyHelper(Node *node) {
    if (node == nil) {
        throw std::runtime_error("Tree is empty");
    }
    if (node->right== nil) {
        return node;
    }
    return findMaxEnergyHelper(node->right);
}

void RB::calculateAverageHelper(Node *node, int& n, double& sum) {

    if (node == nil) {
        return;
    }
    calculateAverageHelper(node->left,n, sum);
    sum+= node->song.getEnergy();
    n++;
    calculateAverageHelper(node->right,n, sum);

}

void RB::getDataHelper(Node *node, std::map<std::string, std::pair<double, int>> &res, bool genre) {
    if (node == nil) {
        return;
    }
    getDataHelper(node->left, res, genre);
    std::string key = (genre)?node->song.getGenre():node->song.getArtist();
    res[key].first += node->song.getEnergy();
    res[key].second++;

    getDataHelper(node->right, res,genre);
}


std::pair<std::string, double> RB::getMostEnergeticGenre() {
    if (root == nil) {
        return std::make_pair("", 0);
    }
    std::map<std::string, std::pair<double, int>> res;
    getDataHelper(root, res, true);
    if (res.empty()) {
        return std::make_pair("", 0);
    }
    std::string topGenre = res.begin()->first;
    double topAvg = res.begin()->second.first/res.begin()->second.second;
    for (const auto& p : res) {
        double newAvg = p.second.first/p.second.second;
        if (newAvg > topAvg) {
            topAvg = newAvg;
            topGenre = p.first;
        }
    }
return std::make_pair(topGenre, topAvg);

}

std::pair<std::string, double> RB::getMostEnergeticArtist() {
    if (root == nil) {
        return std::make_pair("", 0);
    }

    std::map<std::string, std::pair<double, int>> res;
    getDataHelper(root, res, false);
    if (res.empty()) {
        return std::make_pair("", 0);
    }
    std::string topArt = res.begin()->first;
    double topAvg = res.begin()->second.first/res.begin()->second.second;
    for (const auto& p : res) {
        double newAvg = p.second.first/p.second.second;
        if (newAvg > topAvg) {
            topAvg = newAvg;
            topArt = p.first;
        }
    }
    return {topArt, topAvg};
}

std::map<std::string, int> RB::getGenreCount() {
    std::map<std::string, int> res;
    std::map<std::string, std::pair<double,int>> dat;
    getDataHelper(root, dat, true);
    for (const auto& p : dat) {
        res[p.first]=p.second.second;
    }
    return res;
}

std::map<std::string, int> RB::getArtistCount() {
    std::map<std::string, int> res;
    std::map<std::string, std::pair<double,int>> dat;
    getDataHelper(root, dat, false);
    for (const auto& p : dat) {
        res[p.first]=p.second.second;
    }
    return res;
}

double RB::calculateAverage() {
    if (root == nil) {
        return 0.0;
    }
    int num = 0;
    double sum = 0.0;
    calculateAverageHelper(root, num, sum);
    return sum/num;

}

Song RB::findMaxEnergy() {
    Node* node = findMaxEnergyHelper(root);
    return node->song;
}
/////////////-----------------Main: option 2-----------/////////////////
///
///
////////////------------------Main: option 1--------------////////////////
void RB::getRandomSongsHelper(Node *node, std::vector<Song *> &res) const {
    if (node == nil) {
        return;
    }
    getRandomSongsHelper(node->left, res);
    res.push_back(&node->song);
    getRandomSongsHelper(node->right, res);
}

std::vector<Song *> RB::getRandomSongs(int n) {
    std::vector<Song *> songs;
    getRandomSongsHelper(root, songs);
    std::vector<Song *> res;
    if (songs.empty()) {
        return res;
    }
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    std::set<int> indexes;
    while (indexes.size() < n) {
        int i = std::rand() % songs.size();
        if (indexes.find(i) == indexes.end()) {
            indexes.insert(i);
            res.push_back(songs[i]);
        }
    }
    return res;
}
////////////------------------Main: option 1--------------////////////////
void RB::destructorHelper(Node *node) {
    if (node == nil) {
        return;
    }
    destructorHelper(node->left);
    destructorHelper(node->right);
    //std::cout<<"deleted";
    delete node;
}

RB::~RB() {
    destructorHelper(root);
    delete nil;
    root= nullptr;
    nil= nullptr;
}

void RB::printTrace(Node* node, int depth) {
    if (node == nullptr) { std::cout << std::string(depth,' ') << "nullptr\n"; return; }
    if (node == nil)     { std::cout << std::string(depth,' ') << "NIL\n"; return; }

    std::cout << std::string(depth,' ') << node->song.getEnergy() << "\n";
    printTrace(node->left, depth+2);
    printTrace(node->right, depth+2);
}

//RB::Node * RB::helperCopy(Node *rhs) {
//}


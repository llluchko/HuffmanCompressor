//
//  Compressor.hpp
//  HuffmanCompressor
//
//  Created by Latchezar Mladenov on 2/15/16.
//  Copyright © 2016 Latchezar Mladenov. All rights reserved.
//

#ifndef Compressor_h
#define Compressor_h

#include <vector>
#include <fstream>
#include <cmath>
#include <iostream>
using namespace std;

struct Pair {
    char ch;
    string code;
};

class Compressor {
private:
    bool done;
    short endBlanks;
    vector<Pair> map;
    vector<unsigned char> head;
    string bodyBuffer;
    string fromFile;
    string toFile;
    void createHead();
    void createBody();
    void clearBuffer();
    string loadBuffer(ifstream&);
public:
    Compressor(vector<Pair>&);
    ~Compressor(void);
    void compress(string, string);
};

#endif /* Compressor_h */
